#include "tcp_connection.hpp"
#include "socketstream.hpp"
#include "handlemessage.hpp"
#include "classregistry.hpp"
//#include <google/profiler.h>
#include "trace.hpp"

using namespace std;
using namespace boost;
using namespace boost::asio;
using namespace boost::asio::ip;

namespace cobject
{

    tcp_connection::pointer tcp_connection::create(io_service &io_service)
    {
		TRACE;
        return pointer(new tcp_connection(io_service));
    }

    tcp::socket& tcp_connection::socket()
    {
		TRACE;
        return _socket;
    }

    void tcp_connection::start()
    {
		TRACE;
//    	ProfilerStart("/tmp/cobject.prof");
        if (!_socket.is_open()) return;
        _message = "CO_BROKER";
        socketstream s(_socket);
        Serialize(s, _message);
        _msgID=0xdead;
        async_read(_socket, buffer(&_msgID, sizeof(_msgID)), bind(&tcp_connection::handle_read, shared_from_this(), asio::placeholders::error,
                   asio::placeholders::bytes_transferred));
    }

    void tcp_connection::end_connection()
    {
		TRACE;
        _endconn=true;
    }

    void tcp_connection::handle_read(const boost::system::error_code &ec, size_t s)
    {
		TRACE;
        if (ec) return;
        cout << "Recieved MessageID " << _msgID << " from " << this << "." << endl;
        HandleMessage(_msgID, shared_from_this());
        if (!_endconn) async_read(_socket, buffer(&_msgID,sizeof(_msgID)), bind(&tcp_connection::handle_read, shared_from_this(),
                                      asio::placeholders::error, asio::placeholders::bytes_transferred));
    }

    tcp_connection::~tcp_connection()
    {
		TRACE;
        cout << "Connection from " << _socket.remote_endpoint().address().to_string();
        cout << ":" << _socket.remote_endpoint().port();
        cout << " (ID:" << this << ")";
        cout << " terminated." << endl;
        GetClassRegistry().NotifyDisconnect(this);
//        ProfilerStop();
    }

    void tcp_connection::send(const string &s)
    {
		TRACE;
        async_write(_socket, buffer(s), bind(&tcp_connection::handle_write, shared_from_this(), asio::placeholders::error,
                                             asio::placeholders::bytes_transferred));
    }

    ClassInfo tcp_connection::GetClassInfo(const string &cls)
    {
		TRACE;
        return _classes[cls];
    }

    boost::shared_ptr<ObjectHandle> tcp_connection::GetObject(ObjectID_t oid)
    {
		TRACE;
        return _refobjects[oid];
    }

    ObjectID_t tcp_connection::AddObject(boost::shared_ptr<ObjectHandle> obj)
    {
		TRACE;
        ObjectID_t newid=++_objectid;
        _refobjects[newid]=obj;
        return newid;
    }

    void tcp_connection::RemoveObject(ObjectID_t oid)
    {
		TRACE;
        _refobjects.erase(oid);
    }
}

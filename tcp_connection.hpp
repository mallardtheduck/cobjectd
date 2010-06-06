#ifndef TCP_CONNECTION_HPP_INCLUDED
#define TCP_CONNECTION_HPP_INCLUDED

#include <boost/asio.hpp>
#include <boost/bind.hpp>
#include <boost/shared_ptr.hpp>
#include <boost/enable_shared_from_this.hpp>

#include "serialize.hpp"
#include "messages.hpp"
#include "objecthandle.hpp"

using namespace std;
using namespace boost;
using namespace boost::asio;
using namespace boost::asio::ip;

class ObjectHandle;

class tcp_connection : public enable_shared_from_this<tcp_connection>
{
public:
    typedef shared_ptr<tcp_connection> pointer;

    static pointer create(io_service &io_service);

    tcp::socket& socket();
    void start();
    void end_connection();

    ~tcp_connection();

    string nsName;

    friend void HandleMessage(MessageID_t msgID, shared_ptr<tcp_connection> conn);
    void send(const string &s);
    ClassInfo GetClassInfo(const string &cls);
    shared_ptr<ObjectHandle> GetObject(ObjectID_t oid);
    ObjectID_t AddObject(shared_ptr<ObjectHandle> obj);
    void RemoveObject(ObjectID_t oid);
    /*shared_ptr<tcp_connection> GetOwner(ObjectID_t oid);
    ObjectID_t GetOwnerID(ObjectID_t oid);*/

private:
    tcp_connection(io_service& io_service) : _socket(io_service), _endconn(false) {}
    void handle_write(const system::error_code&, size_t) {}
    void handle_read(const system::error_code &ec, size_t s);

    tcp::socket _socket;
    std::string _message;
    MessageID_t _msgID;
    ObjectID_t _objectid;
    bool _endconn;
    map<string, ClassInfo> _classes;
    map<ObjectID_t, shared_ptr<ObjectHandle> > _refobjects;
};

#endif // TCP_CONNECTION_HPP_INCLUDED

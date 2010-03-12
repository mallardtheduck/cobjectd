#include "tcp_connection.hpp"

using namespace std;
using namespace boost;
using namespace boost::asio;
using namespace boost::asio::ip;

tcp_connection::pointer tcp_connection::create(io_service &io_service)
{
    return pointer(new tcp_connection(io_service));
}

tcp::socket& tcp_connection::socket()
{
    return _socket;
}

void tcp_connection::start()
{
    if(!_socket.is_open()) return;
    _message = "Hello world!";
    async_write(_socket, buffer(_message),
                bind(&tcp_connection::handle_write, shared_from_this(),
                     placeholders::error,
                     placeholders::bytes_transferred));
    async_read(_socket, buffer(_buf), bind(&tcp_connection::handle_read, shared_from_this(), placeholders::error,
                        placeholders::bytes_transferred));
}

void tcp_connection::handle_read(const system::error_code &ec, size_t s){
    //cout << _buf[0];
    if(s!=_buf.size()) cout << "Only " << s << " of " << _buf.size() << " bytes recieved!" << endl;
    if(!ec) async_read(_socket, buffer(_buf), bind(&tcp_connection::handle_read, shared_from_this(),
                        placeholders::error, placeholders::bytes_transferred));
}

tcp_connection::~tcp_connection()
{
    cout << "Connection from " << _socket.remote_endpoint().address().to_string();
    cout << ":" << _socket.remote_endpoint().port();
    cout << " (ID:" << this << ")";
    cout << " terminated." << endl;
}

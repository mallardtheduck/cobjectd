#ifndef TCP_CONNECTION_HPP_INCLUDED
#define TCP_CONNECTION_HPP_INCLUDED

#include <boost/asio.hpp>
#include <boost/bind.hpp>
#include <boost/shared_ptr.hpp>
#include <boost/enable_shared_from_this.hpp>

using namespace std;
using namespace boost;
using namespace boost::asio;
using namespace boost::asio::ip;

class tcp_connection : public enable_shared_from_this<tcp_connection>
{
public:
    typedef shared_ptr<tcp_connection> pointer;

    static pointer create(io_service &io_service);

    tcp::socket& socket();
    void start();

    ~tcp_connection();

private:
    tcp_connection(io_service& io_service) : _socket(io_service) {}
    void handle_write(const system::error_code&, size_t) {}
    void handle_read(const system::error_code &ec, size_t s);

    tcp::socket _socket;
    std::string _message;
    array<char, 1024> _buf;
};

#endif // TCP_CONNECTION_HPP_INCLUDED

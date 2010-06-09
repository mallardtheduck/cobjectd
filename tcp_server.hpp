#ifndef TCP_SERVER_HPP_INCLUDED
#define TCP_SERVER_HPP_INCLUDED

#include <boost/asio.hpp>
#include <boost/bind.hpp>
#include <boost/shared_ptr.hpp>
#include <boost/enable_shared_from_this.hpp>

#include "tcp_connection.hpp"

using namespace std;
using namespace boost;
using namespace boost::asio;
using namespace boost::asio::ip;

namespace cobject
{

    /*!
        Server class, handles new incoming connections.
        This class "owns" an incoming socket and creates a new tcp_connection for each connection
    */
    class tcp_server
    {
    public:
        /*!
            Constructor
            \param io_service   An asio io_service
        */
        tcp_server(io_service& io_service);

    private:
        void start_accept();

        void handle_accept(tcp_connection::pointer new_connection, const system::error_code &error);
        tcp::acceptor acceptor_;
    };

}
#endif // TCP_SERVER_HPP_INCLUDED

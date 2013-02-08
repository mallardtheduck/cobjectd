#include "tcp_server.hpp"
#include "trace.hpp"

using namespace std;
using namespace boost;
using namespace boost::asio;
using namespace boost::asio::ip;

namespace cobject
{

    tcp_server::tcp_server(io_service &io_service) : acceptor_(io_service, tcp::endpoint(tcp::v4(), 3300))
    {
		TRACE;
        start_accept();
    }

    void tcp_server::start_accept()
    {
		TRACE;
        tcp_connection::pointer new_connection = tcp_connection::create(acceptor_.get_io_service());

        acceptor_.async_accept(new_connection->socket(), bind(&tcp_server::handle_accept, this, new_connection,
                               asio::placeholders::error));

    }

    void tcp_server::handle_accept(tcp_connection::pointer new_connection, const boost::system::error_code &error)
    {
		TRACE;
        cout << "New connection from: " << new_connection->socket().remote_endpoint().address().to_string();
        cout << ":" << new_connection->socket().remote_endpoint().port();
        cout << " (ID:" << new_connection.get() << ")." << endl;

        if (!error)
        {
            new_connection->start();
            start_accept();
        }
    }
}

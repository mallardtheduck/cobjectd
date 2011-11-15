#include <iostream>
#include <string>
//#include <meta.hpp>

#include "tcp_connection.hpp"
#include "tcp_server.hpp"

#define VERSION "DEV"

using namespace cobject;

int main()
{
    cout << "CrossObject Broker version: " << VERSION << endl;
    try
    {
        boost::asio::io_service io_service;
        tcp_server server(io_service);
        io_service.run();
    }
    catch (std::exception& e)
    {
        std::cerr << e.what() << std::endl;
    }

    return 0;
}

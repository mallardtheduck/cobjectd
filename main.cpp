#include <iostream>
#include <string>
//#include <meta.hpp>

#include "trace.hpp"
#include "tcp_connection.hpp"
#include "tcp_server.hpp"

#define VERSION "DEV"

using namespace cobject;
int TraceToggleCount=0;

int main()
{
	TRACE;
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

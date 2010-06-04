#ifndef SOCKETSTREAM_HPP_INCLUDED
#define SOCKETSTREAM_HPP_INCLUDED

#include <iostream>
#include <boost/iostreams/categories.hpp>
#include <boost/iostreams/stream.hpp>
#include <boost/asio.hpp>

using namespace std;
using namespace boost;
using namespace boost::asio;
using namespace boost::asio::ip;
using namespace boost::iostreams;

class socket_device
{
public:
    typedef char char_type;
    typedef bidirectional_device_tag category;

    socket_device(tcp::socket *socket);
    streamsize read(char *s, streamsize n);
    streamsize write(const char *s, streamsize n);
private:
    tcp::socket *_socket;
};

class socketstream : public stream<socket_device>
{
public:
    socketstream(tcp::socket *socket);
    socketstream(tcp::socket &socket);
};

#endif // SOCKETSTREAM_HPP_INCLUDED

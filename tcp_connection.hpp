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

namespace cobject
{

    class ObjectHandle;

    //! Class representing application connection
    /*!
        This class holds all information and proccesses all requests from an application. One instance of this
        class exists per application connection. It is the server-side equivelent of the client's Connection
        class.
    */
    class tcp_connection : public enable_shared_from_this<tcp_connection>
    {
    public:
        typedef shared_ptr<tcp_connection> pointer;     //!< Typdef for pointer to this class

        /*!
            Construction function
            \param io_service   An asio io_service
        */
        static pointer create(io_service &io_service);

        /*!
            Return socket used by this connection
            \return The socket
        */
        tcp::socket& socket();
        /*!
            Called on connection start
        */
        void start();
        /*!
            End the connection
        */
        void end_connection();

        /*!
            Destructor
        */
        ~tcp_connection();

        string nsName;      //!< Namespace used by this connection

        //! HandleMessage may access private elements of this class
        friend void HandleMessage(MessageID_t msgID, shared_ptr<tcp_connection> conn);

        /*!
            Asynchrously send some data using this connection's socket
            \param s    The data to send
        */
        void send(const string &s);
        /*!
            Get information about a class registered by this connection
            \param cls  The class name
        */
        ClassInfo GetClassInfo(const string &cls);
        /*!
            Get the handle to an object used by this connection
            \param oid  The ObjectID
            \return shared_ptr to the ObjectHandle
        */
        shared_ptr<ObjectHandle> GetObject(ObjectID_t oid);
        /*!
            Add an object to this connection's list of used objects
            \param obj  The handle of the object to add
            \return This connection's ObjectID for the added object
        */
        ObjectID_t AddObject(shared_ptr<ObjectHandle> obj);
        /*!
            Remove an object from this connection's list of used objects
            \param oid  The ObjectID
        */
        void RemoveObject(ObjectID_t oid);

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

}

#endif // TCP_CONNECTION_HPP_INCLUDED

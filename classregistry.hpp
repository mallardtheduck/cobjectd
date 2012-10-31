#ifndef CLASSREGISTRY_HPP_INCLUDED
#define CLASSREGISTRY_HPP_INCLUDED

#include "tcp_connection.hpp"

#include <vector>
#include <map>
#include <string>

#include <boost/shared_ptr.hpp>
#include <boost/weak_ptr.hpp>

using namespace std;
using namespace boost;

namespace cobject
{

    //! Structure to hold details of a call in progress
    struct CallDetails
    {
        boost::weak_ptr<tcp_connection> connection;    //!< Connection that the call came from
        CallID_t appcallid;                     //!< CallID for the call from that connection
    };

    /*!
        A centralized class to co-ordinate interactions between connections.
    */
    class ClassRegistry
    {
    public:
        /*!
            Set a connection's namespace
            \param conn     The connection
            \param ns       The requested namespace
            \return The actual set namespace
        */
        string SetNamespace(boost::shared_ptr<tcp_connection> conn, const string &ns);
        /*!
            Notify this class that a connection has ended, remove references to it.
            \param conn     The connection
        */
        void NotifyDisconnect(tcp_connection *conn);
        /*!
            Get a list of namespaces
            \return A vector of the registered namespaces
        */
        vector<string> ListNamespaces();
        /*!
            Get a connection from a namespace
            \param ns   The namespace
            \return The connection
        */
        boost::shared_ptr<tcp_connection> GetConnection(const string &ns);
        /*!
            Set up a new call (and get an ID)
            \param conn     The connection that the call is from
            \param acallid  The original call id
            \return A new CallID to refer to the call
        */
        CallID_t NewCallID(boost::shared_ptr<tcp_connection> conn, CallID_t acallid);
        /*!
            Get the call details from a CallID
            \param callid   The CallID
            \return The call details
        */
        CallDetails GetCallDetails(CallID_t callid);
    private:
        map<string, boost::weak_ptr<tcp_connection> > _nsmap;
        CallID_t _callid;
        map<CallID_t, CallDetails> calls;
    };

    /*!
        Get the global ClassRegistry
        \return The ClassRegistry
    */
    ClassRegistry &GetClassRegistry();

}
#endif // CLASSREGISTRY_HPP_INCLUDED

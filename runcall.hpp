#ifndef RUNCALL_HPP_INCLUDED
#define RUNCALL_HPP_INCLUDED

#include "tcp_connection.hpp"
#include "classregistry.hpp"

using namespace boost;

namespace cobject
{
    /*!
        Handle an object construction request
        \param conn     The connection that the request comes from
        \param ns       The namespace to construct an object from
        \param cls      The name of the class to construct
        \param callid   The CallID of the construction request
        \param args     The construction arguements
    */
    void RunConstruct(boost::shared_ptr<tcp_connection> conn, const string &ns, const string &cls, CallID_t callid, vector<TypedVal> args);
    /*!
        Handle a static call request
        \param conn     The connection that the request comes from
        \param ns       The namespace to construct an object from
        \param cls      The name of the class to construct
        \param method   The name of the method to call
        \param callid   The CallID of the construction request
        \param args     The call arguements
    */
    void RunCallStatic(boost::shared_ptr<tcp_connection> conn, const string &ns, const string &cls, const string &method, CallID_t callid, vector<TypedVal> args);
    /*!
        Handle a method call request
        \param conn     The connection that the request comes from
        \param objectid The ID of the object to call against
        \param method   The name of the method to call
        \param callid   The CallID of the construction request
        \param args     The call arguements
    */
    void RunCallMethod(boost::shared_ptr<tcp_connection> conn, ObjectID_t objectid, const string &method, CallID_t callid, vector<TypedVal> args);

    /*!
        Handle the reply to a construction request
        \param cd       The CallDetails of the construction request
        \param oid      The ObjectID assigned to the new object
        \param from     The connection that has just constructed the object
    */
    void RunConstructReply(CallDetails cd, ObjectID_t oid, boost::shared_ptr<tcp_connection> from);
    /*!
        Handle the reply to a static call request
        \param cd       The CallDetails of the static call request
        \param ret      The return value from the call
    */
    void RunCallStaticReply(CallDetails cd, TypedVal ret);
    /*!
        Handle the reply to a method call request
        \param cd       The CallDetails of the method call request
        \param ret      The return value from the call
    */
    void RunCallMethodReply(CallDetails cd, TypedVal ret);
}
#endif // RUNCALL_HPP_INCLUDED

#ifndef OBJECTHANDLE_HPP_INCLUDED
#define OBJECTHANDLE_HPP_INCLUDED

#include <boost/shared_ptr.hpp>
#include <cobject-client/serialize.hpp>
#include <cobject-client/messages.hpp>

#include "tcp_connection.hpp"

using namespace boost;

namespace cobject
{
    class tcp_connection;

    /*!
        An object handle
        Class holds information for contacting a specific object. Only one instance exists per object, shared
        between connections that reference it.
    */
    struct ObjectHandle
    {
        /*!
            Constructor
            \param conn     The connection owning the object
            \param oid      The ObjectID assigned by the owner
            \param meths    The object's methods
        */
        ObjectHandle(boost::shared_ptr<tcp_connection> conn, ObjectID_t oid, vector<MethodInfo> meths) :
                owner(conn), ownerid(oid), methods(meths) {}
        //! Destructor
        ~ObjectHandle();

        boost::weak_ptr<tcp_connection> owner;     //!< The owning connection
        ObjectID_t ownerid;                 //!< The owner-assigned ObjectID
        vector<MethodInfo> methods;         //!< The object's methods
    };
}
#endif // OBJECTHANDLE_HPP_INCLUDED

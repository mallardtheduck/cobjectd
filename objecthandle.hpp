#ifndef OBJECTHANDLE_HPP_INCLUDED
#define OBJECTHANDLE_HPP_INCLUDED

#include <boost/shared_ptr.hpp>
#include "tcp_connection.hpp"

using namespace boost;

class tcp_connection;

struct ObjectHandle{
    ObjectHandle(shared_ptr<tcp_connection> conn, ObjectID_t oid, vector<MethodInfo> meths) :
        owner(conn), ownerid(oid), methods(meths) {}
    ~ObjectHandle();
    weak_ptr<tcp_connection> owner;
    ObjectID_t ownerid;
    vector<MethodInfo> methods;
};

#endif // OBJECTHANDLE_HPP_INCLUDED

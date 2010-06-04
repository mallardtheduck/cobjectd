#ifndef RUNCALL_HPP_INCLUDED
#define RUNCALL_HPP_INCLUDED

#include "tcp_connection.hpp"

using namespace boost;

void RunConstruct(shared_ptr<tcp_connection> conn, const string &ns, const string &cls, CallID_t callid, vector<TypedVal> args);
void RunCallStatic(shared_ptr<tcp_connection> conn, const string &ns, const string &cls, const string &method, CallID_t callid, vector<TypedVal> args);
void RunCallMethod(shared_ptr<tcp_connection> conn, ObjectID_t objectid, const string &method, CallID_t callid, vector<TypedVal> args);

#endif // RUNCALL_HPP_INCLUDED

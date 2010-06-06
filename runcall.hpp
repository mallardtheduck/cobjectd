#ifndef RUNCALL_HPP_INCLUDED
#define RUNCALL_HPP_INCLUDED

#include "tcp_connection.hpp"
#include "classregistry.hpp"

using namespace boost;

void RunConstruct(shared_ptr<tcp_connection> conn, const string &ns, const string &cls, CallID_t callid, vector<TypedVal> args);
void RunCallStatic(shared_ptr<tcp_connection> conn, const string &ns, const string &cls, const string &method, CallID_t callid, vector<TypedVal> args);
void RunCallMethod(shared_ptr<tcp_connection> conn, ObjectID_t objectid, const string &method, CallID_t callid, vector<TypedVal> args);

void RunConstructReply(CallDetails cd, ObjectID_t oid, shared_ptr<tcp_connection> from);
void RunCallStaticReply(CallDetails cd, TypedVal ret);
void RunCallMethodReply(CallDetails cd, TypedVal ret);

#endif // RUNCALL_HPP_INCLUDED

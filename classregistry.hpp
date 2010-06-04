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

struct CallDetails{
    weak_ptr<tcp_connection> connection;
    CallID_t appcallid;
};

class ClassRegistry{
    public:
        string SetNamespace(shared_ptr<tcp_connection> conn, const string &ns);
        void NotifyDisconnect(tcp_connection *conn);
        vector<string> ListNamespaces();
        shared_ptr<tcp_connection> GetConnection(const string &ns);
        CallID_t NewCallID(shared_ptr<tcp_connection> conn, CallID_t acallid);
        CallDetails GetCallDetails(CallID_t callid);
    private:
        map<string, weak_ptr<tcp_connection> > _nsmap;
        CallID_t _callid;
        map<CallID_t, CallDetails> calls;
};

ClassRegistry &GetClassRegistry();

#endif // CLASSREGISTRY_HPP_INCLUDED

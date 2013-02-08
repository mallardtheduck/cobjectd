#include "classregistry.hpp"
#include "foreach.hpp"
#include "trace.hpp"

#include <sstream>

using namespace std;
using namespace boost;

namespace cobject
{

    ClassRegistry &GetClassRegistry()
    {
		TRACE;
        static ClassRegistry theReg;
        return theReg;
    }

    string ClassRegistry::SetNamespace(boost::shared_ptr<tcp_connection> conn, const string &ns)
    {
		TRACE;
        boost::weak_ptr<tcp_connection> wconn(conn);
        string tns=ns;
        if (_nsmap.find(ns)!=_nsmap.end())
        {
            stringstream s;
            s << ns << "_" << conn.get();
            tns=s.str();
        }
        _nsmap.insert(pair<string, boost::weak_ptr<tcp_connection> >(tns, wconn));
        return tns;
    }

    void ClassRegistry::NotifyDisconnect(tcp_connection *conn)
    {
		TRACE;
        vector<string> todelete;
        foreach(Q(pair<string, boost::weak_ptr<tcp_connection> >) p, _nsmap)
        {
            boost::shared_ptr<tcp_connection> sp=p.second.lock();
            if (!sp || sp.get()==conn)
            {
                todelete.push_back(p.first);
            }
        }
        foreach(string &s, todelete)
        {
            _nsmap.erase(s);
        }
    }

    vector<string> ClassRegistry::ListNamespaces()
    {
		TRACE;
        vector<string> ret;
        foreach(Q(pair<string, boost::weak_ptr<tcp_connection> >) p, _nsmap)
        {
            ret.push_back(p.first);
        }
        return ret;
    }

    boost::shared_ptr<tcp_connection> ClassRegistry::GetConnection(const string &ns)
    {
		TRACE;
        return _nsmap[ns].lock();
    }

    CallID_t ClassRegistry::NewCallID(boost::shared_ptr<tcp_connection> conn, CallID_t acallid)
    {
		TRACE;
        CallID_t newcallid=++_callid;
        CallDetails d;
        d.appcallid=acallid;
        d.connection=conn;
        calls[newcallid]=d;
        return newcallid;
    }

    CallDetails ClassRegistry::GetCallDetails(CallID_t cid)
    {
		TRACE;
        return calls[cid];
    }
}

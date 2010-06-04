#include "handlemessage.hpp"
#include "socketstream.hpp"
#include "serialize.hpp"
#include "classregistry.hpp"
#include "foreach.hpp"
#include "runcall.hpp"

using namespace std;

void HandleMessage(MessageID_t msgID, shared_ptr<tcp_connection> conn){
    socketstream s(conn->socket());

    if(msgID==Messages::BrokerDetails){
        Serialize(s, Messages::BrokerDetails);
        Serialize(s, CurrentBrokerDetails.ProtocolVersion);
        Serialize(s, CurrentBrokerDetails.BrokerName);
        Serialize(s, CurrentBrokerDetails.Vendor);
        Serialize(s, CurrentBrokerDetails.DisplayVersion);
    }

    if(msgID==Messages::ListNamespaces){
        vector<string> ret=GetClassRegistry().ListNamespaces();
        Serialize(s, Messages::ListNamespaces);
        Serialize(s, ret);
    }

    if(msgID==Messages::ListClasses){
        vector<string> ret;
        string ns;
        Deserialize(s, ns);
        shared_ptr<tcp_connection> ptr=GetClassRegistry().GetConnection(ns);
        foreach(Q(pair<string, ClassInfo>) p, ptr->_classes){
            ret.push_back(p.first);
        }
        Serialize(s, Messages::ListClasses);
        Serialize(s, ns);
        Serialize(s, ret);
    }

    if(msgID==Messages::SetNamespace){
        string ns;
        Deserialize(s, ns);
        ns=GetClassRegistry().SetNamespace(conn, ns);
        conn->nsName=ns;
        Serialize(s, Messages::SetNamespace);
        Serialize(s, ns);
    }

    if(msgID==Messages::RegisterClass){
        ClassInfo info;
        Deserialize(s, info);
        conn->_classes[info.ClassName]=info;
        Serialize(s, Messages::RegisterClass);
        Serialize(s, info.ClassName);
    }

    if(msgID==Messages::GetClassDef){
        string ns, cls;
        Deserialize(s, ns);
        Deserialize(s, cls);
        shared_ptr<tcp_connection> ptr=GetClassRegistry().GetConnection(ns);
        ClassInfo info=ptr->_classes[cls];
        Serialize(s, Messages::GetClassDef);
        Serialize(s, ns);
        Serialize(s, info.ClassName);
        Serialize(s, info);
    }

    if(msgID==Messages::ConstructObject){
        string ns, cls;
        CallID_t callid;
        vector<TypedVal> args;
        Deserialize(s, ns);
        Deserialize(s, cls);
        Deserialize(s, callid);
        Deserialize(s, args);
        RunConstruct(conn, ns, cls, callid, args);
    }

    if(msgID==Messages::CallStatic){
        string ns, cls, method;
        CallID_t callid;
        vector<TypedVal> args;
        Deserialize(s, ns);
        Deserialize(s, cls);
        Deserialize(s, method);
        Deserialize(s, callid);
        Deserialize(s, args);
        RunCallStatic(conn, ns, cls, method, callid, args);
    }

    if(msgID==Messages::CallMethod){
        ObjectID_t objectid;
        string method;
        CallID_t callid;
        vector<TypedVal> args;
        Deserialize(s, objectid);
        Deserialize(s, method);
        Deserialize(s, callid);
        Deserialize(s, args);
        RunCallMethod(conn, objectid, method, callid, args);
    }
}

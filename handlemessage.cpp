#include "handlemessage.hpp"
#include "socketstream.hpp"
#include "classregistry.hpp"
#include "foreach.hpp"
#include "runcall.hpp"
#include "trace.hpp"

using namespace std;

namespace cobject
{

    void HandleMessage(MessageID_t msgID, boost::shared_ptr<tcp_connection> conn)
    {
		TRACE;
        socketstream s(conn->socket());
        try
        {

            if (msgID==Messages::BrokerDetails)
            {
                Serialize(s, Messages::BrokerDetails);
                Serialize(s, CurrentBrokerDetails.ProtocolVersion);
                Serialize(s, CurrentBrokerDetails.BrokerName);
                Serialize(s, CurrentBrokerDetails.Vendor);
                Serialize(s, CurrentBrokerDetails.DisplayVersion);
            }

            if (msgID==Messages::ListNamespaces)
            {
                vector<string> ret=GetClassRegistry().ListNamespaces();
                Serialize(s, Messages::ListNamespaces);
                Serialize(s, ret);
            }

            if (msgID==Messages::ListClasses)
            {
                vector<string> ret;
                string ns;
                Deserialize(s, ns);
                boost::shared_ptr<tcp_connection> ptr=GetClassRegistry().GetConnection(ns);
                foreach(Q(pair<string, ClassInfo>) p, ptr->_classes)
                {
                    ret.push_back(p.first);
                }
                Serialize(s, Messages::ListClasses);
                Serialize(s, ns);
                Serialize(s, ret);
            }

            if (msgID==Messages::SetNamespace)
            {
                string ns;
                Deserialize(s, ns);
                ns=GetClassRegistry().SetNamespace(conn, ns);
                conn->nsName=ns;
                Serialize(s, Messages::SetNamespace);
                Serialize(s, ns);
            }

            if (msgID==Messages::RegisterClass)
            {
                ClassInfo info;
                Deserialize(s, info);
                conn->_classes[info.ClassName]=info;
                Serialize(s, Messages::RegisterClass);
                Serialize(s, info.ClassName);
            }

            if (msgID==Messages::GetClassDef)
            {
                string ns, cls;
                Deserialize(s, ns);
                Deserialize(s, cls);
                boost::shared_ptr<tcp_connection> ptr=GetClassRegistry().GetConnection(ns);
                ClassInfo info=ptr->_classes[cls];
                Serialize(s, Messages::GetClassDef);
                Serialize(s, ns);
                Serialize(s, info.ClassName);
                Serialize(s, info);
            }

            if (msgID==Messages::GetObjectDef)
            {
                ObjectID_t oid;
                Deserialize(s, oid);
                boost::shared_ptr<ObjectHandle> objh=conn->GetObject(oid);
                Serialize(s, Messages::GetObjectDef);
                Serialize(s, oid);
                Serialize(s, objh->methods);
            }

            if (msgID==Messages::RegisterObject)
            {
                ObjectID_t aoid;
                vector<MethodInfo> info;
                Deserialize(s, aoid);
                Deserialize(s, info);
                boost::shared_ptr<ObjectHandle> objh(new ObjectHandle(conn, aoid, info));
                ObjectID_t oid=conn->AddObject(objh);
                Serialize(s, Messages::RegisterObject);
                Serialize(s, aoid);
                Serialize(s, oid);
            }

            if (msgID==Messages::ConstructObject)
            {
                string ns, cls;
                CallID_t callid;
                vector<TypedVal> args;
                Deserialize(s, ns);
                Deserialize(s, cls);
                Deserialize(s, callid);
                Deserialize(s, args);
                RunConstruct(conn, ns, cls, callid, args);
            }

            if (msgID==Messages::CallStatic)
            {
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

            if (msgID==Messages::CallMethod)
            {
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

            if (msgID==Messages::ReleaseObject)
            {
                ObjectID_t oid;
                Deserialize(s, oid);
                conn->RemoveObject(oid);
                Serialize(s, Messages::ReleaseObject);
                Serialize(s, oid);
                Serialize(s, StatusCodes::Success);
            }

            if (msgID==Messages::Construct)
            {
                CallID_t cid;
                Deserialize(s, cid);
                StatusCode_t status;
                Deserialize(s, status);
                ObjectID_t oid;
                Deserialize(s, oid);
                CallDetails cd=GetClassRegistry().GetCallDetails(cid);
                RunConstructReply(cd, oid, conn);
            }

            if (msgID==Messages::StaticCall)
            {
                CallID_t cid;
                Deserialize(s, cid);
                StatusCode_t status;
                Deserialize(s, status);
                TypedVal tv;
                Deserialize(s, tv);
                CallDetails cd=GetClassRegistry().GetCallDetails(cid);
                RunCallStaticReply(cd, tv);
            }

            if (msgID==Messages::MethodCall)
            {
                CallID_t cid;
                Deserialize(s, cid);
                StatusCode_t status;
                Deserialize(s, status);
                TypedVal tv;
                Deserialize(s, tv);
                CallDetails cd=GetClassRegistry().GetCallDetails(cid);
                RunCallMethodReply(cd, tv);
            }

        }
        catch (...)
        {
			TCOM("Connection error!");
			GetClassRegistry().NotifyDisconnect(conn.get());
            conn->end_connection();
        }
    }
}

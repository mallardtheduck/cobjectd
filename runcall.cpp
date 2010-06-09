#include "runcall.hpp"
#include "classregistry.hpp"

#include <stdexcept>

using namespace boost;

namespace cobject
{

    bool CheckArgs(shared_ptr<tcp_connection> conn, shared_ptr<tcp_connection> toconn, MethodInfo mi, vector<TypedVal> &args)
    {
        if (mi.ParamTypes.size()!=args.size()) return false;
        for (size_t i=0; i<mi.ParamTypes.size(); ++i)
        {
            if (mi.ParamTypes[i]!=args[i].type) return false;
            if (args[i].type==Types::Object)
            {
                ObjectID_t oid=any_cast<ObjectID_t>(args[i].val);
                ObjectID_t noid=toconn->AddObject(conn->GetObject(oid));
                args[i].val=noid;
            }
        }
        return true;
    }

    MethodInfo GetMethodInfo(shared_ptr<tcp_connection> conn, const string &cls, const string &method)
    {
        ClassInfo clsi=conn->GetClassInfo(cls);
        for (size_t i=0; i<clsi.Methods.size(); ++i)
        {
            if (clsi.Methods[i].MethodName==method) return clsi.Methods[i];
        }
        if (method=="ctor")
        {
            MethodInfo mi;
            mi.MethodName="ctor";
            mi.ReturnType=Types::Void;
            return mi;
        }
        throw std::exception(); //ERROR!
    }

    void RunConstruct(shared_ptr<tcp_connection> conn, const string &ns, const string &cls, CallID_t callid, vector<TypedVal> args)
    {
        shared_ptr<tcp_connection> toconn=GetClassRegistry().GetConnection(ns);
        stringstream out;
        Serialize(out, Messages::Construct);
        Serialize(out, cls);
        CallID_t icallid=GetClassRegistry().NewCallID(conn, callid);
        Serialize(out, icallid);
        CheckArgs(conn, toconn, GetMethodInfo(conn, cls, "ctor"), args);
        Serialize(out, args);
        toconn->send(out.str());
    }

    void RunCallStatic(shared_ptr<tcp_connection> conn, const string &ns, const string &cls, const string &method, CallID_t callid, vector<TypedVal> args)
    {
        shared_ptr<tcp_connection> toconn=GetClassRegistry().GetConnection(ns);
        stringstream out;
        Serialize(out, Messages::StaticCall);
        Serialize(out, cls);
        Serialize(out, method);
        CallID_t icallid=GetClassRegistry().NewCallID(conn, callid);
        Serialize(out, icallid);
        Serialize(out, args);
        toconn->send(out.str());
    }

    void RunCallMethod(shared_ptr<tcp_connection> conn, ObjectID_t objectid, const string &method, CallID_t callid, vector<TypedVal> args)
    {
        shared_ptr<tcp_connection> toconn=conn->GetObject(objectid)->owner.lock();
        stringstream out;
        Serialize(out, Messages::MethodCall);
        Serialize(out, conn->GetObject(objectid)->ownerid);
        Serialize(out, method);
        CallID_t icallid=GetClassRegistry().NewCallID(conn, callid);
        Serialize(out, icallid);
        Serialize(out, args);
        toconn->send(out.str());
    }

    void RunConstructReply(CallDetails cd, ObjectID_t oid, shared_ptr<tcp_connection> from)
    {
        stringstream out;
        shared_ptr<ObjectHandle> handle(new ObjectHandle(from, oid, vector<MethodInfo>()));
        shared_ptr<tcp_connection> to=cd.connection.lock();
        ObjectID_t newid=to->AddObject(handle);
        Serialize(out, Messages::ConstructObject);
        Serialize(out, cd.appcallid);
        Serialize(out, StatusCodes::Success);
        Serialize(out, newid);
        to->send(out.str());
    }

    void RunCallStaticReply(CallDetails cd, TypedVal ret)
    {
        stringstream out;
        Serialize(out, Messages::CallStatic);
        Serialize(out, cd.appcallid);
        Serialize(out, StatusCodes::Success);
        Serialize(out, ret);
        cd.connection.lock()->send(out.str());
    }

    void RunCallMethodReply(CallDetails cd, TypedVal ret)
    {
        stringstream out;
        Serialize(out, Messages::CallMethod);
        Serialize(out, cd.appcallid);
        Serialize(out, StatusCodes::Success);
        Serialize(out, ret);
        cd.connection.lock()->send(out.str());
    }
}

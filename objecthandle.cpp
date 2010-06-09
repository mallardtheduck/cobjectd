#include "objecthandle.hpp"

#include "serialize.hpp"
#include "messages.hpp"

#include <sstream>

using namespace std;

namespace cobject
{

    ObjectHandle::~ObjectHandle()
    {
        stringstream s;
        Serialize(s, Messages::FreeObject);
        Serialize(s, ownerid);
        shared_ptr<tcp_connection> conn;
        if(conn=owner.lock())conn->send(s.str());
    }
}

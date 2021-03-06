#include "objecthandle.hpp"
#include "trace.hpp"

#include <sstream>

using namespace std;

namespace cobject
{

    ObjectHandle::~ObjectHandle()
    {
		TRACE;
        stringstream s;
        Serialize(s, Messages::FreeObject);
        Serialize(s, ownerid);
        boost::shared_ptr<tcp_connection> conn;
        if(conn=owner.lock())conn->send(s.str());
    }
}

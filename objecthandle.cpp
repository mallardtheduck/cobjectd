#include "objecthandle.hpp"

#include "serialize.hpp"
#include "messages.hpp"

#include <sstream>

using namespace std;

ObjectHandle::~ObjectHandle(){
    stringstream s;
    Serialize(s, Messages::FreeObject);
    Serialize(s, ownerid);
    owner->send(s.str());
}

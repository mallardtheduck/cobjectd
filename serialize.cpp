#include "serialize.hpp"

using namespace std;

void Serialize(ostream &s, const string &v){
    s << v.length();
    s << v;
}

void Deserialize(istream &s, string &v){
    size_t size;
    s >> size;
    v.reserve(size);
    for(size_t i=0; i<size; i++){
        char c;
        s >> c;
        v.push_back(c);
    }
}

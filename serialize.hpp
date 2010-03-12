#ifndef SERIALIZE_HPP_INCLUDED
#define SERIALIZE_HPP_INCLUDED

#include <iostream>
#include <vector>

using namespace std;

void Serialize(ostream &s, const string &v);

template<typename T> void Serialize(ostream &s, const vector<T> &v){
    s << v.length();
    for(size_t i=0; i<v.length(); i++){
        Serialize(s, v.at(i));
    }
}

template<typename T> void Serialize(ostream &s, const T &v){
    s << v;
}

void Deserialize(istream &s, string &v);

template<typename T> void Deserialize(istream &s, vector<T> &v){
    size_t size;
    s >> size;
    v.reserve(size);
    for(size_t i=0; i<size; i++){
        T x;
        Deserialize(s, x);
        v.push_back(x);
    }
}

template<typename T> void Deserialize(istream &s, T &v){
    s >> v;
}

#endif // SERIALIZE_HPP_INCLUDED

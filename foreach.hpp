#ifndef FOREACH_HPP_INCLUDED
#define FOREACH_HPP_INCLUDED

#include "boost/foreach.hpp"

#undef foreach
#undef Q

#define foreach BOOST_FOREACH

#define Q(...) __VA_ARGS__

#endif // FOREACH_HPP_INCLUDED

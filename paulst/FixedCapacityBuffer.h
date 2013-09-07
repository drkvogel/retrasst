#ifndef FIXEDCAPACITYBUFFERH
#define FIXEDCAPACITYBUFFERH

#include <string>

#include <boost/circular_buffer.hpp>

#include "CritSec.h"

namespace paulst
{

class FixedCapacityBuffer
{
public:
    FixedCapacityBuffer( unsigned int size );
    std::string copy() const;
    void push_back( char c );
    typedef char& reference;
	typedef const char& const_reference;
	typedef boost::circular_buffer<char>::value_type value_type;
private:
    boost::circular_buffer<char> m_buffer;
    CritSec m_cs;

    FixedCapacityBuffer( const FixedCapacityBuffer& );
    FixedCapacityBuffer& operator=( const FixedCapacityBuffer& );
};

};

#endif


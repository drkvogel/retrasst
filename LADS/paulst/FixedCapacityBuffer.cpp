#include "AcquireCriticalSection.h"
#include "FixedCapacityBuffer.h"

namespace paulst
{

FixedCapacityBuffer::FixedCapacityBuffer( unsigned int size )
    : m_buffer(size)
{
}

std::string FixedCapacityBuffer::copy() const
{
    AcquireCriticalSection a(m_cs);
    std::string cpy( m_buffer.begin(), m_buffer.end() );
    return cpy;
}

void FixedCapacityBuffer::push_back( char c )
{
    AcquireCriticalSection a(m_cs);
    m_buffer.push_back( c );
}

}


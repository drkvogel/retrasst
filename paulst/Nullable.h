#ifndef NULLABLEH
#define NULLABLEH

#include <SysUtils.hpp>

namespace paulst
{

template<typename T>
class Nullable
{
public:
    Nullable() 
        : 
        m_isNull(true)
    {
    }

    explicit Nullable( const T& t )
        :
        m_isNull(false),
        m_t( t )
    {
    }

    Nullable( const Nullable& other )
        :
        m_isNull( other.m_isNull ),
        m_t     ( other.m_t      )
    {
    }

    operator T() const
    {
        if ( m_isNull )
        {
            throw Exception( "Is Null" );
        }

        return m_t;
    }

    bool isNull() const
    {
        return m_isNull;
    }

    Nullable& operator=( const Nullable& other )
    {
        m_t      = other.m_t;
        m_isNull = other.m_isNull;
        return *this;
    }

    Nullable& operator=( const T& t )
    {
        m_t      = t;
        m_isNull = false;
        return *this;
    }

    bool operator==( const Nullable& other )
    {
        return 
            ( m_isNull && other.m_isNull )
            ||
            (
                ( ! ( m_isNull || other.m_isNull )  )
                &&
                ( m_t == other.m_t )
            );
    }

    bool operator!=( const Nullable& other )
    {
        return ! ( *this == other );
    }


    bool operator<( const Nullable& other )
    {
        return 
            ( m_isNull && ! (other.m_isNull) ) // Nulls sort low
            ||
            (
                ( ! ( m_isNull || other.m_isNull ) ) // Neither null
                &&
                ( m_t < other.m_t )
            );
    }

private:
    bool m_isNull;
    T    m_t;
};

}

#endif


#ifndef CURSORH
#define CURSORH

#include <string>
#include "StringBuilder.h"
#include <System.hpp>
#include <SysUtils.hpp>

namespace paulstdb
{
/*
    Abstract representation of a set of records.

    Note that the set of records may be empty.

    Unless the set of records is empty, a Cursor is initially
    positioned at the first record in the set.

    Use the 'read' methods to obtain values for this record.
    Use 'isNull' before using 'read' to check that there is a
    genuine value available for reading.

    Use 'next' to advance to the next record.

    Call 'close' to release resources.  However, note that,
    internally, 'close' is called by the destructor.
*/
class Cursor
{
public:
    Cursor();
    virtual                 ~Cursor();
    virtual operator bool() const { return ! endOfRecordSet(); }
    virtual void            close()              = 0;
    virtual bool            isNull   ( int col ) = 0;
    virtual bool            endOfRecordSet() const = 0;
    virtual void            next()               = 0;
    virtual void            read( int col, bool&        outVal ) = 0;
    virtual void            read( int col, char&        outVal ) = 0;
    virtual void            read( int col, float&       outVal ) = 0;
    virtual void            read( int col, double&      outVal ) = 0;
    virtual void            read( int col, int&         outVal ) = 0;
    virtual void            read( int col, std::string& outVal ) = 0;
    virtual void            read( int col, TDateTime&   outVal ) = 0;
private:
    Cursor( const Cursor& );
    Cursor& operator=( const Cursor& );
};

template<typename T>
T read( Cursor& c, int col, bool tolerateNull = false )
{
    if ( (!tolerateNull) && c.isNull( col ) )
    {
        std::string msg = std::string("Null value obtained for column ") << col << ".";
		throw Exception( UnicodeString( msg.c_str() ) );
    }

    T t;
    c.read( col, t );
    return t;
}

}
#endif



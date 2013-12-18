#include "DBQueryTask.h"

namespace valc
{

void releaseConnection( paulstdb::DBConnection* c )
{
    if ( c )
    {
        try
        {
            c->close();
        }
        catch( ... )
        {
        }
        delete c;
    }
}

void releaseCursor( paulstdb::Cursor* c )
{
    if ( c )
    {
        try
        {
            c->close();
        }
        catch( ... )
        {
        }
        delete c;
    }
}

}


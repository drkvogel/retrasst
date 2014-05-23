#include "DBQueryTask.h"

namespace valc
{


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


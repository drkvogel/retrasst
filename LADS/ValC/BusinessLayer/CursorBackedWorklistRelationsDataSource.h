#ifndef CURSORBACKEDWORKLISTRELATIONSDATASOURCEH
#define CURSORBACKEDWORKLISTRELATIONSDATASOURCEH

#include <boost/scoped_ptr.hpp>
#include "WorklistRelation.h"

namespace paulstdb
{
    class Cursor;
}

namespace valc
{

class CursorBackedWorklistRelationsDataSource : public WorklistRelationsDataSource
{
public:
    CursorBackedWorklistRelationsDataSource( paulstdb::Cursor* c ); 
    WorklistRelations getRelations( int worklistEntry );
private:
    boost::scoped_ptr<paulstdb::Cursor> m_cursor;

    int  getChildID();
    int  getIntValue( int forColumn );
    char getMotivation();
    int  getParentID();
    int  getRecordNo();
};

};

#endif


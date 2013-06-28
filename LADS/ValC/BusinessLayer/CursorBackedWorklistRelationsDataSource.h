#ifndef CURSORBACKEDWORKLISTRELATIONSDATASOURCEH
#define CURSORBACKEDWORKLISTRELATIONSDATASOURCEH

#include <boost/scoped_ptr.hpp>
#include "WorklistRelation.h"

namespace valc
{

class Cursor;

class CursorBackedWorklistRelationsDataSource : public WorklistRelationsDataSource
{
public:
    CursorBackedWorklistRelationsDataSource( Cursor* c ); 
    WorklistRelations getRelations( int worklistEntry );
private:
    boost::scoped_ptr<Cursor> m_cursor;

    int  getChildID();
    int  getIntValue( int forColumn );
    char getMotivation();
    int  getParentID();
    int  getRecordNo();
};

};

#endif


#include "Cursor.h"
#include "CursorBackedWorklistRelationsDataSource.h"

namespace valc
{

CursorBackedWorklistRelationsDataSource::CursorBackedWorklistRelationsDataSource( paulstdb::Cursor* c )
    : m_cursor( c )
{
}

int CursorBackedWorklistRelationsDataSource::getChildID()
{
    return getIntValue( 1 );
}

int CursorBackedWorklistRelationsDataSource::getIntValue( int forColumn )
{
    int val = 0;
    m_cursor->read( forColumn, val );
    return val;
}

char CursorBackedWorklistRelationsDataSource::getMotivation()
{
    char c = '\0';
    m_cursor->read( 3, c );
    return c;
}

int CursorBackedWorklistRelationsDataSource::getParentID()
{
    return getIntValue( 2 );
}

WorklistRelations CursorBackedWorklistRelationsDataSource::getRelations( int worklistEntry )
{
    WorklistRelations wr;
    
    for ( ; (*m_cursor) && ( getRecordNo() < worklistEntry ); m_cursor->next() );

    if ( (*m_cursor) && ( getRecordNo() == worklistEntry ) )
    {
        for ( ; (*m_cursor) && ( getRecordNo() == worklistEntry ); m_cursor->next() )
        {
            WorklistRelation r( getParentID(), getChildID(), getMotivation() );
            wr.push_back( r );
        }
    }
    
    return wr;
}

int CursorBackedWorklistRelationsDataSource::getRecordNo()
{
    int recordNo = getIntValue( 0 );
        
    return recordNo;
}

}


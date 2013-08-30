#include "API.h"
#include "CursorBackedWorklistRelationsDataSource.h"
#include "LoadWorklistEntries.h"
#include "LoggingService.h"
#include "WorklistEntries.h"
#include "WorklistEntryBuilder.h"

namespace valc
{

LoadWorklistEntries::LoadWorklistEntries( WorklistEntries* worklistEntries, DBConnection* con, 
    paulst::LoggingService* log, ResultIndex* resultIndex,
    const std::string& worklistSQL, const std::string& worklistRelationSQL, const std::string& inclusionRule )
    :
    m_worklistEntries( worklistEntries ),
    m_con( con ),
    m_log( log ),
    m_resultIndex( resultIndex ),
    m_worklistSQL( worklistSQL ),
    m_worklistRelationSQL( worklistRelationSQL ),
    m_inclusionRule( inclusionRule )
{
}


void LoadWorklistEntries::execute()
{
    LOG( m_worklistSQL );
    LOG( m_worklistRelationSQL );

    CursorBackedWorklistRelationsDataSource worklistRelations( m_con->executeQuery( m_worklistRelationSQL ) );

    WorklistEntryBuilder builder( m_worklistEntries, m_resultIndex, &worklistRelations, m_inclusionRule );

    for ( std::auto_ptr<Cursor> cursorWorklist( m_con->executeQuery(m_worklistSQL) ); 
            ! cursorWorklist->endOfRecordSet() && builder.accept(cursorWorklist.get()); cursorWorklist->next() )
    ;

   LOG( std::string("Loaded ") << m_worklistEntries->size() << " worklist entries." );
}

}


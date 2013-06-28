#include "API.h"
#include <boost/foreach.hpp>
#include "CursorBackedWorklistRelationsDataSource.h"
#include "LoadWorklistEntries.h"
#include "LoggingService.h"
#include "StringBuilder.h"
#include "WorklistEntries.h"
#include "WorklistEntryBuilder.h"

namespace valc
{

const std::string LoadWorklistEntries::SQL_WORKLIST_SELECT = 
        "SELECT wl.record_no, machine_cid, barcode, test_cid, group_id, category_id, sample_id, project_cid,"
        "profile_id, profile_name, time_stamp, ts_sequence, status, diluent, buddy_result_id ";
 
const std::string LoadWorklistEntries::SQL_WORKLIST_RELATION_SELECT = 
        "SELECT wl.record_no, wr.rerun_id, wr.parent_id, wr.motivation ";

const std::string LoadWorklistEntries::SQL_ORDER_BY = 
        " ORDER BY wl.record_no ";

LoadWorklistEntries::LoadWorklistEntries( const ClusterIDs* clusterIDs, WorklistEntries* worklistEntries, int localMachineID, DBConnection* con, 
    paulst::LoggingService* log, ResultIndex* resultIndex,
    const std::string& worklistSQL, const std::string& worklistRelationSQL )
    :
    m_worklistEntries( worklistEntries ),
    m_localMachineID( localMachineID ),
    m_con( con ),
    m_log( log ),
    m_resultIndex( resultIndex ),
    m_worklistSQL( worklistSQL ),
    m_worklistRelationSQL( worklistRelationSQL ),
    m_clusterIDs( clusterIDs )
{
}


std::string defineWorklistEntryQualification( int localMachineID, DBConnection* con, const ClusterIDs* clusterIDs )
{
    std::string worklistEntryQualification = std::string( " wl.machine_cid in ( " ) << localMachineID;

    BOOST_FOREACH( const int& clusterID, *clusterIDs )
    {
        worklistEntryQualification.append( std::string(", ") << clusterID );
    }

    worklistEntryQualification.append(") and wl.status in ('Q','P','C','S','W', 'X') ");

    return worklistEntryQualification;
}


void LoadWorklistEntries::execute()
{
    if ( m_worklistSQL.empty() || m_worklistRelationSQL.empty() )
    {
        const std::string worklistEntryQualification = defineWorklistEntryQualification( m_localMachineID, m_con, m_clusterIDs );

        m_worklistSQL = 
            SQL_WORKLIST_SELECT + 
            " FROM valc_worklist wl " 
            " WHERE " + worklistEntryQualification + 
            SQL_ORDER_BY;

        m_worklistRelationSQL = 
            SQL_WORKLIST_RELATION_SELECT + 
            " FROM valc_worklist wl, c_worklist_relation wr "
            " WHERE wl.record_no in ( wr.rerun_id, wr.parent_id ) "
            " and " + worklistEntryQualification +
            SQL_ORDER_BY;
    }

    LOG( m_worklistSQL );
    LOG( m_worklistRelationSQL );

    CursorBackedWorklistRelationsDataSource worklistRelations( m_con->executeQuery( m_worklistRelationSQL ) );

    WorklistEntryBuilder builder( m_worklistEntries, m_resultIndex, &worklistRelations );

    for ( std::auto_ptr<Cursor> cursorWorklist( m_con->executeQuery(m_worklistSQL) ); ! cursorWorklist->endOfRecordSet(); cursorWorklist->next() )
    {
        m_worklistEntries->add( builder.newInstance( cursorWorklist.get() ) );
    }

   LOG( std::string("Loaded ") << m_worklistEntries->size() << " worklist entries." );
}

int LoadWorklistEntries::releaseReturnValue()
{
    return 1;
}

}


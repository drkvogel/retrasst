#include <cstdio>
#include "DBConnection.h"
#include "DBUpdateTaskLinkResultToWorklistEntry.h"

namespace valc
{

DBUpdateTaskLinkResultToWorklistEntry::DBUpdateTaskLinkResultToWorklistEntry( int resultID, int worklistEntryID )
    :
    m_resultID( resultID ),
    m_worklistEntryID( worklistEntryID )
{
}

std::string DBUpdateTaskLinkResultToWorklistEntry::describeUpdate() const
{
    char buffer[1024];
    std::sprintf( buffer, "Updating buddy_result_float.cbw_record_no to %d where buddy_result_id = %d", m_worklistEntryID, m_resultID );
    return buffer;
}

void DBUpdateTaskLinkResultToWorklistEntry::updateDatabase()
{
    char sql[1024];
    std::sprintf( sql, "update buddy_result_float set cbw_record_no = %d where buddy_result_id = %d", m_worklistEntryID, m_resultID );
    getConnection()->executeStmt( sql );
}

}


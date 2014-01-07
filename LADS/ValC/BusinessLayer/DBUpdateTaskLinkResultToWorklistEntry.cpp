#include "Config.h"
#include "DBConnection.h"
#include "DBUpdateTaskLinkResultToWorklistEntry.h"
#include "StrUtil.h"


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
    std::string sql = paulst::format( config->get("LinkResultToWorklistEntryUpdateSQL").c_str(), m_worklistEntryID, m_resultID );
    connection->executeStmt( sql );
}

}


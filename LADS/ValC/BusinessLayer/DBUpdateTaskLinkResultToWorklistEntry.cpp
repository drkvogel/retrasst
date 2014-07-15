#include "Config.h"
#include "DBConnection.h"
#include "DBTransactionResources.h"
#include "DBUpdateTaskLinkResultToWorklistEntry.h"
#include "StrUtil.h"


namespace valc
{

DBUpdateTaskLinkResultToWorklistEntry::DBUpdateTaskLinkResultToWorklistEntry( 
    int resultID, 
    int worklistEntryID,
    const DBTransactionResources* dbtr )
    :
    m_resultID( resultID ),
    m_worklistEntryID( worklistEntryID ),
    m_dbTransactionResources( dbtr )
{
}

void DBUpdateTaskLinkResultToWorklistEntry::doStuff()
{
    std::string sql = paulst::format( 
                        m_dbTransactionResources->getConfig()->get("LinkResultToWorklistEntryUpdateSQL").c_str(), 
                        m_worklistEntryID, 
                        m_resultID );

    m_dbTransactionResources->getConnection()->executeStmt( sql );
}

}


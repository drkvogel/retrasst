#include "Config.h"
#include "DBConnection.h"
#include "DBUpdateTaskUpdateSampleRunID.h"
#include "SampleRunIDResolutionService.h"
#include "StrUtil.h"

namespace valc
{

DBUpdateTaskUpdateSampleRunID::DBUpdateTaskUpdateSampleRunID( const std::string& candidateNewSampleRunID, int buddySampleID )
    :
    m_candidateNewSampleRunID( candidateNewSampleRunID ),
    m_buddySampleID( buddySampleID )
{
}

std::string DBUpdateTaskUpdateSampleRunID::describeUpdate() const
{
    const int sampleRunID = snapshotUpdateHandle.getDatabaseIDForSampleRun( m_candidateNewSampleRunID );

    std::string desc = paulst::format( "Updating buddy_database.sample_run_id to %d where buddy_sample_id = %d ", sampleRunID, m_buddySampleID );
    
    return desc;
}

void DBUpdateTaskUpdateSampleRunID::updateDatabase()
{
    const int sampleRunID = snapshotUpdateHandle.getDatabaseIDForSampleRun( m_candidateNewSampleRunID );

    std::string sql = paulst::format( config->get("BuddyDatabaseSampleRunIDUpdateSQL").c_str(), sampleRunID, m_buddySampleID );

    connection->executeStmt( sql );
}

}


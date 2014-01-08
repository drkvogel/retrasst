#include "Config.h"
#include "Cursor.h"
#include "DBConnection.h"
#include "DBUpdateTaskInsertSampleRun.h"
#include <memory>
#include "SampleRunIDResolutionService.h"
#include "StrUtil.h"

namespace valc
{

DBUpdateTaskInsertSampleRun::DBUpdateTaskInsertSampleRun( const std::string& candidateNewSampleRunID, int sequencePosition )
    :
    m_candidateNewSampleRunID( candidateNewSampleRunID ),
    m_sequencePosition( sequencePosition )
{
}

std::string DBUpdateTaskInsertSampleRun::describeUpdate() const
{
    std::string desc;

    if ( snapshotUpdateHandle.knownDatabaseIDForCandidateSampleRun( m_candidateNewSampleRunID ) )
    {
        desc = paulst::format( "Insert sample-run. Nothing to do. '%s' maps to sample-run %d.", 
            m_candidateNewSampleRunID.c_str(),
            snapshotUpdateHandle.getDatabaseIDForSampleRun( m_candidateNewSampleRunID ) );
    }
    else
    {
        desc = paulst::format( "Insert sample-run. Temporary sample-run ID is '%s'.", m_candidateNewSampleRunID.c_str() );
    }

    return desc;
}

void DBUpdateTaskInsertSampleRun::updateDatabase()
{
    const bool insertRequired = ! ( snapshotUpdateHandle.knownDatabaseIDForCandidateSampleRun( m_candidateNewSampleRunID ) );

    if ( insertRequired  )
    {
        std::auto_ptr<paulstdb::Cursor> c( connection->executeQuery( config->get("SampleRunIDQueryString") ) );

        if ( c->endOfRecordSet() )
        {
            throw Exception( L"No rows obtained for: select sample_run_id.nextval" );
        }

        int newID = 0;
        c->read( 0, newID );
        c->close();

        std::string sql = paulst::format( config->get("SampleRunInsertSQL").c_str(), newID, m_sequencePosition, 
            snapshotUpdateHandle.getGroupIDForSampleRun( m_candidateNewSampleRunID ) );

        connection->executeStmt( sql );

        snapshotUpdateHandle.updateWithDatabaseIDForSampleRun( m_candidateNewSampleRunID, newID );
    }
}

}


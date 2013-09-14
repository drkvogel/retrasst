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
    std::string desc("Insert sample-run");

    if ( getSampleRunIDResolutionService()->hasMappingFor( m_candidateNewSampleRunID ) )
    {
        desc.append( ". Nothing to do. " );
        desc.append( m_candidateNewSampleRunID );
        desc.append( " maps to " );
        desc.append( getSampleRunIDResolutionService()->getMappingFor( m_candidateNewSampleRunID ) );
        desc.append( "." );
    }
    else
    {
        desc.append( ". Temporary sample-run ID is " );
        desc.append( m_candidateNewSampleRunID );
        desc.append( "." );
    }

    return desc;
}

void DBUpdateTaskInsertSampleRun::updateDatabase()
{
    const bool insertRequired = ! ( getSampleRunIDResolutionService()->hasMappingFor( m_candidateNewSampleRunID ) );

    if ( insertRequired  )
    {
        std::auto_ptr<paulstdb::Cursor> c( getConnection()->executeQuery( "select sample_run_id.nextval" ) );
        if ( c->endOfRecordSet() )
        {
            throw Exception( L"No rows obtained for: select sample_run_id.nextval" );
        }
        int newID = 0;
        c->read( 0, newID );
        c->close();
        const std::string newIDStr = paulst::toString(newID);
        std::string sql;
        sql.append( "insert into sample_run ( run_id, sequence_position ) values (" );
        sql.append( newIDStr );
        sql.append( ", " );
        sql.append( paulst::toString( m_sequencePosition ) );
        sql.append( " )" );
        getConnection()->executeStmt( sql );
        getSampleRunIDResolutionService()->addMapping( m_candidateNewSampleRunID, newIDStr );
    }
}

}


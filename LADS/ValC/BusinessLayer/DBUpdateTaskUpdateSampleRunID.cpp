#include "DBConnection.h"
#include "DBUpdateTaskUpdateSampleRunID.h"
#include "SampleRunIDResolutionService.h"
#include "StrUtil.h"

namespace valc
{

DBUpdateTaskUpdateSampleRunID::DBUpdateTaskUpdateSampleRunID( const std::string& candidateNewSampleRunID, int buddySampleID )
    :
    m_candidateNewSampleRunID( candidateNewSampleRunID ),
    m_buddySampleID( paulst::toString(buddySampleID) )
{
}

std::string DBUpdateTaskUpdateSampleRunID::describeUpdate() const
{
    std::string desc;

    std::string sampleRunID = getSampleRunIDResolutionService()->getMappingFor( m_candidateNewSampleRunID, m_candidateNewSampleRunID );

    desc.append( "Updating buddy_database.sample_run_id to " );
    desc.append( sampleRunID );
    desc.append( " where buddy_sample_id = " );
    desc.append( m_buddySampleID );
    desc.append( "." );
    
    return desc;
}

void DBUpdateTaskUpdateSampleRunID::updateDatabase()
{
    const std::string sampleRunID = getSampleRunIDResolutionService()->getMappingFor( m_candidateNewSampleRunID, m_candidateNewSampleRunID );

    std::string sql;
    sql.append( "update buddy_database set sample_run_id = " );
    sql.append( sampleRunID );
    sql.append( " where buddy_sample_id = " );
    sql.append( m_buddySampleID );
    getConnection()->executeStmt( sql );
}

}


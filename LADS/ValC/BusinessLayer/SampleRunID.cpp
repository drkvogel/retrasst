#include "Require.h"
#include "SampleRunID.h"
#include "SampleRunIDResolutionService.h"
#include "StringBuilder.h"

namespace valc
{
SampleRunID::SampleRunID()
    :
    m_sampleRunIDResolutionService(0)
{
}

SampleRunID::SampleRunID( const SampleRunID& o )
    :
    m_dbID                          ( o.m_dbID ),
    m_candidateID                   ( o.m_candidateID ),
    m_sampleRunIDResolutionService  ( o.m_sampleRunIDResolutionService )
{
}

SampleRunID::SampleRunID( int dbID )
    :
    m_dbID( std::string() << dbID ),
    m_sampleRunIDResolutionService(0)
{
    require( m_dbID.size() );
}

SampleRunID::SampleRunID( const std::string& candidateID, const SampleRunIDResolutionService* s )
    :
    m_candidateID( candidateID ),
    m_sampleRunIDResolutionService( s )
{
}

SampleRunID& SampleRunID::operator=( const SampleRunID& o )
{
    m_dbID                          = o.m_dbID;
    m_candidateID                   = o.m_candidateID;
    m_sampleRunIDResolutionService  = o.m_sampleRunIDResolutionService;
    return *this;
}

std::string SampleRunID::toString() const
{
    return m_dbID.size() ? m_dbID : m_sampleRunIDResolutionService->resolve( m_candidateID );
}

}



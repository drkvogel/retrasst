#include "SampleRunIDResolutionService.h"

namespace valc
{

SampleRunIDResolutionService::SampleRunIDResolutionService()
{
}

void SampleRunIDResolutionService::addMapping( const std::string& fromCandidate, const std::string& to )
{
    m_idMap.insert( std::make_pair( fromCandidate, to ) );
}

bool SampleRunIDResolutionService::canResolve( const std::string& candidateID ) const
{
    return m_idMap.count( candidateID );
}

std::string SampleRunIDResolutionService::resolve( const std::string& candidateID ) const
{
    IDMap::const_iterator i = m_idMap.find( candidateID );

    return i == m_idMap.end() ? candidateID : i->second;
}

}


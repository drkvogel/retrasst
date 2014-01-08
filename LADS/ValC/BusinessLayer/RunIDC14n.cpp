#include "RunIDC14n.h"

namespace valc
{

RunIDC14n::RunIDC14n( const SampleRunIDResolutionService& s )
    :
    m_idMap( s.m_idMap )
{
}

std::string RunIDC14n::toCanonicalForm( const std::string& sampleRunID ) const
{
    IDMap::const_iterator i = m_idMap.find( sampleRunID );

    return i == m_idMap.end() ? sampleRunID : i->second;
}

}


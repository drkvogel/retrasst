#include "AcquireCriticalSection.h"
#include "Require.h"
#include "SampleRunIDResolutionService.h"

namespace valc
{

SampleRunIDResolutionService::SampleRunIDResolutionService()
{
}

void SampleRunIDResolutionService::addMapping( const std::string& fromCandidate, const std::string& to )
{
    paulst::AcquireCriticalSection a(m_cs);

    {
        m_idMap.insert( std::make_pair( fromCandidate, to ) );
    }
}

void SampleRunIDResolutionService::clear()
{
    paulst::AcquireCriticalSection a(m_cs);

    {
        m_idMap.clear();
    }
}

std::string SampleRunIDResolutionService::resolve( const std::string& token ) const
{
    paulst::AcquireCriticalSection a(m_cs);

    {
        IDMap::const_iterator i = m_idMap.find( token );
        return i == m_idMap.end() ? token : i->second;
    }
}

}


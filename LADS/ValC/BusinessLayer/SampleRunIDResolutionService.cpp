#include "AcquireCriticalSection.h"
#include <boost/bind.hpp>
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

bool SampleRunIDResolutionService::compareSampleRunIDs( const std::string& oneRunID, const std::string& anotherRunID ) const
{
    paulst::AcquireCriticalSection a(m_cs);

    {
        return getMappingFor( oneRunID, oneRunID ) == getMappingFor( anotherRunID, anotherRunID );
    }
}

bool equalValue( const std::string& val, const std::pair< std::string, std::string >& entry ) 
{
    return val == entry.second;
}

void SampleRunIDResolutionService::clear()
{
    paulst::AcquireCriticalSection a(m_cs);

    {
        m_idMap.clear();
    }
}

bool SampleRunIDResolutionService::containsValue( const std::string& val ) const
{
    IDMap::const_iterator i = std::find_if( m_idMap.begin(), m_idMap.end(), boost::bind( equalValue, val, _1 ) );
    return i != m_idMap.end();
}

bool SampleRunIDResolutionService::existsAnAlternativeEquivalentFor( const std::string& runID ) const
{
    paulst::AcquireCriticalSection a(m_cs);

    {
        return hasMappingFor( runID ) || containsValue( runID );
    }
}

std::string SampleRunIDResolutionService::getAlternativeEquivalentFor( const std::string& runID ) const
{
    paulst::AcquireCriticalSection a(m_cs);

    {
        if ( hasMappingFor( runID ) )
        {
            return getMappingFor( runID );
        }
        else
        {
            IDMap::const_iterator i = std::find_if( m_idMap.begin(), m_idMap.end(), boost::bind( equalValue, runID, _1 ) );
            require( i != m_idMap.end() );
            return i->first;
        }
    }
}

std::string SampleRunIDResolutionService::getMappingFor( const std::string& candidateNewSampleRunID, const std::string& ifNoMapping ) const
{
    paulst::AcquireCriticalSection a(m_cs);

    {
        IDMap::const_iterator i = m_idMap.find( candidateNewSampleRunID );
        return i == m_idMap.end() ? ifNoMapping : i->second;
    }
}

bool SampleRunIDResolutionService::hasMappingFor( const std::string& candidateNewSampleRunID ) const
{
    paulst::AcquireCriticalSection a(m_cs);

    {
        return m_idMap.count( candidateNewSampleRunID );
    }
}

}


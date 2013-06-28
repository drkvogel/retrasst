#include "AcquireCriticalSection.h"
#include "API.h"
#include <boost/foreach.hpp>
#include "Require.h"
#include "WorklistEntries.h"

namespace valc
{

WorklistEntries::WorklistEntries()
{
}

void WorklistEntries::add( const WorklistEntry* e )
{
    paulst::AcquireCriticalSection a( m_criticalSection );

    {
        WorklistEntryPtr p(e);
        m_mapKeyedOnID.insert( std::make_pair( p->getID(), p ) );
        m_mapKeyedOnSampleDescriptor.insert( std::make_pair( p->getSampleDescriptor(), p) );
    }
}

void WorklistEntries::forEach( WorklistDirectory::Func& f, const std::string& sampleDescriptor ) const
{
    paulst::AcquireCriticalSection a( m_criticalSection );

    {
        MapKeyedOnSampleDescriptor::const_iterator begin, end;

        if ( sampleDescriptor.empty() )
        {
            begin = m_mapKeyedOnSampleDescriptor.begin();
            end   = m_mapKeyedOnSampleDescriptor.end();
        }
        else
        {
            std::pair<MapKeyedOnSampleDescriptor::const_iterator, MapKeyedOnSampleDescriptor::const_iterator> entriesForSample;

            entriesForSample = m_mapKeyedOnSampleDescriptor.equal_range(sampleDescriptor);

            begin = entriesForSample.first;
            end   = entriesForSample.second;
        }

        for( MapKeyedOnSampleDescriptor::const_iterator i = begin; i != end; ++i )
        {
            if ( ! sampleDescriptor.empty() )
            {
                require( i->second->getSampleDescriptor() == sampleDescriptor );
            }

            f.execute( i->second.get() );
        }
    }
}

const WorklistEntry* WorklistEntries::get( int id ) const
{
    paulst::AcquireCriticalSection a( m_criticalSection );

    {
        MapKeyedOnID::const_iterator i = m_mapKeyedOnID.find( id );

        return i == m_mapKeyedOnID.end() ? 0 : i->second.get();
    }
}

int WorklistEntries::size() const
{
    paulst::AcquireCriticalSection a( m_criticalSection );

    {
        return m_mapKeyedOnID.size();
    }
}

}


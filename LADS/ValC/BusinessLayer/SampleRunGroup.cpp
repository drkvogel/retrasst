#include <algorithm>
#include <iterator>
#include "Require.h"
#include "SampleRunGroup.h"

namespace valc
{

SampleRunGroup::SampleRunGroup( bool isQC, int groupID, bool canAcceptIDRevision )
    :
    m_isQC( isQC ),
    m_id( groupID ),
    m_canAcceptIDRevision( canAcceptIDRevision )
{
}

bool SampleRunGroup::canAcceptIDRevision( int newID ) const
{
    return ( newID == m_id ) || m_canAcceptIDRevision;
}

int  SampleRunGroup::getID() const
{
    return m_id;
}

bool SampleRunGroup::includes( const IDToken& sampleRunID ) const
{
    return m_members.contains( sampleRunID );
}

bool SampleRunGroup::isQC() const
{
    return m_isQC;
}

void SampleRunGroup::listRunIDs( IDTokenSequence& out ) const
{
    for ( const IDToken& t : m_members )
    {
        out.push_back( t );
    }
}

void SampleRunGroup::push_back( const IDToken& sampleRunID )
{
    m_members.push_back( sampleRunID );
}

void SampleRunGroup::reviseGroupID( int newGroupID )
{
    require( canAcceptIDRevision( newGroupID ) );

    m_id = newGroupID;

    m_canAcceptIDRevision = false;
}

}


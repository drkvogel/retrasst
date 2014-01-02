#include "AcquireCriticalSection.h"
#include "ExceptionUtil.h"
#include "Require.h"
#include "WorklistDirectory.h"
#include "WorklistLinks.h"
#include "WorklistNode.h"

namespace valc
{

WorklistLinks::WorklistLinks( const WorklistDirectory* worklistDirectory )
    :
    m_worklistDirectory( worklistDirectory )
{
}

WorklistLinks::~WorklistLinks()
{
    for ( NodeIndex::iterator i = m_nodeIndex.begin(); i != m_nodeIndex.end(); ++i )
    {
        delete i->second;
    }
}

void WorklistLinks::addLink( int from, int to, char howRelated )
{
    require( from != to );

    paulst::AcquireCriticalSection a(m_cs);

    {
        WorklistNode* fromNode = findNodeOrCreate( from );
        WorklistNode* toNode   = findNodeOrCreate( to );

        fromNode->linkTo( toNode, howRelated );
    }
}

WorklistNode* WorklistLinks::findNode( int id ) const
{
    NodeIndex::const_iterator i = m_nodeIndex.find( id );

    return i == m_nodeIndex.end() ? 0 : i->second;
}

std::vector<WorklistRelative> WorklistLinks::getChildren( int worklistID ) const
{
    paulst::AcquireCriticalSection a(m_cs);

    {
        if ( ! hasChildren( worklistID ) )
        {
            paulst::exception( "Worklist entry %d does not have children", worklistID );
        }

        std::vector<WorklistRelative> relatedEntries;

        WorklistNode* n = findNode( worklistID );

        if ( n )
        {
            for ( WorklistNode::const_iterator i = n->begin(); i != n->end(); ++i )
            {
                const WorklistNode* child = *i;
                WorklistRelative entry( child->getID(), m_worklistDirectory->get( child->getID() ), child->getRelation() );
                relatedEntries.push_back( entry );
            }
        }

        return relatedEntries;
    }
}

WorklistRelative WorklistLinks::getParent( int worklistID ) const
{
    paulst::AcquireCriticalSection a(m_cs);

    {
        if ( ! hasParent( worklistID ) )
        {
            paulst::exception( "Worklist entry %d does not have a parent", worklistID );
        }

        WorklistNode* n = findNode( worklistID );

        require( n );

        const WorklistNode* parent = n->getParent();

        require( parent );

        return WorklistRelative( parent->getID(), m_worklistDirectory->get( parent->getID() ), parent->getRelation() );
    }
}

bool WorklistLinks::hasChildren( int worklistID ) const
{
    paulst::AcquireCriticalSection a(m_cs);

    {
        const WorklistNode* n = findNode( worklistID );

        return n && ( n->begin() != n->end() );
    }
}

bool WorklistLinks::hasParent( int worklistID ) const
{
    paulst::AcquireCriticalSection a(m_cs);

    {
        const WorklistNode* n = findNode( worklistID );

        return n && n->hasParent();
    }
}

WorklistNode* WorklistLinks::findNodeOrCreate( int id )
{
    WorklistNode* n = findNode( id );

    if ( ! n )
    {
        n = new WorklistNode( id );
        m_nodeIndex.insert( std::make_pair(id, n) );
    }

    return n;
}

WorklistRelative WorklistLinks::wrap( const WorklistEntry* e ) const
{
    WorklistNode* n = findNode( e->getID() );

    return WorklistRelative( e->getID(), e, n ? n->getRelation() : '\0' );
}

}


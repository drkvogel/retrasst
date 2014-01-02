#include "Require.h"
#include "Trace.h"
#include "WorklistNode.h"

namespace valc
{

bool CompareNodes::operator()( const WorklistNode* n1, const WorklistNode* n2 ) const
{
    return n1->getID() < n2->getID();
}

WorklistNode::WorklistNode( int id )
    :
    m_id(id),
    m_parent( 0 ),
    m_relation( '\0' )
{
}

WorklistNode::~WorklistNode()
{
}

void WorklistNode::linkTo( WorklistNode* child, char howRelated )
{
    require( howRelated != '\0' );

    child->m_parent = this;
    child->m_relation = howRelated;

    m_childNodes.insert( child );
}

WorklistNode::const_iterator WorklistNode::begin() const
{
    return m_childNodes.begin();
}

WorklistNode::const_iterator WorklistNode::end() const
{
    return m_childNodes.end();
}

int WorklistNode::getID() const
{
    return m_id;
}

const WorklistNode* WorklistNode::getParent() const
{
    return m_parent;
}

char WorklistNode::getRelation() const
{
    return m_relation;
}

bool WorklistNode::hasParent() const
{
    return m_parent;
}

}


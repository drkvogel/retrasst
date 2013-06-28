#include "WorklistRelation.h"

namespace valc
{

WorklistRelation::WorklistRelation( int parent, int child, char relationshipMotivation )
    :
    m_parent( parent ),
    m_child( child ),
    m_relationshipMotivation( relationshipMotivation )
{
}

int WorklistRelation::getParent() const
{
    return m_parent;
}

int WorklistRelation::getChild() const
{
    return m_child;
}

char WorklistRelation::getRelationshipMotivation() const
{
    return m_relationshipMotivation;
}

WorklistRelationsDataSource::WorklistRelationsDataSource()
{
}

WorklistRelationsDataSource::~WorklistRelationsDataSource()
{
}

}


#ifndef WORKLISTRELATIONH
#define WORKLISTRELATIONH

#include <vector>

namespace valc
{

/*  Describes a relationship between 2 worklist entries. 

    Examples of when a worklist entry has a parent: 
        - when it is a rerun
        - when it is a triggered test
    Examples of when a worklist entry has children:
        - when it has been rerun
        - when it triggered further tests

    Refer to the worklist_relation database table.
*/
class WorklistRelation
{
public:
    WorklistRelation( int parent = 0, int child = 0, char relationshipMotivation = '\0' );
    int  getParent()                    const;
    int  getChild()                     const;
    char getRelationshipMotivation()    const;
private:
    int m_parent;
    int m_child;
    char m_relationshipMotivation;
};

typedef std::vector<WorklistRelation> WorklistRelations;

/* Encapsulates a means of obtaining the WorklistRelations for a given WorklistEntry. */
class WorklistRelationsDataSource
{
public:
    WorklistRelationsDataSource();
    virtual ~WorklistRelationsDataSource();
    virtual WorklistRelations getRelations( int worklistEntry ) = 0;
private:
    WorklistRelationsDataSource( const WorklistRelationsDataSource& );
    WorklistRelationsDataSource& operator=( const WorklistRelationsDataSource& );
};


};

#endif


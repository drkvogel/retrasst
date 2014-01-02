#ifndef WORKLISTNODEH
#define WORKLISTNODEH

#include <set>

namespace valc
{

class WorklistNode;

struct CompareNodes
{
    bool operator()( const WorklistNode* n1, const WorklistNode* n2 ) const;
};

class WorklistNode
{
public:
    typedef std::set< const WorklistNode*, CompareNodes > ChildNodes;
    typedef ChildNodes::const_iterator const_iterator;

    WorklistNode( int id );
    ~WorklistNode();
    // Note that nodes do not delete their children.
    void                linkTo( WorklistNode* child, char howRelated );
    const_iterator      begin()                                 const;
    const_iterator      end()                                   const;
    const WorklistNode* getParent()                             const;
    int                 getID()                                 const;
    char                getRelation()                           const;
    bool                hasParent()                             const;
private:
    const int                   m_id;
    char                        m_relation;
    const WorklistNode*         m_parent;
    ChildNodes                  m_childNodes;

    WorklistNode( const WorklistNode& );
    WorklistNode& operator=( const WorklistNode& );
};

}

#endif


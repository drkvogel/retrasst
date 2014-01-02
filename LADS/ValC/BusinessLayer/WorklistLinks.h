#ifndef WORKLISTLINKSH
#define WORKLISTLINKSH

#include "API.h"
#include "CritSec.h"
#include "IntList.h"
#include <map>

namespace valc
{

class WorklistDirectory;
class WorklistNode;

class WorklistLinks
{

public:
    WorklistLinks( const WorklistDirectory* worklistDirectory );
    ~WorklistLinks();
    void                            addLink                 ( int from, int to, char howRelated );
    std::vector<WorklistRelative>   getChildren             ( int worklistID ) const;
    WorklistRelative                getParent               ( int worklistID ) const;
    bool                            hasChildren             ( int worklistID ) const;
    bool                            hasParent               ( int worklistID ) const;
    WorklistRelative                wrap                    ( const WorklistEntry* e ) const;
private:
    const WorklistDirectory*    const m_worklistDirectory;
    paulst::CritSec                   m_cs;
    typedef std::map<int, WorklistNode*> NodeIndex;
    NodeIndex                         m_nodeIndex;
    
    WorklistNode*   findNode( int id ) const;
    WorklistNode*   findNodeOrCreate( int id );
};

}

#endif


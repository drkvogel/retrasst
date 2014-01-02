#ifndef WORKLISTRELATIVEIMPLH
#define WORKLISTRELATIVEIMPLH

#include "API.h"
#include <vector>

namespace valc
{

class WorklistLinks;

/*
    This is the declaration of which there is a forward declaration in API.h

    This class delegates to WorklistLinks but note that, whenever it is returning 
    an instance of WorklistRelative, it injects itself as the value of 
    the 'm_impl' member variable.
*/
class WorklistRelative::Impl
{
public:
    Impl( WorklistLinks* worklistLinks );
    bool                            hasChildren( int worklistID ) const;
    bool                            hasParent  ( int worklistID ) const;
    WorklistRelative                getParent  ( int worklistID ) const;
    std::vector<WorklistRelative>   getChildren( int worklistID ) const;
    WorklistRelative                wrap( const WorklistEntry* e ) const;
private:
    WorklistLinks* m_worklistLinks;
};

}

#endif


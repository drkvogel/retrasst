#ifndef WORKLISTENTRIESH
#define WORKLISTENTRIESH

#include <boost/shared_ptr.hpp>
#include "CritSec.h"
#include <map>
#include "WorklistDirectory.h"

namespace valc
{

class WorklistEntries : public WorklistDirectory
{
public:
    WorklistEntries();
    void add( const WorklistEntry* entry );
    void forEach( WorklistDirectory::Func& f, const std::string& sampleDescriptor = "" ) const;
    const WorklistEntry* get( int id ) const;
    int size() const;
private:
    typedef boost::shared_ptr< const WorklistEntry > WorklistEntryPtr;
    typedef std::map< int, WorklistEntryPtr > MapKeyedOnID;
    typedef std::multimap< std::string, WorklistEntryPtr > MapKeyedOnSampleDescriptor;
    MapKeyedOnID m_mapKeyedOnID;
    MapKeyedOnSampleDescriptor m_mapKeyedOnSampleDescriptor;
    paulst::CritSec m_criticalSection;
};

};

#endif


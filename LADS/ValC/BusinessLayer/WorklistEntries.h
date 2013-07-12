#ifndef WORKLISTENTRIESH
#define WORKLISTENTRIESH

#include <boost/shared_ptr.hpp>
#include "CritSec.h"
#include <map>
#include "WorklistDirectory.h"

namespace valc
{

typedef boost::shared_ptr< const WorklistEntry > WorklistEntryPtr;
typedef std::multimap< std::string, WorklistEntryPtr > WorklistEntriesKeyedOnSampleDescriptor;

class WorklistEntries : public WorklistDirectory
{
public:

    typedef WorklistEntriesKeyedOnSampleDescriptor::const_iterator const_iterator;

    WorklistEntries();
    void add( const WorklistEntry* entry );
    Range<WorklistEntryIterator> equal_range( const std::string& sampleDescriptor ) const;
    void forEach( WorklistDirectory::Func& f, const std::string& sampleDescriptor = "" ) const;
    const WorklistEntry* get( int id ) const;
    int size() const;
private:
    typedef std::map< int, WorklistEntryPtr > MapKeyedOnID;
    MapKeyedOnID m_mapKeyedOnID;
    WorklistEntriesKeyedOnSampleDescriptor m_mapKeyedOnSampleDescriptor;
    paulst::CritSec m_criticalSection;
};

};

#endif


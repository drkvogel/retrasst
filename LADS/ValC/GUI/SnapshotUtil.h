#ifndef SNAPSHOTUTILH
#define SNAPSHOTUTILH

#include <algorithm>
#include "API.h"

/*
    Methods for working with the API.
*/
namespace valcui
{

/* Returns an iterator to the start of the specified range.  */
template<class Iter>
Iter begin( valc::Range<Iter>& range ) { return range.first; }

/* Returns an iterator to the end of the specified range.  */
template<class Iter>
Iter end( valc::Range<Iter>& range ) { return range.second; }

/* Returns true if the specified range is empty. */
template<class Iter>
bool empty( valc::Range<Iter>& range )
{
    return 0 == std::distance( begin(range), end(range) );
}

/*
    Searches both the local runs and the queued samples, looking for the specified worklist entry.

    Returns NULL if not found.
*/
const valc::WorklistEntry* findWorklistEntry( int worklistEntryID, valc::SnapshotPtr inSnapshot );

/*
    Searches the specified range for the specified worklist entry.

    Returns NULL if not found.
*/
const valc::WorklistEntry* findWorklistEntry( int worklistEntryID, valc::Range<valc::WorklistEntryIterator> inRange );

/*
    Returns true if wle is the parent of a child worklist entry that is a rerun.
*/
bool hasRerun( const valc::WorklistEntry* wle, valc::SnapshotPtr snapshot );

/*
    Returns 'true' if the worklist entry has a barcode which starts with 'QC'.
*/
bool isQC( const valc::WorklistEntry* wle );
bool isQC( const std::string& barcode     );

class TestNames
{
public:
    TestNames();
    virtual ~TestNames();
    virtual std::string getNameFor( int testID ) = 0;
};


class TestNamesAdapter : public TestNames
{
public:
    TestNamesAdapter( valc::SnapshotPtr s );
    std::string getNameFor( int testID );
private:
    valc::SnapshotPtr m_snapshot;
};

}

#endif


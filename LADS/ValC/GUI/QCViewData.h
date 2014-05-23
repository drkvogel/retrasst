#ifndef QCVIEWDATAH
#define QCVIEWDATAH

#include <map>
#include <vector>

namespace valc
{
    class QCControls;
    class SnapshotPtr;
    class TestResult;
    class WorklistEntry;
}

namespace valcui
{

class QCViewData
{
public:

    typedef std::vector< const valc::TestResult* > LocalResults;
    typedef LocalResults::const_iterator const_iterator;

    QCViewData( const valc::SnapshotPtr& snapshot, int worklistID );
    const valc::WorklistEntry*   getWorklistEntry() const;
    int                         getWorklistEntryID() const;
    bool                        hasLocalResults() const;
    const_iterator              localResultBegin() const;
    const_iterator              localResultEnd() const;
    // Opportunities to look up supplementary control-related info. May return NULL.
    const valc::TestResult*     lookupQCResult( int resultID ) const;
    const valc::WorklistEntry*  lookupQCWorklistEntry( int resultID ) const;
    
private:
   /*
        localResults have a 'ControlStatus' property which (indirectly) publishes the ID of the QC results that 
        perform a controlling role.  However, there is no means of obtaining the associated Worklist entries for these QCs.
        SupplementaryQCData exists to compensate for this.  Given the ID of a QC result, querying SupplementaryQCData SHOULD
        yield the WorklistEntry for that QC (but the quality of the data is variable - there may be no entry in 
        some circumstances).
    */
    typedef std::map< int, const valc::WorklistEntry* > SupplementaryQCData;
    SupplementaryQCData         m_supplement;
	const int                   m_worklistEntryID;
    const valc::WorklistEntry*  m_worklistEntry{};
    LocalResults                m_localResults;


    void cacheSupplementaryWorklistEntriesFor   ( const std::vector<int>& resultIDs, const valc::SnapshotPtr& snapshot );
    void listLocalResultsForWorklistEntry       ( const valc::SnapshotPtr& snapshot );
    void listResultIDsOf                        ( const valc::QCControls& qcControls, std::vector<int>& out );
    void listResultIDsOfControllingQCsFor       ( const valc::TestResult* result, std::vector<int>& out );
};

}

#endif


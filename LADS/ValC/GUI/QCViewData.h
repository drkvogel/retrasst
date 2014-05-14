#ifndef QCVIEWDATAH
#define QCVIEWDATAH

#include "API.h"
#include "CritSec.h"
#include <map>
#include "TaskWithCallback.h"
#include <vector>

namespace stef
{
    class ThreadPool;
}

namespace valcui
{

class QCViewController;

struct QCViewData
{
    public:

	int worklistEntryID{};
    const valc::WorklistEntry* worklistEntry{};
    std::vector< const valc::TestResult* > localResults;

    private:
    /*
        localResults have a 'ControlStatus' property which (indirectly) publishes the ID of the QC results that 
        perform a controlling role.  However, there is no means of obtaining the associated Worklist entries for these QCs.
        SupplementaryQCData exists to compensate for this.  Given the ID of a QC result, querying SupplementaryQCData SHOULD
        yield the WorklistEntry for that QC (but the quality of the data is variable - there may be no entry in 
        some circumstances).
    */
    typedef std::map< int, const valc::WorklistEntry* > SupplementaryQCData;
    SupplementaryQCData supplement;

    public:

    // Opportunities to look up supplementary control-related info.
    // May return NULL.
    const valc::TestResult* lookupResult( int resultID ) const;
    const valc::WorklistEntry* lookupWorklistEntry( int resultID ) const;

    struct Factory
    {
        public:

        valc::SnapshotPtr snapshotPtr;
        int worklistEntryID;

        // This is the factory method, which builds an instance of QCViewData and assigns it to the supplied variable 'out'.
        void operator()( QCViewData& out );

        private:

        void findWorklistEntriesFor( const std::vector<int>& resultIDs, SupplementaryQCData& out );
        void listLocalResultsForWorklistEntry( QCViewData& out );
        void listResultIDsOf( const valc::QCControls& qcControls, std::vector<int>& out );
        void listResultIDsOfControllingQCsFor( const valc::TestResult* result, std::vector<int>& out );

        public:

        /*
            Construct an Order on the heap.  It will delete itself when it is done, after delivering output to QCViewController
            (method 'factoryCallback').
        */
        struct Order
        {
            QCViewController* controller;
            typedef stef::Submission< QCViewData, QCViewData::Factory, QCViewData::Factory::Order > OrderSubmission;
            OrderSubmission* submission;
            paulst::CritSec m_critSec;

            Order( valc::SnapshotPtr snapshotPtr, int worklistEntryID, QCViewController* c, stef::ThreadPool* tp );

            void set( bool cancelled, const std::string& error, const QCViewData& output );

        private:
            Order( const Order& );
            Order& operator=( const Order& );
        };
    };
};

}

#endif


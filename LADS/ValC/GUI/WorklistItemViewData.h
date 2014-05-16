#ifndef WORKLISTITEMVIEWDATAH
#define WORKLISTITEMVIEWDATAH

#include "API.h"
#include "CritSec.h"
#include "TaskWithCallback.h"

namespace stef
{
    class ThreadPool;
}

namespace valcui
{

class WorklistItemViewController;

struct WorklistItemViewData
{
	int worklistEntryID{};
    const valc::WorklistEntry* worklistEntry{};

    struct Factory
    {
        valc::SnapshotPtr snapshotPtr;
        int worklistEntryID;

        // This is the factory method, which builds an instance of WorklistItemViewData and 
        // assigns it to the supplied variable 'out'.
        void operator()( WorklistItemViewData& out );

        private:

        const valc::WorklistEntry* searchLocalRunSequenceForWorklistEntry( int worklistEntryID ) const;

        public:

        /*
            Construct an Order on the heap.  
            It will delete itself when it is done, after delivering output to WorklistItemViewController
            (method 'factoryCallback').
        */
        struct Order
        {
            WorklistItemViewController* controller;

            typedef stef::Submission< 
                WorklistItemViewData, 
                WorklistItemViewData::Factory, 
                WorklistItemViewData::Factory::Order
            > 
            OrderSubmission;

            OrderSubmission* submission;
            paulst::CritSec m_critSec;

            Order( valc::SnapshotPtr snapshotPtr, int worklistEntryID, WorklistItemViewController* c, stef::ThreadPool* tp );

            void set( bool cancelled, const std::string& error, const WorklistItemViewData& output );

        private:
            Order( const Order& );
            Order& operator=( const Order& );
        };
    };
};

}

#endif


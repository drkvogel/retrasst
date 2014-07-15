#ifndef SampleRunViewControllerH
#define SampleRunViewControllerH

#include "API.h"
#include <FMX.StdCtrls.hpp>
#include "IdleServiceUserAdapter.h"
#include "ModelEventListenerAdapter.h"
#include "Nullable.h"
#include <memory>
#include <set>
#include <vector>

class TSampleRunFrame;

namespace valcui
{

class Model;


struct CompareWorklistEntries
{
    /* Performs a comparison based on the ID of each WorklistEntry. */
    bool operator()( const valc::WorklistEntry* a, const valc::WorklistEntry* b );
};

typedef std::set< const valc::WorklistEntry*, CompareWorklistEntries > WorklistEntrySet;

struct RunAssociation
{
    valc::IDToken runID;
    bool isOpen{};

    RunAssociation( const valc::IDToken& run = valc::IDToken(), bool open = false );
};

class WorklistEntryContext
{
public:
    WorklistEntryContext( const valc::WorklistEntry* wle, valc::SnapshotPtr snapshot );
    bool            existsPendingForSample()const;
    std::string     getBarcode()            const;
    int             getID()                 const;
    RunAssociation  getRunAssociation()     const;
    std::string     getSampleDescriptor()   const;
    int             getSampleID()           const;
    int             getTestID()             const;
    std::string     getTestName()           const;

    /*
        Returns true if an association between the worklist entry and a sample-run has been 
        specified using 'setRunAssociation'.
    */
    bool hasRunAssociation()       const;

    /*
        The number of worklist entries that are for the same sample as this worklist entry and 
        for which results are still pending.
    */
    void setNumPendingForSample( int numPending );

    /*
        The sample-run with which this worklist entry is associated.
        Might be set multiple times, if a worklist entry has multiple results and these results
        have ended up in different sample-runs. In this case, this method overwrites any previous value.
    */
    void setRunAssociation( const valc::IDToken& runID, bool isRunOpen );

    /*
        The run with which the worklist entry will be associated unless any other is explicitly set
        via 'setRunAssociation'.
    */
    void updateFallbackRunAssociation( const valc::IDToken& runID, bool isRunOpen );

private:
    const int           m_id{};
    const int           m_testID{};
    const std::string   m_sampleDescriptor;
    const std::string   m_testName;
    const std::string   m_barcode;
    const int           m_sampleID{};
    int                 m_numPendingForSample{};
    paulst::Nullable<RunAssociation> m_runAssociation;
    paulst::Nullable<RunAssociation> m_fallbackRunAssociation;

    WorklistEntryContext( const WorklistEntryContext& );
    WorklistEntryContext& operator=( const WorklistEntryContext& );
};


class SampleRunViewController
{
public:
	SampleRunViewController();
    IdleServiceUser*    getIdleServiceUserInterface();
    ModelEventListener* getModelEventListenerInterface();
    void                init();
	void                notify( int modelEvent, const EventData& eventData );
    void                onIdle();
    void                onResize();
	void __fastcall     selectWorklistEntry(TObject* sender);
    void                setModel( Model* m );
    void                setView( TSampleRunFrame* v );
	void __fastcall     rerun(TObject* sender);
    void __fastcall     update();
private:
	TSampleRunFrame*                                    m_view{};
    IdleServiceUserAdapter<SampleRunViewController>     m_idleServiceUser;
	ModelEventListenerAdapter<SampleRunViewController>  m_eventListener;
	Model*                                              m_model{};
    std::vector< TFmxObject* >                          m_runContainerAdditions;
	std::unique_ptr<WorklistEntryContext>               m_selectedWorklistEntry;

	void            addResultBox(   valc::SnapshotPtr s, 
                                    TFlowLayout* l, 
                                    const valc::WorklistEntry* w, 
                                    const valc::TestResult* r, 
                                    const valc::IDToken& runID );
    TFlowLayout*    createRow( const std::string& labelText );
	void            describeLocalRun( const valc::LocalRun& lr, valc::SnapshotPtr snapshot, WorklistEntrySet& outPending );
	void            describePending( const WorklistEntrySet& worklistEntries, valc::SnapshotPtr s );
};

template<typename OutputIter>
void listResultCodes( const valc::WorklistEntry* qc, valc::SnapshotPtr snapshot, OutputIter i )
{
    auto results = qc->getTestResults();

    for ( auto result = begin(results); result != end(results); ++result )
    {
        const valc::TestResult* tr = *result;

        const bool isLocal = ! tr->getSampleRunID().isNull();

        if ( isLocal && snapshot->hasRuleResults( tr->getID() ) )
        {
            valc::RuleResults rr = snapshot->getRuleResults( tr->getID() );
            *i = rr.getSummaryResultCode();
            ++i;
        }
    }
}

}

#endif


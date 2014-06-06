#ifndef SampleRunViewControllerH
#define SampleRunViewControllerH

#include "API.h"
#include <FMX.StdCtrls.hpp>
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
    std::string runID{};
    bool isOpen{};

    RunAssociation( const std::string& run = "", bool open = false );
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
    void setRunAssociation( const std::string& runID, bool isRunOpen );

    /*
        The run with which the worklist entry will be associated unless any other is explicitly set
        via 'setRunAssociation'.
    */
    void updateFallbackRunAssociation( const std::string& runID, bool isRunOpen );

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
	SampleRunViewController( TSampleRunFrame* widgetContainer, Model* m );
	void notify( int modelEvent );
	void __fastcall selectWorklistEntry(TObject* sender);
	void __fastcall rerun(TObject* sender);
    void __fastcall update();
private:
	TSampleRunFrame* const                              m_widgetContainer{};
	ModelEventListenerAdapter<SampleRunViewController>  m_eventListener;
	Model* const                                        m_model{};
    std::vector< TFmxObject* >                          m_runContainerAdditions;
	std::unique_ptr<WorklistEntryContext>               m_selectedWorklistEntry;

	void addResultBox( valc::SnapshotPtr s, TFlowLayout* l, const valc::WorklistEntry* w, const valc::TestResult* r, const std::string& runID );
    TFlowLayout* createRow( const std::string& labelText );
	void describeLocalRun( const valc::LocalRun& lr, valc::SnapshotPtr snapshot, WorklistEntrySet& outPending );
	void describePending( const WorklistEntrySet& worklistEntries, valc::SnapshotPtr s );
};

}

#endif


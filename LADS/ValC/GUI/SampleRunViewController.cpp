#include <algorithm>
#include "ExceptionHandler.h"
#include "FMXTemplates.h"
#include <iterator>
#include "LocalRunIterator.h"
#include "Model.h"
#include "ModelEventConstants.h"
#include "Require.h"
#include "SampleRunViewController.h"
#include <set>
#include "SnapshotUtil.h"
#include "StrUtil.h"
#include "TSampleRunFrame.h"
#include <vector>

namespace valcui
{

/*
Comparison object for ordering a sequence of ResultCodes from worst to best.
*/
class WorstFirst
{
private:
    std::vector< valc::ResultCode > m_orderedList;

    int indexOf( const valc::ResultCode& r ) const
    {
        auto i = std::find( m_orderedList.begin(), m_orderedList.end(), r );
        require( i != m_orderedList.end() );
        return std::distance( m_orderedList.begin(), i );
    }

public:

    WorstFirst()
        :
        m_orderedList( 
            { 
                valc::ResultCode::RESULT_CODE_FAIL, 
                valc::ResultCode::RESULT_CODE_ERROR, 
                valc::ResultCode::RESULT_CODE_NULL, 
                valc::ResultCode::RESULT_CODE_NO_RULES_APPLIED,
                valc::ResultCode::RESULT_CODE_BORDERLINE, 
                valc::ResultCode::RESULT_CODE_PASS 
            } )
    {
    }

    WorstFirst( const WorstFirst& wf )
        :
        m_orderedList( wf.m_orderedList )
    {
    }

    WorstFirst& operator=( const WorstFirst& wf )
    {
        m_orderedList = wf.m_orderedList;
        return *this;
    }

    bool operator()( const valc::ResultCode& a, const valc::ResultCode& b ) const
    {
        return indexOf(a) < indexOf(b);
    }
};

RunAssociation::RunAssociation( const valc::IDToken& run, bool open )
    :
    runID( run ),
    isOpen( open )
{
}

WorklistEntryContext::WorklistEntryContext( const valc::WorklistEntry* wle, valc::SnapshotPtr snapshot )
    :
    m_id( wle->getID() ),
    m_testID( wle->getTestID() ),
    m_sampleDescriptor( wle->getSampleDescriptor() ),
    m_testName( snapshot->getTestName( wle->getTestID() ) ),
    m_barcode( wle->getBarcode() ),
    m_sampleID( wle->getSampleID() )
{
}

bool        WorklistEntryContext::existsPendingForSample()const { return m_numPendingForSample; }
std::string WorklistEntryContext::getBarcode()            const { return m_barcode; }
int         WorklistEntryContext::getID()                 const { return m_id; }
std::string WorklistEntryContext::getSampleDescriptor()   const { return m_sampleDescriptor; }
int         WorklistEntryContext::getSampleID()           const { return m_sampleID; }
int         WorklistEntryContext::getTestID()             const { return m_testID; }
std::string WorklistEntryContext::getTestName()           const { return m_testName; }
bool        WorklistEntryContext::hasRunAssociation()     const { return ! m_runAssociation.isNull(); }
void        WorklistEntryContext::setNumPendingForSample( int num ) { m_numPendingForSample = num; }

void WorklistEntryContext::setRunAssociation( const valc::IDToken& runID, bool isRunOpen )
{
    m_runAssociation = RunAssociation( runID, isRunOpen );
}

void WorklistEntryContext::updateFallbackRunAssociation( const valc::IDToken& runID, bool isRunOpen )
{
    m_fallbackRunAssociation = RunAssociation( runID, isRunOpen );
}

RunAssociation WorklistEntryContext::getRunAssociation() const
{
    require( hasRunAssociation() || ! m_fallbackRunAssociation.isNull() );

    return hasRunAssociation() ? m_runAssociation : m_fallbackRunAssociation;
}

//////////////////////////////////////////////////////////////////////////////////////////////////////

bool CompareWorklistEntries::operator()( const valc::WorklistEntry* a, const valc::WorklistEntry* b )
{
    return a->getID() < b->getID();
}

//////////////////////////////////////////////////////////////////////////////////////////////////////

SampleRunViewController::SampleRunViewController()
    :
    m_view( 0 ),
    m_idleServiceUser( this ),
    m_eventListener( this ),
	m_model( 0 )
{
}



void SampleRunViewController::addResultBox(
    valc::SnapshotPtr snapshot,
    TFlowLayout* toLayout,
    const valc::WorklistEntry* worklistEntry,
    const valc::TestResult* result = NULL,
    const valc::IDToken& runID = valc::IDToken() )
{
    TPanel* p = new TPanel(toLayout);
    p->Parent = toLayout;
    p->StyleLookup = L"resultbox";
    p->Width = 60;
    toLayout->AddObject(p);

    setText( p, "resultbox_test", snapshot->getTestName( worklistEntry->getTestID() ) );
    setText( p, "resultbox_value", result ? paulst::formatFloatToFit( (double)result->getResultValue(), 8 ) : std::string() );

    std::string statusStyle;

    if ( ! result )
    {
        statusStyle = "status_pending";
    }
    else
	{
		// Status is differently determined for QCs than for Unknowns
        if ( isQC( worklistEntry ) )
        {
            std::set< valc::ResultCode, WorstFirst > resultCodes;

            listResultCodes( worklistEntry, snapshot, std::inserter(resultCodes, resultCodes.begin() ) );

            if ( resultCodes.empty() )
            {
                statusStyle = "status_pending";
            }
            else
            {
                switch( *(resultCodes.begin()) ) // worst-case scenario
                {
                    case valc::ResultCode::RESULT_CODE_FAIL:             
                    case valc::ResultCode::RESULT_CODE_ERROR:
                    case valc::ResultCode::RESULT_CODE_NO_RULES_APPLIED:
                    case valc::ResultCode::RESULT_CODE_NULL:
                        statusStyle = "status_fail"; break;
                    case valc::ResultCode::RESULT_CODE_BORDERLINE:
                        statusStyle = "status_warn"; break;
                    case valc::ResultCode::RESULT_CODE_PASS:
						statusStyle = "status_ok"; break;
					default:
						throwException( "Unexpected value for ResultCode" );
                }
            }
        }
        else
        {
            switch( result->getControlStatus().summaryCode() )
            {
            case valc::CONTROL_STATUS_CONFIG_ERROR_NO_RULES :
            case valc::CONTROL_STATUS_ERROR                 :
            case valc::CONTROL_STATUS_FAIL                  : statusStyle = "status_fail"; break;
            case valc::CONTROL_STATUS_UNCONTROLLED          : 
            case valc::CONTROL_STATUS_BORDERLINE            : statusStyle = "status_warn"; break;
            case valc::CONTROL_STATUS_PASS                  : statusStyle = "status_ok"  ; break;
            }
        }
    }

    require( ! statusStyle.empty() );

    TPanel* statusPanel = findStyleResource<TPanel>( p, "resultbox_status" );
    statusPanel->StyleLookup = statusStyle.c_str();

	p->PopupMenu = NULL;
	p->Tag = worklistEntry->getID();
	p->OnClick = selectWorklistEntry;

    require( m_selectedWorklistEntry.get() );

	if ( worklistEntry->getID() == m_selectedWorklistEntry->getID() )
	{
		TRectangle* r = findStyleResource<TRectangle>( p, "resultbox_bounds" );
		r->Stroke->Thickness = 4;

		if ( ! isQC(worklistEntry) && ! hasRerun(worklistEntry, snapshot) )
		{
			p->PopupMenu = m_view->popupMenu;
		}
	}
}

TFlowLayout* SampleRunViewController::createRow( const std::string& labelText )
{
    TScrollBox* sb = m_view->runContainer;

    const int runBoxHeight = 85;
    const int spacing = 5;
    TPanel* p = new TPanel(sb);
    p->Parent = sb;
    p->Position->X = 5;
    p->Position->Y = 25 + ( m_runContainerAdditions.size() * ( runBoxHeight + spacing ) );
	p->StyleLookup = "runbox";
	p->Width = 985;
    sb->AddObject( p );
    m_runContainerAdditions.push_back( p );

	setText( p, "caption", labelText );

	TFlowLayout* fl = findStyleResource<TFlowLayout>( p, "resultflow" );

	/*
	FireMonkey appears sometimes to cache styles. Consequently,
	the flowlayout obtained above sometimes comes with sub-elements
	already added. Hence...
	*/
	fl->DeleteChildren();

	assertion( fl->ChildrenCount == 0, "FlowLayout not expected to have any children yet" );

	return fl;
}

void SampleRunViewController::describeLocalRun( const valc::LocalRun& lr, valc::SnapshotPtr snapshot, WorklistEntrySet& outPending )
{
	TFlowLayout* fl = createRow(
		paulst::format( "Batch: %d     Run: %s       Closed-off: %s",
			lr.getGroupID, lr.getRunID().value().c_str(), lr.isOpen() ? "false" : "true" ) );

    auto worklistRange = snapshot->getWorklistEntries( lr.getSampleDescriptor() );

    for ( auto worklistEntryIter = begin(worklistRange); worklistEntryIter != end(worklistRange); ++worklistEntryIter )
    {
        const valc::WorklistEntry* worklistEntry = *worklistEntryIter;

        auto resultRange = worklistEntry->getTestResults();

        if ( empty( resultRange ) )
        {
			outPending.insert( worklistEntry );
            continue;
        }
        
        for ( auto resultIter = begin(resultRange); resultIter != end(resultRange); ++resultIter )
        {
            const valc::TestResult* result = *resultIter;

            if ( lr.getRunID() == result->getSampleRunID() )
			{
				// Local results
				addResultBox( snapshot, fl, worklistEntry, result, lr.getRunID() );

				if ( worklistEntry->getID() == m_selectedWorklistEntry->getID() )
				{
                    m_selectedWorklistEntry->setRunAssociation( lr.getRunID(), lr.isOpen() );
				}
            }
        }
    }

    if ( ! m_selectedWorklistEntry->hasRunAssociation() )
    {
        // Until we know different, will have to treat the selected worklist entry as if it were pending.
        // The effect of this code is that we associate pending worklist entries with the MOST RECENT local run in which the 
        // sample is known to have been involved.
        m_selectedWorklistEntry->updateFallbackRunAssociation( lr.getRunID(), lr.isOpen() );
    }
}

void SampleRunViewController::describePending( const WorklistEntrySet& pending, valc::SnapshotPtr snapshot )
{
    TFlowLayout* fl = createRow( "Pending" );

    for ( auto worklistEntry : pending )
    {
		addResultBox( snapshot, fl, worklistEntry );
    }
}

IdleServiceUser* SampleRunViewController::getIdleServiceUserInterface()
{
    return &m_idleServiceUser;
}

ModelEventListener* SampleRunViewController::getModelEventListenerInterface()
{
    return &m_eventListener;
}

void SampleRunViewController::init()
{
	TMenuItem* menuItem = new TMenuItem(m_view->popupMenu);
	menuItem->OnClick = rerun;
	menuItem->Parent = m_view->popupMenu;
	menuItem->Text = "Rerun...";

	m_view->popupMenu->AddObject( menuItem );

    if ( m_model->getSelectedWorklistEntry() )
    {
        m_model->borrowSnapshot( update );
    }
}

void SampleRunViewController::notify( int modelEvent, const EventData& eventData )
{
    switch( modelEvent )
    {
    case MODEL_EVENT::WORKLIST_ENTRY_SELECTION_CHANGE:
    case MODEL_EVENT::RERUN_QUEUED:
        m_model->borrowSnapshot( update );
        break;
    }
}

void SampleRunViewController::onIdle()
{
}

void SampleRunViewController::onResize()
{
}

void __fastcall SampleRunViewController::selectWorklistEntry( TObject* sender )
{
	require( dynamic_cast<TPanel*>(sender) );

	TPanel* resultBox = (TPanel*)sender;
	const int worklistEntryID = resultBox->Tag;

	require( worklistEntryID );

	m_model->setSelectedWorklistEntry( worklistEntryID );
}

void __fastcall SampleRunViewController::rerun(TObject* sender)
{
	std::ostringstream msg;

	msg << "Rerun test '" << m_selectedWorklistEntry->getTestName() 
        << "' for barcode '" << m_selectedWorklistEntry->getBarcode() << "'?";

    if ( m_selectedWorklistEntry->getRunAssociation().isOpen && m_selectedWorklistEntry->existsPendingForSample() )
    {
        msg << "\n\n"
            << "WARNING:\n\n"
            << "This worklist entry is associated with open sample-run '"
            << m_selectedWorklistEntry->getRunAssociation().runID << "'."
            << " There are associated worklist entries with no results."
            << " Rerunning this now means CLOSING-OFF these worklist entries.";
    }

    ShowMessage( msg.str().c_str() );

    m_model->doRerun(
        m_selectedWorklistEntry->getID(),
        m_selectedWorklistEntry->getRunAssociation().runID,
        m_selectedWorklistEntry->getSampleDescriptor(),
        m_selectedWorklistEntry->getBarcode(),
        m_selectedWorklistEntry->getTestName() );
}

void SampleRunViewController::setModel( Model* m )
{
    m_model = m;
}

void SampleRunViewController::setView( TSampleRunFrame* view )
{
    m_view = view;
}
void __fastcall SampleRunViewController::update()
{
    try
    {
        for ( TFmxObject* o : m_runContainerAdditions )
        {
            m_view->runContainer->RemoveObject(o);
        }

		m_runContainerAdditions.clear();
		WorklistEntrySet pending;
		m_selectedWorklistEntry.reset( NULL );
		valc::SnapshotPtr snapshot = m_model->getSnapshot();

        do
        {
            const int worklistID = m_model->getSelectedWorklistEntry();
        
            if ( ! worklistID )
            {
                break;
            }

            const valc::WorklistEntry* worklistEntry = findWorklistEntry( worklistID, snapshot );

            if ( ! worklistEntry )
            {
                break;
            }
            
            m_selectedWorklistEntry.reset( new WorklistEntryContext(worklistEntry, snapshot) );

            m_view->barcode ->Text   = m_selectedWorklistEntry->getBarcode().c_str();
            m_view->sampleID->Text   = m_selectedWorklistEntry->getSampleID();

            for ( LocalRunIterator localRun( snapshot->localBegin(), snapshot->localEnd() ), eof; localRun != eof; ++localRun )
            {
                if ( localRun->getSampleDescriptor() == m_selectedWorklistEntry->getSampleDescriptor() )
                {
                    describeLocalRun( *localRun, snapshot, pending );
                }
            }

            m_selectedWorklistEntry->setNumPendingForSample( pending.size() );

            if ( pending.size() )
            {
                describePending(pending, snapshot );
            }
        } 
        while(false);
    }
    catch( const Exception& e )
    {
        ExceptionHandler()(e);
    }
}

}


#include "ExceptionHandler.h"
#include "FMXTemplates.h"
#include "LocalRunIterator.h"
#include "Model.h"
#include "ModelEventConstants.h"
#include "Require.h"
#include "SampleRunViewController.h"
#include "SnapshotUtil.h"
#include "StrUtil.h"
#include "TSampleRunFrame.h"

namespace valcui
{


RunAssociation::RunAssociation( const std::string& run, bool open )
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
bool        WorklistEntryContext::hasRunAssociation()     const { return ! m_runAssociation.isNull; }
void        WorklistEntryContext::setNumPendingForSample( int num ) { m_numPendingForSample = num; }

void WorklistEntryContext::setRunAssociation( const std::string& runID, bool isRunOpen )
{
    m_runAssociation = RunAssociation( runID, isRunOpen );
}

void WorklistEntryContext::updateFallbackRunAssociation( const std::string& runID, bool isRunOpen )
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

SampleRunViewController::SampleRunViewController( TSampleRunFrame* widgetContainer, Model* m )
    :
    m_widgetContainer( widgetContainer ),
    m_eventListener( this ),
	m_model( m )
{
	m_model->registerModelEventListener( &m_eventListener );

	TMenuItem* menuItem = new TMenuItem(m_widgetContainer->popupMenu);
	menuItem->OnClick = rerun;
	menuItem->Parent = m_widgetContainer->popupMenu;
	menuItem->Text = "Rerun...";

	m_widgetContainer->popupMenu->AddObject( menuItem );
}

void SampleRunViewController::addResultBox(
    valc::SnapshotPtr snapshot,
    TFlowLayout* toLayout,
    const valc::WorklistEntry* worklistEntry,
    const valc::TestResult* result = NULL,
    const std::string& runID = "" )
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
        switch( result->getControlStatus().summaryCode() )
        {
        case valc::CONTROL_STATUS_UNCONTROLLED: statusStyle = isQC(worklistEntry) ? "status_ok" : "status_warn"; break;
        case valc::CONTROL_STATUS_CONFIG_ERROR_NO_RULES :
        case valc::CONTROL_STATUS_ERROR                 :
        case valc::CONTROL_STATUS_FAIL                  : statusStyle = "status_fail"; break;
        case valc::CONTROL_STATUS_BORDERLINE            : statusStyle = "status_warn"; break;
        case valc::CONTROL_STATUS_PASS                  : statusStyle = "status_ok"  ; break;
        }
    }

    require( ! statusStyle.empty() );

    TPanel* statusPanel = findStyleResource<TPanel>( p, "resultbox_status" );
    statusPanel->StyleLookup = statusStyle.c_str();

	p->PopupMenu = NULL;
	p->Tag = worklistEntry->getID();
	p->OnClick = selectWorklistEntry;
	p->HitTest = true;

    require( m_selectedWorklistEntry.get() );

	if ( worklistEntry->getID() == m_selectedWorklistEntry->getID() )
	{
		TRectangle* r = findStyleResource<TRectangle>( p, "resultbox_bounds" );
		r->Stroke->Thickness = 4;

		if ( ! isQC(worklistEntry) && ! hasRerun(worklistEntry, snapshot) )
		{
			p->PopupMenu = m_widgetContainer->popupMenu;
		}
	}
}

TFlowLayout* SampleRunViewController::createRow( const std::string& labelText )
{
    TScrollBox* sb = m_widgetContainer->runContainer;

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

	return findStyleResource<TFlowLayout>( p, "resultflow" );
}

void SampleRunViewController::describeLocalRun( const valc::LocalRun& lr, valc::SnapshotPtr snapshot, WorklistEntrySet& outPending )
{
	TFlowLayout* fl = createRow(
		paulst::format( "Batch: %d     Run: %s       Closed-off: %s",
			lr.getGroupID, lr.getRunID().c_str(), lr.isOpen() ? "false" : "true" ) );

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

            if ( snapshot->compareSampleRunIDs( lr.getRunID(), result->getSampleRunID() ) )
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

void SampleRunViewController::notify( int modelEvent )
{
	if ( modelEvent == MODEL_EVENT::WORKLIST_ENTRY_SELECTION_CHANGE )
	{
		m_model->borrowSnapshot(update);
	}
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
}

void __fastcall SampleRunViewController::update()
{
    try
    {
        for ( TFmxObject* o : m_runContainerAdditions )
        {
            m_widgetContainer->runContainer->RemoveObject(o);
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

            m_widgetContainer->barcode ->Text   = m_selectedWorklistEntry->getBarcode().c_str();
            m_widgetContainer->sampleID->Text   = m_selectedWorklistEntry->getSampleID();

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


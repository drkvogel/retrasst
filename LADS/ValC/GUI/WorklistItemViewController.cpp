#include "AcquireCriticalSection.h"
#include "FMXTemplates.h"
#include "Model.h"
#include "WorklistItemViewController.h"
#include "Require.h"
#include <sstream>


namespace valcui
{

WorklistItemViewController::WorklistItemViewController( TWorklistItemViewFrame* widgetContainer, Model* m )
	:
	m_widgetContainer( widgetContainer ),
	m_eventListener(this),
    m_threadPool(NULL)
{
	m->registerModelEventListener( &m_eventListener );
}

void addCellText(
	TListBoxItem* i,
	const UnicodeString& cell,
	const UnicodeString& text )
{
	TText* t = (TText*)i->FindStyleResource(cell);

	if ( t )
	{
		t->Text = text;
	}
}

void WorklistItemViewController::clear()
{
	m_widgetContainer->resultListBox->Clear();
}

void WorklistItemViewController::describeResult( const valc::TestResult* r )
{
	TListBox* b = m_widgetContainer->resultListBox;

	TListBoxItem* i = new TListBoxItem(b);
	i->Parent = b;
	i->StyleLookup = L"PropertyGrid";
    i->Height = 90;
	addCellText( i, "1_1", "ID:" );
	addCellText( i, "1_2", r->getID() );
	addCellText( i, "2_1", "Value:" );
	addCellText( i, "2_2", r->getResultValue() );
	addCellText( i, "3_1", "Date Analysed:" );
	addCellText( i, "3_2", r->getDateAnalysed().FormatString(L"d mmm h:nn:ss") );
}

UnicodeString WorklistItemViewController::describeTest( int testID ) const
{
    std::ostringstream s;
    s << m_snapshotPtr->getTestName( testID ) << " (" << testID << ")";
    return s.str().c_str();
}

void WorklistItemViewController::factoryCallback( bool cancelled, const std::string& error, const WorklistItemViewData& output )
{
    paulst::AcquireCriticalSection a(m_critSec);

    if ( cancelled || error.size() )
    {
        m_viewData.reset();
    }
    else
    {
        m_viewData = std::unique_ptr<WorklistItemViewData>(new WorklistItemViewData(output));
    }

    TThread::Queue(NULL,useUpdatedViewData);
}

void WorklistItemViewController::onForceReload( valc::SnapshotPtr& sp )
{
    m_snapshotPtr = sp;
}

void WorklistItemViewController::onWarningAlarmOn()
{
}

void WorklistItemViewController::onWarningAlarmOff()
{
}

void WorklistItemViewController::onWorklistEntrySelected( int worklistEntryID )
{
    require( m_threadPool );
    new WorklistItemViewData::Factory::Order( m_snapshotPtr, worklistEntryID, this, m_threadPool );
}

void WorklistItemViewController::setThreadPool( stef::ThreadPool* tp )
{
    m_threadPool = tp;
}

void __fastcall WorklistItemViewController::useUpdatedViewData()
{
    paulst::AcquireCriticalSection a(m_critSec);
    
    clear();

    if ( m_viewData && m_viewData->worklistEntry )
    {
		m_widgetContainer->ID       ->Text = m_viewData->worklistEntry->getID();
        m_widgetContainer->barcode  ->Text = m_viewData->worklistEntry->getBarcode().c_str();
        m_widgetContainer->machineID->Text = m_viewData->worklistEntry->getMachineID();
        m_widgetContainer->test     ->Text = describeTest( m_viewData->worklistEntry->getTestID() );
        m_widgetContainer->status   ->Text = m_viewData->worklistEntry->getStatus();
        m_widgetContainer->projectID->Text = m_viewData->worklistEntry->getProjectID();
		m_widgetContainer->timestamp->Text = m_viewData->worklistEntry->getTimeStamp().FormatString(L"d mmm h:nn:ss");
		valc::Range<valc::TestResultIterator> results =
			m_viewData->worklistEntry->getTestResults();

		for( auto result = results.first; result != results.second; ++result )
		{
			describeResult( *result );
		}
	}
}

}


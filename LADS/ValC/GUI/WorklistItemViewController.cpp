#include "API.h"
#include "FMXTemplates.h"
#include "LocalRunIterator.h"
#include "Model.h"
#include "ModelEventConstants.h"
#include "WorklistItemViewController.h"
#include "Require.h"
#include <sstream>
#include "StrUtil.h"
#include "TWorklistItemViewFrame.h"


namespace valcui
{

WorklistItemViewController::WorklistItemViewController()
	:
	m_view( 0 ),
    m_idleServiceUser(this),
	m_eventListener(this),
    m_model( 0 )
{
}

void addCellText( TListBoxItem* i, const UnicodeString& cell, const UnicodeString& text )
{
	TText* t = (TText*)i->FindStyleResource(cell);

	if ( t )
	{
		t->Text = text;
	}
}

void WorklistItemViewController::clear()
{
	m_view->ID       ->Text = L"";
    m_view->barcode  ->Text = L"";
    m_view->machineID->Text = L"";
    m_view->test     ->Text = L"";
    m_view->status   ->Text = L"";
    m_view->projectID->Text = L"";
    m_view->timestamp->Text = L"";
	m_view->resultListBox->Clear();
	m_view->familyTree->Clear();
}

void WorklistItemViewController::addTreeNodeForWorklistEntry(
	int idOfSelectedWorklistEntry,
	const valc::SnapshotPtr& snapshot,
	const valc::WorklistRelative& wr,
	TTreeViewItem* parent )
{
    TTreeViewItem* newNode{};

    if ( parent )
    {
        newNode = addNodeUnder( parent, describe(snapshot,wr) );
    }
    else
    {
        newNode = addNodeUnder( m_view->familyTree, describe(snapshot,wr) );
    }

	newNode->Tag     = wr.getID();
	newNode->OnClick = familyTreeClickHandler;
	newNode->Expand();

	if ( wr.getID() == idOfSelectedWorklistEntry )
	{
		newNode->Select();
	}

	if( wr.hasChildren() )
    {
        auto children = wr.getChildren();

        for ( valc::WorklistRelative& child : children )
        {
            addTreeNodeForWorklistEntry( idOfSelectedWorklistEntry, snapshot, child, newNode );
        }
    }
}

std::string WorklistItemViewController::describe( const valc::SnapshotPtr& snapshot, const valc::WorklistRelative& wr ) const
{
    std::ostringstream s;

    s << wr.getID();

    switch( wr.getRelation() )
    {
    case 'T': s << " [triggered]"; break;
    case 'R': s << " [rerun]"; break;
    }

    if ( wr.isBoundToWorklistEntryInstance() )
    {
        s << " " << wr->getBarcode() << " " << snapshot->getTestName( wr->getTestID() );
    }

    return s.str();
}

void WorklistItemViewController::describeFamilyTree( const valc::SnapshotPtr& snapshot, const valc::WorklistEntry* w )
{
    valc::WorklistRelative wr = snapshot->viewRelatively( w );

    while ( wr.hasParent() )
    {
        wr = wr.getParent();
    }

	addTreeNodeForWorklistEntry( w->getID(), snapshot, wr ); // recursively adds nodes for all the children
}
 
void WorklistItemViewController::describeResult( const valc::TestResult* r )
{
	TListBox* b = m_view->resultListBox;

	TListBoxItem* i = new TListBoxItem(b);
	i->Parent = b;
	i->StyleLookup = L"PropertyGrid";
    i->Height = 90;
	addCellText( i, "1_1", "ID:" );
	addCellText( i, "1_2", r->getID() );
	addCellText( i, "2_1", "Value:" );
	addCellText( i, "2_2", paulst::formatFloatToFit( r->getResultValue(), 8/*max chars*/ ).c_str() );
	addCellText( i, "3_1", "Date Analysed:" );
	addCellText( i, "3_2", r->getDateAnalysed().FormatString(L"d mmm h:nn:ss") );
}

UnicodeString WorklistItemViewController::describeTest( const valc::SnapshotPtr& snapshot, int testID ) const
{
    std::ostringstream s;
    s << snapshot->getTestName( testID ) << " (" << testID << ")";
    return s.str().c_str();
}

void __fastcall WorklistItemViewController::familyTreeClickHandler( TObject* sender )
{
	TTreeViewItem* i = dynamic_cast<TTreeViewItem*>(sender);
	if ( i )
	{
		const UnicodeString clickedOn( i->Tag );

		if ( m_view->ID->Text != clickedOn )
		{
			m_model->setSelectedWorklistEntry( i->Tag );
		}
	}
}

IdleServiceUser* WorklistItemViewController::getIdleServiceUserInterface()
{
    return &m_idleServiceUser;
}

ModelEventListener* WorklistItemViewController::getModelEventListenerInterface()
{
    return &m_eventListener;
}

void WorklistItemViewController::init()
{
    if ( m_model->getSelectedWorklistEntry() )
    {
        m_model->borrowSnapshot( update );
    }
}

void WorklistItemViewController::notify( int modelEvent, const EventData& eventData )
{
    switch( modelEvent )
    {
    case MODEL_EVENT::WORKLIST_ENTRY_SELECTION_CHANGE:
    case MODEL_EVENT::RERUN_QUEUED:
        m_model->borrowSnapshot( update );
        break;
    }
}

void WorklistItemViewController::onIdle()
{
}

void WorklistItemViewController::onResize()
{
}

const valc::WorklistEntry* WorklistItemViewController::searchLocalRunSequenceForWorklistEntry( 
    const valc::SnapshotPtr& snapshot, 
    int worklistEntryID ) const
{
    for ( LocalRunIterator localRun( snapshot->localBegin(), snapshot->localEnd() ), end; localRun != end; ++localRun )
    {
        valc::Range<valc::WorklistEntryIterator> worklistEntries = snapshot->getWorklistEntries( localRun->getSampleDescriptor() );

        for ( valc::WorklistEntryIterator w = worklistEntries.first; w != worklistEntries.second; ++w )
        {
            if ( (*w)->getID() == worklistEntryID )
			{
				return *w;
            }
        }
    }
    return NULL;
}            

const valc::WorklistEntry* WorklistItemViewController::searchQueueForWorklistEntry( 
    const valc::SnapshotPtr& snapshot, int worklistEntryID ) const
{
    for ( auto queuedSample = snapshot->queueBegin(); queuedSample != snapshot->queueEnd(); ++queuedSample )
    {
        auto worklistEntries = snapshot->getWorklistEntries( queuedSample->getSampleDescriptor() );
        for ( auto worklistEntry = worklistEntries.first; worklistEntry != worklistEntries.second; ++worklistEntry )
        {
            if ( (*worklistEntry)->getID() == worklistEntryID )
            {
                return *worklistEntry;
            }
        }
    }

    return NULL;
}

void WorklistItemViewController::setModel( Model* m )
{
    m_model = m;
}

void WorklistItemViewController::setView( TWorklistItemViewFrame* v )
{
    m_view = v;
}

void __fastcall WorklistItemViewController::update()
{
    do
    {
        clear();

        const valc::SnapshotPtr snapshot( m_model->getSnapshot() );

        const int worklistEntryID = m_model->getSelectedWorklistEntry();

        if ( ! worklistEntryID )
        {
            break;
        }

        const valc::WorklistEntry* worklistEntry = searchLocalRunSequenceForWorklistEntry( snapshot, worklistEntryID  );

        if ( ! worklistEntry )
        {
            worklistEntry = searchQueueForWorklistEntry( snapshot, worklistEntryID );
        }

        if ( ! worklistEntry )
        {
            break;
        }

        m_view->ID       ->Text = worklistEntry->getID();
        m_view->barcode  ->Text = worklistEntry->getBarcode().c_str();
        m_view->machineID->Text = worklistEntry->getMachineID();
        m_view->test     ->Text = describeTest( snapshot, worklistEntry->getTestID() );
        m_view->status   ->Text = worklistEntry->getStatus();
        m_view->projectID->Text = worklistEntry->getProjectID();
        m_view->timestamp->Text = worklistEntry->getTimeStamp().FormatString(L"d mmm h:nn:ss");

        valc::Range<valc::TestResultIterator> results = worklistEntry->getTestResults();

        for( auto result = results.first; result != results.second; ++result )
        {
            describeResult( *result );
        }

        describeFamilyTree( snapshot, worklistEntry );
    }
    while( false );
}

}


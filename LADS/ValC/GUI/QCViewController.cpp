#include "FMXTemplates.h"
#include "Model.h"
#include "ModelEventConstants.h"
#include "QCVDisplayModeQC.h"
#include "QCVDisplayModeUnknown.h"
#include "QCViewController.h"
#include "Require.h"
#include "SnapshotUtil.h"
#include <sstream>


namespace valcui
{

QCViewController::QCVDisplayMode::QCVDisplayMode()
{
}

QCViewController::QCVDisplayMode::~QCVDisplayMode()
{
}

QCViewController::QCViewController()
	:
	m_view( 0 ),
	m_eventListener(this),
    m_idleServiceUser(this),
    m_model(0)
{
}

std::string QCViewController::describe( valc::ResultCode rc ) const
{
    std::string s;

    switch( rc )
    {
    case valc::RESULT_CODE_FAIL 		   	: s = "FAIL"; break;
    case valc::RESULT_CODE_PASS 		   	: s = "PASS"; break;
    case valc::RESULT_CODE_BORDERLINE 	   	: s = "BORDERLINE"; break;
    case valc::RESULT_CODE_ERROR 		   	: s = "ERROR"; break;
    case valc::RESULT_CODE_NO_RULES_APPLIED	: s = "NO RULES APPLIED"; break;
    case valc::RESULT_CODE_NULL				: s = "NULL"; break;
    }

    return s;
}
std::string QCViewController::describeWorklistEntry( const valc::WorklistEntry* wle, valc::SnapshotPtr snapshot ) const
{
    std::ostringstream label;

    label
        << "Barcode: " << wle->getBarcode()
        << "    "
        << "Test: " << snapshot->getTestName( wle->getTestID() )
        << "    "
        << "ID: "	<< wle->getID();

    return label.str();
}

IdleServiceUser* QCViewController::getIdleServiceUserInterface()
{
    return &m_idleServiceUser;
}

ModelEventListener* QCViewController::getModelEventListenerInterface()
{
    return &m_eventListener;
}

void QCViewController::init()
{
    if ( m_model->getSelectedWorklistEntry() )
    {
        m_model->borrowSnapshot( update );
    }
}

void QCViewController::notify( int modelEvent, const EventData& eventData )
{
    if (  modelEvent == MODEL_EVENT::WORKLIST_ENTRY_SELECTION_CHANGE )
    {
        m_model->borrowSnapshot( update );
    }
}

void QCViewController::onIdle()
{
}

void QCViewController::onResize()
{
}

void QCViewController::popNode()
{
	require( ! m_nodeStack.empty() );
	m_nodeStack.pop();
}

void QCViewController::pushNode( const std::string& label, bool expand )
{
	TTreeViewItem* newNode{};

	if ( m_nodeStack.empty() )
	{
		newNode = addNodeUnder( m_view->tree, label );
	}
	else
	{
		newNode = addNodeUnder( m_nodeStack.top(), label );
	}

	m_nodeStack.push( newNode );

	if ( expand )
	{
		m_expandList.push_back( newNode );
	}
}

void QCViewController::setModel( Model* m )
{
    m_model = m;
}

void QCViewController::setView( TQCViewFrame* v )
{
    m_view = v;
}

void __fastcall QCViewController::update()
{
	std::stack< TTreeViewItem* > empty;
	m_nodeStack.swap(empty);
	m_expandList.clear();
	m_view->tree->Clear();

    const valc::SnapshotPtr snapshot( m_model->getSnapshot() );

    const valc::WorklistEntry* worklistEntry = findWorklistEntry( m_model->getSelectedWorklistEntry(), snapshot );

    if ( worklistEntry )
    {
        std::unique_ptr<QCVDisplayMode> displayMode;

        if ( isQC( worklistEntry ) )
        {
            displayMode.reset( new QCVDisplayModeQC( this, snapshot, m_model ) );
        }
        else
        {
            displayMode.reset( new QCVDisplayModeUnknown( this, snapshot, m_model ) );
        }

        displayMode->doUpdate();

        for ( TTreeViewItem* item : m_expandList )
        {
            item->Expand();
        }
    }
}

}


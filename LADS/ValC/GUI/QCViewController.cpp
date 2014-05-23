#include "FMXTemplates.h"
#include "Model.h"
#include "ModelEventConstants.h"
#include "QCViewController.h"
#include "QCViewData.h"
#include "Require.h"
#include <sstream>


namespace valcui
{


QCViewController::QCViewController( TQCViewFrame* widgetContainer, Model* m )
	:
	m_widgetContainer( widgetContainer ),
	m_eventListener(this),
    m_model(m)
{
	m->registerModelEventListener( &m_eventListener );
}

void QCViewController::addNodesForControllingQCs( 
    const QCViewData& data, 
    const valc::SnapshotPtr& snapshot, 
    const valc::QCControls& controls )
{
	for ( int i = 0; i < controls.size(); ++i )
	{
		const valc::QCControl& c = controls[i];

        pushNode( describeControl( data, c ), false );

        pushNode( "Result", false );
        pushNode( describeControlResult( data, c ), false );
        popNode();
        popNode();

        pushNode( "Rules applied", false  );
		addNodesForRules( snapshot, c.resultID() );
		popNode();

        popNode();
	}
}

void QCViewController::addNodesForRules( const valc::SnapshotPtr& snapshot, int controlResultID )
{
    valc::RuleResults rr = snapshot->getRuleResults( controlResultID );

    for ( const valc::RuleResult& r : rr )
    {
        pushNode( r.rule, false );
        
        pushNode( describe(r.resultCode), false );
        popNode();

        if ( r.msg.size() )
        {
            pushNode( r.msg, false );
            popNode();
        }

        popNode();
    }
}

std::string QCViewController::describe( valc::ResultCode rc ) const
{
    std::string s;

    switch( rc )
    {
    case valc::RESULT_CODE_FAIL 		   	: s = "Fail"; break;
    case valc::RESULT_CODE_PASS 		   	: s = "Pass"; break;
    case valc::RESULT_CODE_BORDERLINE 	   	: s = "Borderline"; break;
    case valc::RESULT_CODE_ERROR 		   	: s = "Error"; break;
    case valc::RESULT_CODE_NO_RULES_APPLIED	: s = "No Rules Applied"; break;
    case valc::RESULT_CODE_NULL				: s = "Null"; break;
    }

    return s;
}

std::string QCViewController::describeControl( const QCViewData& data, const valc::QCControl& c ) const
{
	// The QCControl gives the result ID.  Try to get hold of the associated WorklistEntry.
	const valc::WorklistEntry* qcWorklistEntry = data.lookupQCWorklistEntry( c.resultID() );

	std::ostringstream label;

	if ( qcWorklistEntry )
	{
		label << qcWorklistEntry->getBarcode() << " (id:" << qcWorklistEntry->getID() << ") ";
	}
	else
	{
		label << "[Worklist entry not found] ";
	}

	label << describe( c.status() );

	return label.str();
}

std::string QCViewController::describeControlResult( const QCViewData& data, const valc::QCControl& c ) const
{
    std::ostringstream s;

    const valc::TestResult* qcResult = data.lookupQCResult( c.resultID() );

    if ( qcResult ) 
    {
        s << qcResult->getResultValue() << " ";
    }
    else
    {
        s << "[Result value not available] ";
    }

    s << "(id:" << c.resultID() << ")";

    return s.str();
}

std::string QCViewController::describeTestResult( const valc::TestResult* r ) const
{
	std::ostringstream label;

	label << r->getResultValue() << " (id:" << r->getID() << ") ";

	valc::ControlStatus cs(r->getControlStatus());

	switch( cs.summaryCode() )
	{
	case valc::CONTROL_STATUS_UNCONTROLLED          : label << "Uncontrolled"; break;
	case valc::CONTROL_STATUS_CONFIG_ERROR_NO_RULES : label << "Config Error No Rules"; break;
	case valc::CONTROL_STATUS_ERROR                 : label << "Error"; break;
	case valc::CONTROL_STATUS_FAIL                  : label << "Fail"; break;
	case valc::CONTROL_STATUS_BORDERLINE            : label << "Borderline"; break;
	case valc::CONTROL_STATUS_PASS                  : label << "Pass"; break;
	}

	return label.str();
}

std::string QCViewController::describeWorklistEntry( const QCViewData& data, const valc::SnapshotPtr& snapshot ) const
{
	std::ostringstream label;

	if ( data.getWorklistEntry() )
	{
		label << data.getWorklistEntry()->getBarcode()
			<< "  "
			<< snapshot->getTestName( data.getWorklistEntry()->getTestID() )
			<< " (id:"
			<< data.getWorklistEntry()->getID()
			<< ")";
	}
	else
	{
		label << "Details for worklist entry "
			<< data.getWorklistEntryID()
			<< " not found.  Perhaps not run on this analyser?";
	}

	return label.str();
}

void QCViewController::notify( int modelEvent )
{
    if (  modelEvent == MODEL_EVENT::WORKLIST_ENTRY_SELECTION_CHANGE )
    {
        m_model->borrowSnapshot( update );
    }
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
		newNode = addNodeUnder( m_widgetContainer->tree, label );
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

void __fastcall QCViewController::update()
{
	std::stack< TTreeViewItem* > empty;
	m_nodeStack.swap(empty);
	m_expandList.clear();
	m_widgetContainer->tree->Clear();

    const valc::SnapshotPtr snapshot( m_model->getSnapshot() );

    const QCViewData data( snapshot, m_model->getSelectedWorklistEntry() );

    pushNode( describeWorklistEntry( data, snapshot ), true );

    if ( data.hasLocalResults() )
    {
        pushNode( "Local results", true );

        for ( auto i = data.localResultBegin(); i != data.localResultEnd(); ++i  )
        {
            const valc::TestResult* r = *i;

            pushNode( describeTestResult(r), true );

            const valc::ControlStatus cs(r->getControlStatus());

            if ( cs.summaryCode() != valc::CONTROL_STATUS_UNCONTROLLED )
            {
                pushNode( "Controlling QCs", true );

                pushNode( "Before", true );
                addNodesForControllingQCs( data, snapshot, cs.precedingQCs() );
                popNode();
                pushNode( "After", true );
                addNodesForControllingQCs( data, snapshot, cs.followingQCs() );
                popNode();

                popNode();
            }

            popNode();
        }

        popNode();
    }

    for ( TTreeViewItem* item : m_expandList )
    {
        item->Expand();
    }
}

}


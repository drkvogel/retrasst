#include "Model.h"
#include "QCVDisplayModeUnknown.h"
#include <sstream>

namespace valcui
{

QCVDisplayModeUnknown::QCVDisplayModeUnknown( QCViewController* c, valc::SnapshotPtr s, valcui::Model* m )
    :
    m_controller(c),
    m_snapshot(s),
    m_model(m),
    m_data( s, m->getSelectedWorklistEntry() )
{
}

void QCVDisplayModeUnknown::addNodesForControllingQCs( const valc::QCControls& controls )
{
    for ( int i = 0; i < controls.size(); ++i )
    {
        const valc::QCControl& c = controls[i];

        m_controller->pushNode( describeControl( c ), false );

        m_controller->pushNode( "Result", false );
        m_controller->pushNode( describeControlResult( c ), false );
        m_controller->popNode();
        m_controller->popNode();

        m_controller->pushNode( "Rules applied", false  );
        addNodesForRules( c.resultID() );
        m_controller->popNode();

        m_controller->popNode();
    }
}

void QCVDisplayModeUnknown::addNodesForRules( int controlResultID )
{
    valc::RuleResults rr = m_snapshot->getRuleResults( controlResultID );

    for ( const valc::RuleResult& r : rr )
    {
        m_controller->pushNode( r.rule, false );
        
        m_controller->pushNode( m_controller->describe(r.resultCode), false );
        m_controller->popNode();

        if ( r.msg.size() )
        {
            m_controller->pushNode( r.msg, false );
            m_controller->popNode();
        }

        m_controller->popNode();
    }
}

std::string QCVDisplayModeUnknown::describeControl( const valc::QCControl& c ) const
{
    // The QCControl gives the result ID.  Try to get hold of the associated WorklistEntry.
    const valc::WorklistEntry* qcWorklistEntry = m_data.lookupQCWorklistEntry( c.resultID() );

    std::ostringstream label;

    if ( qcWorklistEntry )
    {
        label << qcWorklistEntry->getBarcode() << " (id:" << qcWorklistEntry->getID() << ") ";
    }
    else
    {
        label << "[Worklist entry not found] ";
    }

    label << m_controller->describe( c.status() );

    return label.str();
}

std::string QCVDisplayModeUnknown::describeControlResult( const valc::QCControl& c ) const
{
    std::ostringstream s;

    const valc::TestResult* qcResult = m_data.lookupQCResult( c.resultID() );

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


std::string QCVDisplayModeUnknown::describeTestResult( const valc::TestResult* r ) const
{
    std::ostringstream label;

    label << r->getResultValue() << " (id:" << r->getID() << ") ";

        valc::ControlStatus cs(r->getControlStatus());

        switch( cs.summaryCode() )
        {
        case valc::CONTROL_STATUS_UNCONTROLLED          : label << "UNCONTROLLED"; break;
        case valc::CONTROL_STATUS_CONFIG_ERROR_NO_RULES : label << "CONFIG ERROR NO RULES"; break;
        case valc::CONTROL_STATUS_ERROR                 : label << "ERROR"; break;
        case valc::CONTROL_STATUS_FAIL                  : label << "FAIL"; break;
        case valc::CONTROL_STATUS_BORDERLINE            : label << "BORDERLINE"; break;
        case valc::CONTROL_STATUS_PASS                  : label << "PASS"; break;
        }

    return label.str();
}

void QCVDisplayModeUnknown::doUpdate()
{
    m_controller->pushNode( m_controller->describeWorklistEntry(m_data.getWorklistEntry(), m_snapshot), true );

    if ( m_data.hasLocalResults() )
    {
        m_controller->pushNode( "Local results", true );

        for ( auto i = m_data.localResultBegin(); i != m_data.localResultEnd(); ++i  )
        {
            const valc::TestResult* r = *i;

            m_controller->pushNode( describeTestResult( r ), true );

            const valc::ControlStatus cs(r->getControlStatus());

            m_controller->pushNode( "Controlling QCs", true );

            m_controller->pushNode( "Before", true );
            addNodesForControllingQCs( cs.precedingQCs() );
            m_controller->popNode();
            m_controller->pushNode( "After", true );
            addNodesForControllingQCs( cs.followingQCs() );
            m_controller->popNode();

            m_controller->popNode(); // Controlling QCs

            m_controller->popNode(); // describeTestResult
        }

        m_controller->popNode();
    }
}

}


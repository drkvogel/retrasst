#include "Model.h"
#include "QCVDisplayModeQC.h"
#include "Require.h"
#include "SnapshotUtil.h"
#include "StrUtil.h"

namespace valcui
{

QCVDisplayModeQC::QCVDisplayModeQC( QCViewController* c, valc::SnapshotPtr s, valcui::Model* m )
    :
    m_controller(c),
    m_snapshot(s),
    m_model(m),
    m_worklistEntry( findWorklistEntry( m->getSelectedWorklistEntry(), s ) )
{
    require( m_worklistEntry );
}

void QCVDisplayModeQC::doUpdate()
{
    m_controller->pushNode( m_controller->describeWorklistEntry(m_worklistEntry, m_snapshot), true );

    auto results = m_worklistEntry->getTestResults();

    for ( auto result = begin(results); result != end(results); ++result )
    {
        const valc::TestResult* tr = *result;

        const bool isLocal = ! ( tr->getSampleRunID().isNull() );

        if ( isLocal )
        {
            std::unique_ptr<valc::RuleResults> ruleResults;
            std::ostringstream resultLabel;
            resultLabel << "Result ID " << tr->getID();
            m_controller->pushNode( resultLabel.str(), true );
            m_controller->pushNode( "Status", true );
            if ( m_snapshot->hasRuleResults( tr->getID() ) )
            {
                ruleResults.reset( new valc::RuleResults( m_snapshot->getRuleResults( tr->getID() ) ) );
                m_controller->pushNode( m_controller->describe( ruleResults->getSummaryResultCode() ), true );
            }
            else
            {
                m_controller->pushNode( "None available", true );
            }
            m_controller->popNode(); // result code value
            m_controller->popNode(); // 'Summary Result Code'
            m_controller->pushNode( "Rules applied", false );
            if ( ruleResults )
            {
                for ( const valc::RuleResult& r : *ruleResults )
                {
                    m_controller->pushNode( r.rule, false );
                    m_controller->pushNode( "Status", false );
                    m_controller->pushNode( m_controller->describe( r.resultCode ), false );
                    m_controller->popNode();
                    m_controller->popNode();
                    m_controller->pushNode( "Message", false );
                    if ( r.msg.empty() )
                    {
                        m_controller->pushNode( "(None)", false );
                    }
                    else
                    {
                        m_controller->pushNode( r.msg, false );
                    }
                    m_controller->popNode();
                    m_controller->popNode();
                    m_controller->popNode();
                }
            }
            else
            {
                m_controller->pushNode( "None", true );
                m_controller->popNode();
            }
            m_controller->popNode(); // 'Result'
        }
    }
}

}


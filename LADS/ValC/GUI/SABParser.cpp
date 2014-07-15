#include "Nullable.h"
#include "Require.h"
#include "SAB.h"
#include "SABParser.h"
#include "SnapshotUtil.h"

namespace valcui
{

SABParser::SABParser( valc::SnapshotPtr s )
    :
    m_snapshot( s )
{
}

void SABParser::parse( SAB* out )
{
    typedef paulst::Nullable<int> GroupID;

    GroupID currentGroupID; // Initially NULL

    for( auto entry = m_snapshot->localBegin(); entry != m_snapshot->localEnd(); ++entry )
    {
        const valc::LocalRun* localRun = boost::get<valc::LocalRun>( &(*entry) );

        if ( ! localRun )
        {
            continue;
        }

        if ( GroupID( localRun->getGroupID() ) != currentGroupID ) // Change of Group/Batch? 
        {
            out->group( isQC( localRun->getBarcode() ), localRun->getGroupID() );

            currentGroupID = GroupID( localRun->getGroupID() );

            require( GroupID( localRun->getGroupID() ) == currentGroupID );
        }

        out->run( localRun->getRunID(), localRun->getBarcode(), localRun->isOpen() );

        auto worklistEntries = m_snapshot->getWorklistEntries( localRun->getSampleDescriptor() );

        for ( auto w = begin( worklistEntries ); w != end( worklistEntries ); ++w )
        {
            const valc::WorklistEntry* worklistEntry = *w;

            bool worklistEntryHasLocalResults = false;

            auto results = worklistEntry->getTestResults();

            for ( auto r = begin( results ); r != end( results ); ++r )
            {
                const valc::TestResult* testResult = *r;

                if ( testResult->getSampleRunID() == localRun->getRunID() )
                {
                    worklistEntryHasLocalResults = true;

                    typedef paulst::Nullable<valc::RuleResults> ResultEvaluation;

                    const ResultEvaluation resultEvaluation = 
                        m_snapshot->hasRuleResults( testResult->getID() ) ?
                        ResultEvaluation( m_snapshot->getRuleResults( testResult->getID() ) ) :
                        ResultEvaluation();

                    out->result( testResult, resultEvaluation, worklistEntry );
                }
            }

            if ( ( ! worklistEntryHasLocalResults ) && localRun->isOpen() )
            {
                out->pending( worklistEntry );
            }
        }
    }

    out->eof();
}

}


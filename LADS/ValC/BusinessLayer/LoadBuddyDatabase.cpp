#include <algorithm>
#include "Cursor.h"
#include <boost/bind.hpp>
#include <boost/foreach.hpp>
#include <boost/lexical_cast.hpp>
#include "BuddyDatabase.h"
#include "BuddyDatabaseBuilder.h"
#include "BuddySampleIDKeyedOnSampleRunID.h"
#include "DBConnection.h"
#include "DBUpdateSchedule.h"
#include <iterator>
#include "LoadBuddyDatabase.h"
#include "LoggingService.h"
#include "SampleRun.h"
#include "SampleRunIDResolutionService.h"
#include "StringBuilder.h"

namespace valc
{

LoadBuddyDatabase::LoadBuddyDatabase( 
    int                                     localMachineID, 
    paulstdb::DBConnection*                 con, 
	paulst::LoggingService*                 log, 
    ResultIndex*                            resultIndex, 
    Projects*                               projects,
	BuddyDatabase**                         bd, 
    DBUpdateSchedule*                       dbUpdateSchedule,
    SampleRunIDResolutionService*           sampleRunIDResolutionService,
    const std::string&                      sql,
    const std::string&                      inclusionRule,
    ExceptionalDataHandler*                 exceptionalDataHandler,
    RuleEngineContainer*                    ruleEngine,
    QCSampleDescriptorDerivationStrategy*   qcsdds,
    BuddyDatabaseEntryIndex*                bdei )
    :
    m_localMachineID                        ( localMachineID ),
    m_projects                              ( projects ),
    m_con                                   ( con ),
    m_resultIndex                           ( resultIndex ),
    m_log                                   ( log ),
    m_buddyDatabase                         ( bd ),
    m_dbUpdateSchedule                      ( dbUpdateSchedule ),
    m_sampleRunIDResolutionService          ( sampleRunIDResolutionService ),
    m_sql                                   ( sql ),
    m_inclusionRule                         ( inclusionRule ),
    m_exceptionalDataHandler                ( exceptionalDataHandler ),
    m_ruleEngine                            ( ruleEngine ),
    m_QCSampleDescriptorDerivationStrategy  ( qcsdds ),
    m_buddyDatabaseEntryIndex               ( bdei )
{
}

class ExistsAnOpenSampleRunMatchingOnSampleDescriptor
{
private:
    const SampleRuns& m_existing;
    SampleRunIDResolutionService& m_sampleRunIDResolutionService;
public:
    ExistsAnOpenSampleRunMatchingOnSampleDescriptor( const SampleRuns& existing, SampleRunIDResolutionService& out ) 
        : 
        m_existing(existing),
        m_sampleRunIDResolutionService(out)
    {
    }

    bool operator()(const SampleRun& candidate ) 
    {
        BOOST_FOREACH( const SampleRun& existing, m_existing )
        {
            if  (   ( existing.getSampleDescriptor() == candidate.getSampleDescriptor() ) && existing.isOpen()    )
            {
                m_sampleRunIDResolutionService.addMapping( candidate.getID(), existing.getID() );
                return true;
            }
        }
        return false;
    }
};

int compareRuns( const SampleRun& a, const SampleRun& b )
{
    return a.getID() < b.getID();
}

int compareOnSequencePosition( const SampleRun& a, const SampleRun& b )
{
    return a.getSequencePosition() < b.getSequencePosition();
}

bool equivalentRuns( const SampleRun& a, const SampleRun& b )
{
    return  ( ! compareRuns( a, b ) ) && 
            ( ! compareRuns( b, a ) );
}

void LoadBuddyDatabase::doStuff()
{
    /*  sampleRuns:             a list of SampleRun instances each of which represents a row in the sample_run table
        candidateSampleRuns:    these do NOT exist in the sample_run table, but buddy_database activity suggests they are needed

        Note that both are LISTS. There is nothing to stop them including duplicates.
    */
    std::auto_ptr<SampleRuns>                      sampleRuns             ( new SampleRuns()), 
                                                   candidateSampleRuns    ( new SampleRuns());
    std::auto_ptr<BuddySampleIDKeyedOnSampleRunID> buddySampleIDKeyedOnSampleRunID
                                                                          ( new BuddySampleIDKeyedOnSampleRunID(m_sampleRunIDResolutionService));

    BuddyDatabaseBuilder builder(m_projects, m_resultIndex, sampleRuns.get(), candidateSampleRuns.get(), m_sampleRunIDResolutionService,
        m_dbUpdateSchedule, buddySampleIDKeyedOnSampleRunID.get(), m_buddyDatabaseEntryIndex, m_inclusionRule, m_exceptionalDataHandler,
        m_ruleEngine, m_log, m_QCSampleDescriptorDerivationStrategy );

    for ( std::auto_ptr<paulstdb::Cursor> cursor( m_con->executeQuery( m_sql ) ); 
            ( ! cursor->endOfRecordSet() ) && builder.accept( cursor.get() ); cursor->next() );
   
    // Remove duplicates from sampleRuns
	std::sort( sampleRuns->begin(), sampleRuns->end(), compareRuns );
	LOG( std::string("Number of sample-runs before deduplication: ") << sampleRuns->size() );
	SampleRuns::iterator sampleRunsEnd = std::unique( sampleRuns->begin(), sampleRuns->end(), equivalentRuns );
	sampleRuns->resize( std::distance( sampleRuns->begin(), sampleRunsEnd ) );
	LOG( std::string("Number of sample-runs after deduplication: ") << sampleRuns->size() );
    
	// Remove duplicates from candidateSampleRuns
	LOG( std::string("Number of candidate-sample-runs before deduplication: ") << candidateSampleRuns->size() );
	std::sort( candidateSampleRuns->begin(), candidateSampleRuns->end(), compareRuns );

	SampleRuns::iterator candidatesEnd = std::unique( candidateSampleRuns->begin(), candidateSampleRuns->end(), equivalentRuns );
	LOG( std::string("Number of candidate-sample-runs after deduplication: ") <<
	std::distance( candidateSampleRuns->begin(), candidatesEnd ) );

    // Remove candidates for which there already exists an open sample-run for the same sample.
    // Note that a side-effect of this procedure is that sampleRunIDResolutionService may gain 
    // mappings, in order to avoid TestResult instances having dangling references to removed candidate-sample runs.
    candidatesEnd = std::remove_if( candidateSampleRuns->begin(), candidatesEnd, 
        ExistsAnOpenSampleRunMatchingOnSampleDescriptor( *sampleRuns, *m_sampleRunIDResolutionService ) );

    // Remaining candidates represent genuine sample-runs that don't yet exist in the sample_run table. Add them to sampleRuns.
    std::copy( candidateSampleRuns->begin(), candidatesEnd, std::back_inserter( *sampleRuns ) );

    // Sort sampleRuns on sequence position
    std::sort( sampleRuns->begin(), sampleRuns->end(), compareOnSequencePosition );

    LOG( std::string("Number of sample-runs: ") << sampleRuns->size() );

	*m_buddyDatabase = new BuddyDatabase( m_localMachineID, sampleRuns.release(), buddySampleIDKeyedOnSampleRunID.release(),
        m_buddyDatabaseEntryIndex );
}

}


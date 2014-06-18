#include <algorithm>
#include "Cursor.h"
#include <boost/bind.hpp>
#include <boost/foreach.hpp>
#include <boost/lexical_cast.hpp>
#include "BuddyDatabase.h"
#include "BuddyDatabaseBuilder.h"
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

int compareOnSequencePosition( const SampleRun& a, const SampleRun& b )
{
    return a.getSequencePosition() < b.getSequencePosition();
}

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
    BuddyDatabaseEntryIndex*                bdei,
    const ControlModel*                     controlModel )
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
    m_buddyDatabaseEntryIndex               ( bdei ),
    m_controlModel                          ( controlModel )
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
        for ( const SampleRun& existing : m_existing )
        {
            if  (   ( existing.getSampleDescriptor() == candidate.getSampleDescriptor() ) && existing.isOpen()    )
            {
                m_sampleRunIDResolutionService.addMapping( candidate.getID().token(), existing.getID().token() );
                return true;
            }
        }
        return false;
    }
};


void LoadBuddyDatabase::doStuff()
{
    /*  sampleRuns:             a list of SampleRun instances each of which represents a row in the sample_run table
        candidateSampleRuns:    these do NOT exist in the sample_run table, but buddy_database activity suggests they are needed
    */
    std::auto_ptr<SampleRuns>                      sampleRuns             ( new SampleRuns()), 
                                                   candidateSampleRuns    ( new SampleRuns());

    BuddyDatabaseBuilder builder(m_projects, m_resultIndex, sampleRuns.get(), candidateSampleRuns.get(), m_sampleRunIDResolutionService,
        m_dbUpdateSchedule, m_buddyDatabaseEntryIndex, m_inclusionRule, m_exceptionalDataHandler,
        m_ruleEngine, m_log, m_QCSampleDescriptorDerivationStrategy, m_controlModel );

    for ( std::auto_ptr<paulstdb::Cursor> cursor( m_con->executeQuery( m_sql ) ); 
            ( ! cursor->endOfRecordSet() ) && builder.accept( cursor.get() ); cursor->next() );
   
    // Remove candidates for which there already exists an open sample-run for the same sample.
    // Note that a side-effect of this procedure is that sampleRunIDResolutionService may gain 
    // mappings, in order to avoid TestResult instances having dangling references to removed candidate-sample runs.
    SampleRuns::iterator candidatesEnd = std::remove_if( candidateSampleRuns->begin(), candidateSampleRuns->end(), 
        ExistsAnOpenSampleRunMatchingOnSampleDescriptor( *sampleRuns, *m_sampleRunIDResolutionService ) );

    m_dbUpdateSchedule->queueSampleRunInsertions( candidateSampleRuns->begin(), candidatesEnd );

    // Remaining candidates represent genuine sample-runs that don't yet exist in the sample_run table. Add them to sampleRuns.
    std::copy( candidateSampleRuns->begin(), candidatesEnd, std::back_inserter( *sampleRuns ) );

    // Sort sampleRuns on sequence position
    std::sort( sampleRuns->begin(), sampleRuns->end(), compareOnSequencePosition );

    LOG( std::string("Number of sample-runs: ") << sampleRuns->size() );

	*m_buddyDatabase = new BuddyDatabase( m_localMachineID, sampleRuns.release(), m_buddyDatabaseEntryIndex );
}

}


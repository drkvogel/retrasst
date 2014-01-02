#ifndef SNAPSHOTTESTFIXTUREH
#define SNAPSHOTTESTFIXTUREH

#include "API.h"
#include "ConsoleWriter.h"
#include "LoggingService.h"
#include "MockConfig.h"
#include "NoLogging.h"
#include <string>
#include <vector>

const valc::TestResult* testResultFor( const valc::WorklistEntry* wle )
{
    using namespace valc;
    Range<TestResultIterator> resultRange = wle->getTestResults();
    unsigned int numResults = std::distance( resultRange.first, resultRange.second );
    if (  numResults != 1 )
    {
        std::string testIDs;
        if ( numResults )
        {
            testIDs = " .TestResult IDs: ";

            for ( TestResultIterator i = resultRange.first; i != resultRange.second; ++i )
            {
                testIDs.append( AnsiString( (*i)->getID() ).c_str() );
                testIDs.append(",");
            }
        }
        char buffer[1024];
        std::sprintf( buffer, "Worklist entry %d has %d test result(s)%s", wle->getID(), numResults, testIDs.c_str() );
        throw Exception( buffer );
    }
    return *(resultRange.first);
}

const valc::WorklistEntry* worklistEntry( valc::Range<valc::WorklistEntryIterator>& worklistEntries, int id )
{
    using namespace valc;
    const WorklistEntry* wle = 0;
    for ( WorklistEntryIterator i = worklistEntries.first; i != worklistEntries.second; ++i )
    {
        if ( (*i)->getID() == id )
        {
            wle = *i;
            break;
        }
    }

    if ( wle == 0 )
    {
        throw Exception( "Failed to find expected worklist entry." );
    }

    return wle;
}

class UserWarnings : public valc::UserAdvisor
{
public:
    std::vector< std::string > messages;

    UserWarnings() {}

    // NB: might want some concurrency protection here!
    void advise( const std::string& msg ) { messages.push_back( msg ); }
};


class SnapshotTestFixture
{
public:
    valc::SnapshotPtr s;
    valc::MockConfig                config;
    UserWarnings                    userWarnings;

    SnapshotTestFixture( int localMachineID, int userID, bool initialise = false, bool suppressLogMessages = true )
        : m_localMachineID( localMachineID ), m_userID( userID ), m_log(0), m_logToConsole( ! suppressLogMessages )
    {
        if ( initialise )
        {
            init();
        }
    }

    void init()
    {
        paulst::Writer* logWriter = 0;

        if ( m_logToConsole )
        {
            logWriter = new paulst::ConsoleWriter();
        }
        else
        {
            logWriter = new NoLogging();
        }
        m_log = new paulst::LoggingService( logWriter );
        valc::InitialiseApplicationContext( m_localMachineID, m_userID, config.toString(), m_log, &userWarnings );
        s   = valc::Load();
    }
    
    ~SnapshotTestFixture()
    {
        valc::DeleteApplicationContext();
        delete m_log;
    }

    valc::SnapshotPtr get() const
    {
        return s;
    }

private:
    const int               m_userID;
    const int               m_localMachineID;
    const bool              m_logToConsole;
    paulst::LoggingService* m_log;
};

#endif


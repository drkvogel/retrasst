#include <cstdio>
#include "Cursor.h"
#include "DBConnection.h"
#include "ExceptionalDataHandler.h"
#include "LoadNonLocalResults.h"
#include "LoadWorklistEntries.h"
#include "LoggingService.h"
#include <memory>
#include "Projects.h"
#include "ResultIndex.h"
#include "StringBuilder.h"
#include "TestResultImpl.h"

namespace valc
{

LoadNonLocalResults::LoadNonLocalResults(
        const Projects*         projects,
        paulstdb::DBConnection* con, 
        paulst::LoggingService* log,
        ResultIndex*            resultIndex,
        const std::string&      sql,
        ExceptionalDataHandler* exceptionalDataHandler )
    :
    m_projects              ( projects ),
    m_con                   ( con ),
    m_log                   ( log ),
    m_resultIndex           ( resultIndex ),
    m_sql                   ( sql ),
    m_exceptionalDataHandler( exceptionalDataHandler )
{
}

void LoadNonLocalResults::doStuff()
{
    int numLoaded = 0;

    for ( std::auto_ptr<paulstdb::Cursor> cursor( m_con->executeQuery(m_sql) ); ! cursor->endOfRecordSet(); cursor->next() )
    {
        if ( loadResult( *cursor ) )
        {
            ++numLoaded;
        }
    }

    std::string logMsg = std::string("Loaded ") << numLoaded << " non-local results.";
    LOG( logMsg );    
}

bool LoadNonLocalResults::loadResult( paulstdb::Cursor& c )
{
    bool loaded = false;

    do
    {
        using namespace paulstdb;

        const std::string barcode         = read<std::string> ( c, 0 );
        const int         machineID       = read<int>         ( c, 1 );
        const int         alphaSampleID   = read<int>         ( c, 2 );
        const std::string databaseName    = read<std::string> ( c, 3 );
        const int         buddyResultID   = read<int>         ( c, 4 );
        const int         testID          = read<int>         ( c, 5 );
        const float       resValue        = read<float>       ( c, 6 );
        const char        actionFlag      = read<char>        ( c, 7 );
        const TDateTime   dateAnalysed    = read<TDateTime>   ( c, 8 );
        const std::string resText         = read<std::string> ( c, 9 );
        const TDateTime   updateWhen      = read<TDateTime>   ( c, 10 );
        const int         cbwRecordNo     = read<int>         ( c, 11 );
        const int         cbwProjectID    = read<int>         ( c, 12 );

        std::string sampleDescriptor;

        if ( barcode.find( "QC" ) == 0U )
        {
            sampleDescriptor = barcode << "/" << machineID;
        }
        else
        {
            if ( ( 0 == alphaSampleID ) || ( 0 == cbwProjectID ) )
            {
                m_exceptionalDataHandler->notifyWorklistEntryIgnored( cbwRecordNo, 
                    "If the barcode doesn't start with 'QC', then neither alpha_sample_id nor project_cid can be zero." );
                break;
            }

            sampleDescriptor = std::string() << alphaSampleID << "/" << cbwProjectID;
        }

        // Not a real sampleRunID. Distinct enough, in form, from local sample-run IDs that 
        // a non-local result will never look like it originates from a local run.
        std::string sampleRunID = std::string("NonLocalResult") << buddyResultID; 

        m_resultIndex->addIndexEntryForResult( 
            new TestResultImpl( actionFlag, sampleDescriptor, dateAnalysed, machineID, buddyResultID, sampleRunID, testID, resValue, resText ) );

        m_resultIndex->allocateResultToWorklistEntry( buddyResultID, cbwRecordNo );

        loaded = true;
    }
    while( false );

    return loaded;
}

}


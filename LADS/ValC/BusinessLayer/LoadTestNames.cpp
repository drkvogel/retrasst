#include "Cursor.h"
#include "DBConnection.h"
#include "LoadTestNames.h"
#include "LoggingService.h"
#include "Trace.h"


namespace valc
{

LoadTestNames::LoadTestNames( TestNames* testNames, paulst::LoggingService* log, paulstdb::DBConnection* con )
    :
    m_log(log),
    m_con(con),
    m_testNames(testNames)
{
    trace( "LoadTestNames constructor" );
}

LoadTestNames::~LoadTestNames()
{
    trace( "LoadTestNames destructor" );
}

int LoadTestNames::releaseReturnValue()
{
    return 1;
}

void LoadTestNames::execute()
{
    std::string sql("SELECT test_cid, external_name FROM c_test");

    for ( std::auto_ptr<paulstdb::Cursor> cursor( m_con->executeQuery( sql ) ); ! cursor->endOfRecordSet(); cursor->next() )
    {
        int         testID = 0;
        std::string externalName;

        cursor->read( 0, testID );
        cursor->read( 1, externalName );

        m_testNames->insert( std::make_pair( testID, externalName ) );
    }

    m_log->log( std::string("Loaded ") <<  m_testNames->size() << " test names." );
}

}


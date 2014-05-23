#include "AbstractConnectionFactory.h"
#include <boost/scoped_ptr.hpp>
#include "Config.h"
#include "Cursor.h"
#include "DBConnection.h"
#include "ExceptionUtil.h"
#include "SampleRunGroupIDGeneratorUsingDB.h"
#include "ResourceHandle.h"

namespace valc
{

SampleRunGroupIDGeneratorUsingDB::SampleRunGroupIDGeneratorUsingDB( 
    paulstdb::AbstractConnectionFactory* connectionFactory, 
    paulst::Config* config )
    :
    m_connectionFactory     ( connectionFactory ),
    m_connectionString      ( config->get( "SampleRunGroupIDConnectionString") ),
    m_sessionReadLockSetting( config->get( "SampleRunGroupIDSessionReadLockSetting") ),
    m_sqlQuery              ( config->get( "SampleRunGroupIDQueryString" ) )
{
}

int SampleRunGroupIDGeneratorUsingDB::nextID()
{
    paulst::ResourceHandle<paulstdb::DBConnection> con( 
        m_connectionFactory->createConnection( m_connectionString, m_sessionReadLockSetting ) );

    boost::scoped_ptr<paulstdb::Cursor> cursor( con->executeQuery( m_sqlQuery ) );

    if ( cursor->endOfRecordSet() )
    {
        paulst::exception( "No rows obtained for '%s'", m_sqlQuery.c_str() );
    }

    return paulstdb::read<int>( *cursor, 0 );
}

}


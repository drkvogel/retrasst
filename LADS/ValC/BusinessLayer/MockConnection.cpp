#include <algorithm>
#include <boost/bind.hpp>
#include "MockConnection.h"
#include "MockConnectionFactory.h"
#include "StringBackedCursor.h"
#include "StrUtil.h"

namespace valc
{

std::vector< std::string> MockConnection::m_updateStmts;

MockConnection::MockConnection()
{
    m_updateStmts.clear();
}

void MockConnection::close()
{
}

paulstdb::Cursor* MockConnection::executeQuery( const std::string& sql )
{
    SerializedRecordset sr = MockConnectionFactory::findRecordSetForQuery( sql );
   
    return new paulstdb::StringBackedCursor( sr.data, sr.fieldParsingStrategy );
}

void MockConnection::executeStmt ( const std::string& sql )
{
    m_updateStmts.push_back( sql );
}

int MockConnection::totalNewResult2WorklistLinks()
{
    return std::count_if( m_updateStmts.begin(), m_updateStmts.end(), boost::bind( paulst::ifind, "set cbw_record_no", _1 ) );
}

int MockConnection::totalNewSampleRuns()
{
    return std::count_if( m_updateStmts.begin(), m_updateStmts.end(), boost::bind( paulst::ifind, "into sample_run", _1 ) );
}

int MockConnection::totalUpdatesForSampleRunIDOnBuddyDatabase()
{
    return std::count_if( m_updateStmts.begin(), m_updateStmts.end(), boost::bind( paulst::ifind, "set sample_run_id", _1 ) );
}

}


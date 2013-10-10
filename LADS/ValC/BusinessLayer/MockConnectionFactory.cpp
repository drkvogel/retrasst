#include "MockConnection.h"
#include "MockConnectionFactory.h"

namespace valc
{

std::string MockConnectionFactory::clusters;
std::string MockConnectionFactory::projects;
std::string MockConnectionFactory::worklist;
std::string MockConnectionFactory::buddyDB;
std::string MockConnectionFactory::testNames;
std::string MockConnectionFactory::nonLocalResults;

MockConnectionFactory::MockConnectionFactory()
{
}

paulstdb::DBConnection* MockConnectionFactory::createConnection( const std::string& connectionString, const std::string& sessionReadLockSetting )
{
    return new MockConnection();
}

void MockConnectionFactory::reset()
{
    projects = worklist = buddyDB = testNames = nonLocalResults = "";
}

}


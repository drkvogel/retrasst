#include "MockConnection.h"
#include "MockConnectionFactory.h"

namespace valc
{

SerializedRecordset::SerializedRecordset( const std::string& dataString, paulstdb::FieldParsingStrategy fps )
    :
    data( dataString ),
    fieldParsingStrategy( fps )
{
}

SerializedRecordset::SerializedRecordset( const char* dataString, paulstdb::FieldParsingStrategy fps )
    :
    data( dataString ),
    fieldParsingStrategy( fps )
{
}

SerializedRecordset::SerializedRecordset( const SerializedRecordset& other )
    :
    data( other.data ),
    fieldParsingStrategy( other.fieldParsingStrategy )
{
}

SerializedRecordset& SerializedRecordset::operator=( const SerializedRecordset& other )
{
    data = other.data;
    fieldParsingStrategy = other.fieldParsingStrategy;
    return *this;
}

SerializedRecordset MockConnectionFactory::clusters;
SerializedRecordset MockConnectionFactory::projects;
SerializedRecordset MockConnectionFactory::worklist;
SerializedRecordset MockConnectionFactory::buddyDB;
SerializedRecordset MockConnectionFactory::testNames;
SerializedRecordset MockConnectionFactory::nonLocalResults;
SerializedRecordset MockConnectionFactory::ruleConfig;
SerializedRecordset MockConnectionFactory::rules;

MockConnectionFactory::MockConnectionFactory()
{
}

paulstdb::DBConnection* MockConnectionFactory::createConnection( const std::string& connectionString, const std::string& sessionReadLockSetting )
{
    return new MockConnection();
}

void MockConnectionFactory::reset()
{
    projects = worklist = buddyDB = testNames = nonLocalResults = ruleConfig = "";
    rules = SerializedRecordset();
}

}


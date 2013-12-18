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

MockConnectionFactory::RecordSetsKeyedOnQueryKeyword MockConnectionFactory::s_recordsetsKeyedOnQueryKeyword;

MockConnectionFactory::MockConnectionFactory()
{
}

paulstdb::DBConnection* MockConnectionFactory::createConnection( const std::string& connectionString, const std::string& sessionReadLockSetting )
{
    return new MockConnection();
}

SerializedRecordset MockConnectionFactory::findRecordSetForQuery( const std::string& sql )
{
    SerializedRecordset recordset;

    for ( RecordSetsKeyedOnQueryKeyword::const_iterator i = s_recordsetsKeyedOnQueryKeyword.begin();
            i != s_recordsetsKeyedOnQueryKeyword.end(); ++i )
    {
        const std::string queryKeyword = i->first;

        if ( paulst::ifind( queryKeyword, sql ) )
        {
            recordset = i->second;
            break;
        }
    }

    return recordset;
}

void MockConnectionFactory::prime( const std::string& queryKeywordCaseInsensitive, const SerializedRecordset& recordset )
{
    s_recordsetsKeyedOnQueryKeyword.insert( std::make_pair( queryKeywordCaseInsensitive, recordset ) );
}


void MockConnectionFactory::prime( WellKnownQuery query, const SerializedRecordset& recordset )
{
    std::string keyword;

    switch( query )
    {
    case CLUSTERS_QRY       : keyword = "LoadClusterIDs"        ; break;
    case PROJECTS_QRY       : keyword = "LoadProjects"          ; break;
    case WORKLIST_QRY       : keyword = "LoadWorklistEntries"   ; break;
    case BUDDYDB_QRY        : keyword = "LoadBuddyDatabase"     ; break;
    case TESTNAMES_QRY      : keyword = "LoadTestNames"         ; break;
    case NONLOCALRESULTS_QRY: keyword = "LoadNonLocalResults"   ; break;
    case RULECONFIG_QRY     : keyword = "LoadRuleConfig"        ; break;
    case RULES_QRY          : keyword = "LoadRules"             ; break;
    case SAMPLERUNID_QRY    : keyword = "sample_run_id.nextval" ; break;
    default:
        throw Exception( L"Not a well-known query!" );
    }

    prime( keyword, recordset );
}

void MockConnectionFactory::reset()
{
    s_recordsetsKeyedOnQueryKeyword.clear();
    MockConnectionFactory::prime( SAMPLERUNID_QRY, "1,\n" );
}

}


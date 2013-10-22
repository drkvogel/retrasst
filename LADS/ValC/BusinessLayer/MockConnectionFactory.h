#ifndef MOCKCONNECTIONFACTORYH
#define MOCKCONNECTIONFACTORYH

#include "AbstractConnectionFactory.h"
#include <string>
#include "StringBackedCursor.h"

namespace valc
{

class MockConnection;

struct SerializedRecordset
{
    std::string data;
    paulstdb::FieldParsingStrategy fieldParsingStrategy;

    SerializedRecordset( const std::string& dataString, paulstdb::FieldParsingStrategy fps = paulstdb::parseCSV );
    SerializedRecordset( const char* dataString  = "", paulstdb::FieldParsingStrategy fps = paulstdb::parseCSV );
    SerializedRecordset( const SerializedRecordset& other );
    SerializedRecordset& operator=( const SerializedRecordset& other );
};

/*
ConnectionFactory from which can be obtained connections which run 
their queries against the data strings specified using the 
setXYZ methods.

Data strings use a '\n' to separate distinct records and use a 
comma to separate distinct column values. An empty string is treated 
as a NULL.  Note that there must be a trailing comma.

An example:

"-832455,reveal,ldb25,\n"
"-1015160,STICS,ldb26,\n"

*/
class MockConnectionFactory : public paulstdb::AbstractConnectionFactory
{
public:
    MockConnectionFactory();
    paulstdb::DBConnection* createConnection( const std::string& connectionString, const std::string& sessionReadLockSetting );
    static void reset();
    /* Example sql:
    
     select first 2 wl.record_no, machine_cid, barcode, test_cid, group_id, category_id, sample_id, project_cid,profile_id, profile_name,
     time_stamp, ts_sequence, status, diluent, buddy_result_id
     from valc_worklist
    */
    static SerializedRecordset clusters,
                projects,
                worklist,
                buddyDB,
                testNames,
                nonLocalResults,
                ruleConfig,
                rules;
private:

    MockConnectionFactory( const MockConnectionFactory& );
    MockConnectionFactory& operator=( const MockConnectionFactory& );

};

};

#endif


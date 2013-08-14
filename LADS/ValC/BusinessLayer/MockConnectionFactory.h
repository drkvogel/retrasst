#ifndef MOCKCONNECTIONFACTORYH
#define MOCKCONNECTIONFACTORYH

namespace valc
{

class MockConnection;

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
class MockConnectionFactory
{
public:
    MockConnectionFactory();
    MockConnection* createConnection();
    void setClusters( const std::string& serializedClusters );
    void setBuddyDB ( const std::string& serializedBuddyDB );
    void setProjects( const std::string& serializedProjects );
    void setTestNames( const std::string& serializedTestNames );
    /* Example sql:
    
     select first 2 wl.record_no, machine_cid, barcode, test_cid, group_id, category_id, sample_id, project_cid,profile_id, profile_name,
     time_stamp, ts_sequence, status, diluent, buddy_result_id
     from valc_worklist
    */
    void setWorklist( const std::string& serializedWorklist );
private:
    std::string m_clusters,
                m_projects,
                m_worklist,
                m_buddyDB,
                m_testNames;

    MockConnectionFactory( const MockConnectionFactory& );
    MockConnectionFactory& operator=( const MockConnectionFactory& );
};

};

#endif


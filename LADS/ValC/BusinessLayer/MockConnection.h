#ifndef MOCKCONNECTIONH
#define MOCKCONNECTIONH

#include "API.h"
#include <vector>

namespace valc
{

class MockConnection : public DBConnection
{
public:
    MockConnection();
    void close();
    Cursor* executeQuery        ( const std::string& sql );
    void    executeStmt         ( const std::string& sql );
    void    setBuddyDB          ( const std::string& buddyDB   );
    void    setClusters         ( const std::string& clusters  );
    void    setNonLocalResults  ( const std::string& nonLocalResults );
    void    setProjects         ( const std::string& projects  );
    void    setTestNames        ( const std::string& testNames );
    void    setWorklist         ( const std::string& worklist  );
    int     totalNewResult2WorklistLinks                () const;
    int     totalNewSampleRuns                          () const;
    int     totalUpdatesForSampleRunIDOnBuddyDatabase   () const;
private:
    std::string m_clusters,
                m_projects,
                m_worklist,
                m_buddyDB,
                m_testNames,
                m_nonLocalResults;

    std::vector< std::string > m_updateStmts;
};

};

#endif


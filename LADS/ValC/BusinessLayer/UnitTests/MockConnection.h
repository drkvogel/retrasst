#ifndef MOCKCONNECTIONH
#define MOCKCONNECTIONH

#include "API.h"

namespace valc
{

class MockConnection : public DBConnection
{
public:
    MockConnection();
    void close();
    Cursor* executeQuery( const std::string& sql );
    void    executeStmt ( const std::string& sql );
    void    setBuddyDB  ( const std::string& buddyDB  );
    void    setClusters ( const std::string& clusters );
    void    setProjects ( const std::string& projects );
    void    setWorklist ( const std::string& worklist );
private:
    std::string m_clusters,
                m_projects,
                m_worklist,
                m_buddyDB;
};

};

#endif


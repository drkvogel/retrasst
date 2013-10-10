#ifndef MOCKCONNECTIONH
#define MOCKCONNECTIONH

#include "DBConnection.h"
#include <vector>

namespace valc
{

class MockConnection : public paulstdb::DBConnection
{
public:
    MockConnection();
    void                close();
    paulstdb::Cursor*   executeQuery        ( const std::string& sql );
    void                executeStmt         ( const std::string& sql );

    static int     totalNewResult2WorklistLinks                ();
    static int     totalNewSampleRuns                          ();
    static int     totalUpdatesForSampleRunIDOnBuddyDatabase   ();
private:
    static std::vector< std::string > m_updateStmts;

};

};

#endif


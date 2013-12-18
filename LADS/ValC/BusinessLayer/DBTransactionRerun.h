#ifndef DBTRANSACTIONRERUNH
#define DBTRANSACTIONRERUNH

#include <boost/bind.hpp>
#include <boost/shared_ptr.hpp>
#include "Config.h"
#include "Cursor.h"
#include "DBConnection.h"
#include "DBTransactionUtil.h"
#include "StrUtil.h"

namespace valc
{

class DBTransactionRerun
{
public:
    int worklistID;
    int newWorklistID;

    std::string describe() const
    {
        return "nowt to say, really";
    }

    void execute( paulstdb::DBConnection* c, const paulst::Config* config )
    {
        setAutoCommit( c, false );

        boost::shared_ptr<void> restoreAutoCommitOnBlockExit( c, boost::bind( setAutoCommit, _1, true ) );

        try
        {
            paulstdb::Cursor* cursor = c->executeQuery( config->get( "SQLStmtNextWorklistID" ) );
            require( *cursor );
            newWorklistID = paulstdb::read<int>( *cursor, 0 );
            cursor->close();
            c->executeStmt( paulst::format( config->get( "SQLStmtInsertRerun"            ).c_str(), newWorklistID, worklistID ) );
            c->executeStmt( paulst::format( config->get( "SQLStmtInsertWorklistRelation" ).c_str(), newWorklistID, worklistID ) );
            commit( c );
        }
        catch( ... )
        {
            rollback( c );
            throw;
        }
    }
};

}

#endif


#ifndef DBTRANSACTIONRERUNH
#define DBTRANSACTIONRERUNH

#include <boost/bind.hpp>
#include <boost/shared_ptr.hpp>
#include "Config.h"
#include "Cursor.h"
#include "DBConnection.h"
#include "DBTransactionUtil.h"
#include "ExceptionUtil.h"
#include <memory>
#include "StrUtil.h"

namespace valc
{

class DBTransactionRerun
{
public:
    int worklistID;
    volatile int newWorklistID;
    IDToken sampleRunID;
    int userID;
    std::string errorMsg;


    std::string describe() const
    {
        return paulst::format("Rerun-insertion on behalf of user %d.  "
            "Involves a) closing-off sample-run %s; b) inserting new worklist entry as a chid of entry %d.",
            userID, sampleRunID.value().c_str(), worklistID );
    }

    void execute( paulstdb::DBConnection* c, const paulst::Config* config )
    {
        setAutoCommit( c, false );

        boost::shared_ptr<void> restoreAutoCommitOnBlockExit( c, boost::bind( setAutoCommit, _1, true ) );

        try
        {
            reserveIDValueForNewWorklistEntry( c, config );
            confirmOKToCloseSampleRun( c, config );
            const int intSampleRunID = paulst::toInt( sampleRunID.value() );
            c->executeStmt( paulst::format( config->get( "SQLStmtCloseSampleRun"         ).c_str(), userID, intSampleRunID    ) );
            c->executeStmt( paulst::format( config->get( "SQLStmtInsertRerun"            ).c_str(), newWorklistID, worklistID ) );
            c->executeStmt( paulst::format( config->get( "SQLStmtInsertWorklistRelation" ).c_str(), newWorklistID, worklistID ) );
            commit( c );
        }
        catch( const Exception& e )
        {
            errorMsg = AnsiString( e.Message.c_str() ).c_str();   
            rollback( c );
            throw;
        }
        catch( ... )
        {
            errorMsg = "Unspecified error";
            rollback( c );
            throw;
        }
    }

private:

    void confirmOKToCloseSampleRun( paulstdb::DBConnection* c, const paulst::Config* config )
    {
        const int intSampleRunID = paulst::toInt( sampleRunID.value() );
        std::auto_ptr<paulstdb::Cursor> cursor( c->executeQuery( paulst::format( config->get( "QueryForSampleRun" ).c_str(), intSampleRunID ) ) );

        require( *cursor );

        int sampleRunEntryFound = paulstdb::read<int>( *cursor, 0 );

        cursor->close();

        if ( ! sampleRunEntryFound )
        {
            paulst::exception( "Failed to find entry %d in buddy_sample_run", intSampleRunID );
        }
    }

    void reserveIDValueForNewWorklistEntry( paulstdb::DBConnection* c, const paulst::Config* config )
    {
        std::auto_ptr<paulstdb::Cursor> cursor( c->executeQuery( config->get( "SQLStmtNextWorklistID" ) ) );
        require( *cursor );
        newWorklistID = paulstdb::read<int>( *cursor, 0 );
        cursor->close();
    }
};

}

#endif


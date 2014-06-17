#include "MockConfig.h"

namespace valc
{

MockConfig::MockConfig()
{
    addProperty( "CacheConnections", "false" );
    addProperty( "BuddyDatabaseSampleRunIDUpdateSQL", "update buddy_database set sample_run_id = %d where buddy_sample_id = %d" );
    addProperty( "LinkResultToWorklistEntryUpdateSQL", "update buddy_result_float set cbw_record_no = %d where buddy_result_id = %d" );
    addProperty( "SampleRunIDQueryString"                       , "select sample_run_id.nextval"  );
    addProperty( "SampleRunInsertSQL"                 , "insert into sample_run ( run_id, sequence_position, group_id ) values ( %d, %d, %d )" );
    addProperty( "SampleRunGroupIDConnectionString"             , "dsn=paulst_brat_64;db=paulst_test3" );
    addProperty( "SampleRunGroupIDSessionReadLockSetting"       , "set lockmode session where readlock = nolock" );
    addProperty( "SampleRunGroupIDQueryString"                  , "select valc_next_id.nextval" );
    addProperty( "QueryForSampleRun"                            , "QueryForSampleRun %d" );
    addProperty( "SQLStmtCloseSampleRun"                        , "SQLStmtCloseSampleRun %d %d" );
    addProperty( "SQLStmtInsertWorklistRelation"                , "SQLStmtInsertWorklistRelation"  );
    addProperty( "SQLStmtNextWorklistID"                        , "SQLStmtNextWorklistID"  );
    addProperty( "SQLStmtInsertRerun"                           , "SQLStmtInsertRerun"  );
    addProperty( "LogAllDatabaseStatements"                     , "true"  );
    addProperty( "DBUpdateThreadConnectionString"               , "10"  );
    addProperty( "DBUpdateThreadSessionReadLockSetting"         , ""    );
    addProperty( "DBUpdateThreadShutdownTimeoutSecs"            , "10"  );
    addProperty( "DBUpdateThreadCancelPendingUpdatesOnShutdown" , "true");
    addProperty( "PendingUpdateWaitTimeoutSecs"     , "10" );
    addProperty( "PendingUpdateWaitTimeoutSecs"     , "10" );
    addProperty( "PendingUpdateWaitTimeoutSecs"     , "10" );
    addProperty( "PendingUpdateWaitTimeoutSecs"     , "10" );
    addProperty( "RuleEngineTimeoutSecs"            , "10" );
    addProperty( "TestNamesQuery"                   , "LoadTestNames" );
    addProperty( "TestNamesConnectionString"        , "Owt" );
    addProperty( "TestNamesSessionReadLockSetting"  , "Owt" );
    addProperty( "ClusterIDsQuery"                  , "LoadClusterIDs" );
    addProperty( "ClusterIDsConnectionString"       , "Owt" );
    addProperty( "ClusterIDsSessionReadLockSetting" , "Owt" );
    addProperty( "WorklistRelationQuery"                    , "LoadWorklistRelation" );
    addProperty( "WorklistRelationConnectionString"         , "Owt" );
    addProperty( "WorklistRelationSessionReadLockSetting"   , "Owt" );
    addProperty( "ProjectsQuery"                    , "LoadProjects" );
    addProperty( "ProjectsConnectionString"         , "Owt" );
    addProperty( "ProjectsSessionReadLockSetting"   , "Owt" );
    addProperty( "QCRuleQuery"                      , "LoadRules" );
    addProperty( "QCRuleConfigQuery"                , "LoadRuleConfig" );
	addProperty( "InitialisationTimeoutSecs"        , "10" );
	addProperty( "QCRuleConfigSessionReadLockSetting", "Owt" );
	addProperty( "QCRuleConfigConnectionString"     , "Owt" );
	addProperty( "QCRuleSessionReadLockSetting"     , "Owt" );
	addProperty( "QCRuleConnectionString"           , "Owt" );
    addProperty( "ConnectionFactoryType"            , "Mock" );
    addProperty( "ForceReloadConnectionString"      , "Owt" );
    addProperty( "ForceReloadSessionReadLockSetting", "" );
    addProperty( "LoadWorklistEntries"              , "LoadWorklistEntries" );
    addProperty( "LoadWorklistRelations"            , "LoadWorklistRelations" );
    addProperty( "RefTempTableName"                 , "Q109Temp" );
    addProperty( "LoadReferencedWorklistEntries"    , "LoadReferencedWorklistEntries" );
    addProperty( "LoadReferencedWorklistRelations"  , "LoadReferencedWorklistRelations" );
    addProperty( "LoadNonLocalResults"              , "LoadNonLocalResults" );
    addProperty( "LoadBuddyDatabase"                , "LoadBuddyDatabase" );
    addProperty( "BuddyDatabaseInclusionRule"       , "function accept () return true end" );
    addProperty( "WorklistInclusionRule"            , "function accept () return true end" );
    addProperty( "ExceptionalDataHandler"           , 
"    function canProvideProjectIDFor( barcode )                         \n"
"        local ok = false                                               \n"
"        if ( string.find( barcode, '118' ) == 1 ) then                 \n"
"            ok = true                                                  \n"
"        end                                                            \n"
"        return ok                                                      \n"
"    end                                                                \n"
"    ---------------------------------------------------------          \\n"
"    function getProjectIDFor( barcode )                                \n"
"        if ( string.find( barcode, '118' ) ~= 1 ) then                 \n"
"            error( \"Cannot identify project ID for barcode \" .. barcode )  \n"
"        end                                                            \n"
"        return -832455                                                 \n"
"    end                                                                \n"
"    ---------------------------------------------------------          \\n"
"    function notifyBuddyDatabaseEntryIgnored()                         \n"
"        local continueProcessing = true                                \n"
"        local adviseUser = true                                        \n"
"        return continueProcessing, adviseUser                          \n"
"    end  \n"
"    ---------------------------------------------------------          \\n"
"    function notifyWorklistEntryIgnored()                              \n"
"        local continueProcessing = true                                \n"
"        local adviseUser = true                                        \n"
"        return continueProcessing, adviseUser                          \n"
"    end  \n"
"    ---------------------------------------------------------          \\n"
"    function notifyCannotAllocateResultToWorklistEntry()               \n"
"        local continueProcessing = true                                \n"
"        local adviseUser = true                                        \n"
"        return continueProcessing, adviseUser                          \n"
"    end  \n"
 );
    addProperty( "QCGatesConnectionString", "QCGatesConnectionString" );
    addProperty( "QCGatesSessionReadLockSetting", "QCGatesSessionReadLockSetting" );
    addProperty( "QCGatesQuery", "LoadQCGates" );
}

void MockConfig::addProperty( const std::string& name, const std::string& value )
{
    m_properties.insert( std::make_pair( name, value ) );
}

void MockConfig::edit( const std::string& configElement, const std::string& newValue )
{
    m_properties.erase( configElement );
    m_properties.insert( std::make_pair( configElement, newValue ) );
}

std::string MockConfig::toString() const
{
    std::string buffer;

    for ( Properties::const_iterator i = m_properties.begin(); i != m_properties.end(); ++i )
    {
        buffer.append( i->first );
        buffer.append( ":\n" );
        buffer.append( i->second );
        buffer.append( "\n\n\n" );
    }

    return buffer;
}

}


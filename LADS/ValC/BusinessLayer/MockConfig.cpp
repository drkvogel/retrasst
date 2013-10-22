#include "MockConfig.h"

namespace valc
{

MockConfig::MockConfig()
{
    addProperty( "LoadRules"                        , "LoadRules" );
    addProperty( "LoadRuleConfig"                   , "LoadRuleConfig" );
	addProperty( "InitialisationTimeoutSecs", "10" );
	addProperty( "QCRuleConfigConnectionSessionReadLockSetting", "Owt" );
	addProperty( "QCRuleConfigConnectionString"     , "Owt" );
	addProperty( "RuleEngineErrorCode"              , "999" );
	addProperty( "DBUpdateConnectionString"         , "Doesn't matter" );
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
"    ---------------------------------------------------------          \n"
"    function getProjectIDFor( barcode )                                \n"
"        if ( string.find( barcode, '118' ) ~= 1 ) then                 \n"
"            error( \"Cannot identify project ID for barcode \" .. barcode )  \n"
"        end                                                            \n"
"        return -832455                                                 \n"
"    end                                                                \n"
"    ---------------------------------------------------------          \n"
"    function notifyBuddyDatabaseEntryIgnored()                         \n"
"        local continueProcessing = true                                \n"
"        local adviseUser = true                                        \n"
"        return continueProcessing, adviseUser                          \n"
"    end  \n"
"    ---------------------------------------------------------          \n"
"    function notifyWorklistEntryIgnored()                              \n"
"        local continueProcessing = true                                \n"
"        local adviseUser = true                                        \n"
"        return continueProcessing, adviseUser                          \n"
"    end  \n"
"    function notifyCannotAllocateResultToWorklistEntry()               \n"
"        local continueProcessing = true                                \n"
"        local adviseUser = true                                        \n"
"        return continueProcessing, adviseUser                          \n"
"    end  \n"
 );
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


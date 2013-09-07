#include "MockConfig.h"

namespace valc
{

MockConfig::MockConfig()
{
    addProperty( "LoadWorklistEntries"              , "LoadWorklistEntries" );
    addProperty( "LoadWorklistRelations"            , "LoadWorklistRelations" );
    addProperty( "RefTempTableName"                 , "Q109Temp" );
    addProperty( "LoadReferencedWorklistEntries"    , "LoadReferencedWorklistEntries" );
    addProperty( "LoadReferencedWorklistRelations"  , "LoadReferencedWorklistRelations" );
    addProperty( "LoadNonLocalResults"              , "LoadNonLocalResults" );
    addProperty( "LoadBuddyDatabase"                , "LoadBuddyDatabase" );
    addProperty( "BuddyDatabaseInclusionRule"       , "function accept () return true end" );
    addProperty( "WorklistInclusionRule"            , "function accept () return true end" );
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


#include "Cursor.h"
#include "RuleLoaderInitialiser.h"
#include "RuleLoaderUsingMap.h"

namespace valc
{

RuleLoaderInitialiser::RuleLoaderInitialiser( const std::string& sqlQuery )
    :
    m_ruleLoader(0),
    m_sql( sqlQuery )
{
}

void RuleLoaderInitialiser::consume( paulstdb::Cursor* c )
{
    while ( *c )
    {
        const std::string name   = paulstdb::read<std::string>( *c, 0 );
        const std::string script = paulstdb::read<std::string>( *c, 1 );
        m_ruleLoader->addRule( name, script );
        c->next();
    }
}

std::string RuleLoaderInitialiser::getSQL()
{
    return m_sql;
}

void RuleLoaderInitialiser::setInitialisationTarget( RuleLoaderUsingMap* ruleLoader )
{
    m_ruleLoader = ruleLoader;
}

}


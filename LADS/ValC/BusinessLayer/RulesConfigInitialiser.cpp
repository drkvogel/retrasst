#include "Cursor.h"
#include "RulesConfigInitialiser.h"
#include "RulesConfigUsingMap.h"

namespace valc
{

RulesConfigInitialiser::RulesConfigInitialiser( const std::string& sqlQuery )
    :
    m_rulesConfig( 0 ),
    m_sql( sqlQuery )
{
}

void RulesConfigInitialiser::consume( paulstdb::Cursor* c )
{
    while ( *c )
    {
        const int         test     = paulstdb::read<int>        ( *c, 0 );
        const int         machine  = paulstdb::read<int>        ( *c, 1 );
        const std::string ruleName = paulstdb::read<std::string>( *c, 2 );
        m_rulesConfig->specify( ruleName, test, machine );
        c->next();
    }
}

std::string RulesConfigInitialiser::getSQL()
{
    return m_sql;
}

void RulesConfigInitialiser::setInitialisationTarget( RulesConfigUsingMap* rulesConfig )
{
    m_rulesConfig = rulesConfig;
}

}



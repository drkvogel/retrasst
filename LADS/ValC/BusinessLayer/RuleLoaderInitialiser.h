#ifndef RULELOADERINITIALISERH
#define RULELOADERINITIALISERH

#include "CursorConsumer.h"

namespace valc
{

class RuleLoaderUsingMap;

class RuleLoaderInitialiser : public CursorConsumer
{
public:
    RuleLoaderInitialiser( const std::string& sqlQuery );
    void consume( paulstdb::Cursor* c );
    std::string getSQL();
    void setInitialisationTarget( RuleLoaderUsingMap* ruleLoader );
private:
    RuleLoaderUsingMap* m_ruleLoader;
    std::string m_sql;

};

}

#endif


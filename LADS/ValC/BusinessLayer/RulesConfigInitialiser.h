#ifndef RULESCONFIGINITIALISERH
#define RULESCONFIGINITIALISERH

#include "CursorConsumer.h"

namespace valc
{

class RulesConfigUsingMap;

class RulesConfigInitialiser : public CursorConsumer
{
public:
    RulesConfigInitialiser( const std::string& sqlQuery );
    void consume( paulstdb::Cursor* c );
    std::string getSQL();
    void setInitialisationTarget( RulesConfigUsingMap* rulesConfig );
private:
    RulesConfigUsingMap* m_rulesConfig;
    std::string m_sql;
};

}

#endif


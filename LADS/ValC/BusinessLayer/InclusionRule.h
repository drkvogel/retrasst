#ifndef INCLUSIONRULEH
#define INCLUSIONRULEH

#include <string>

struct lua_State;

/*
    Call 'init' before 'addParam' or 'run'.

    Can call 'run' more than once on the same instance, but always must first call 'init' and then specify any required parameters.
*/
namespace valc
{

class InclusionRule
{
public:
    InclusionRule( const std::string& script, const std::string& functionName = "accept" );
    ~InclusionRule();
    void init();
    void addParam( const std::string& s );
    void addParam( int i );
    bool getBooleanResult();
    void run(); 
private:
    lua_State* L;
    std::string m_functionName;

    InclusionRule( const InclusionRule& );
    InclusionRule& operator=( const InclusionRule& );
};

}

#endif


#ifndef MockConfigH
#define MockConfigH

#include <map>
#include <string>

namespace valc
{

class MockConfig
{
public:
    MockConfig();
    void edit( const std::string& configElement, const std::string& newValue );
    std::string toString() const;
private:
    typedef std::map< std::string, std::string > Properties;
    Properties m_properties;

    void addProperty( const std::string& name, const std::string& value );
};

};

#endif

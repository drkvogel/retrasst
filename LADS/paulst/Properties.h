#ifndef PropertiesH
#define PropertiesH

#include <map>
#include <string>

namespace paulst
{

class Properties
{
public:
    Properties( const std::string& nameValuePairs );
    std::string getValue( const std::string& name ) const;
private:
    typedef std::map< std::string, std::string > Map;
    Map m_map;
};

};
#endif


#ifndef PAULSTCONFIGH
#define PAULSTCONFIGH

#include <map>
#include <string>

namespace paulst
{

/*
configString is processed as follows:

On encountering a line that ends with a colon, the value of the string value of that line is assumed to represent a property name. The 
colon is not considered part of the property name.
The following lines are treated as the property value, until an  empty line is encountered.
Newlines are stripped. If your property should include a newline value, use '\n'. For carriage-returns, use '\r'. For tabs, use '\t'. For '\' use '\\'. Note that a '\' is only considered as an escape sequence if it is followed by one of 'n', 'r', 't' or '\'.

Lines beginning with '#' are ignored.

example:

--------------------------------------------
my property name:
my
property
value

my second property name:
my second

value
--------------------------------------------

The above would result in the following config:

 name                    | value
--------------------------------------------
 my property name        | mypropertyvalue
--------------------------------------------
 my second property name | my second

*/
class Config
{
public:
    Config( const std::string& configString );
    bool contains( const std::string& name ) const;
    std::string get( const std::string& name ) const;
private:
    typedef std::map< std::string, std::string > Properties;
    Properties m_properties;
};

};

#endif


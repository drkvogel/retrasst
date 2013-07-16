#include "Config.h"
#include "Require.h"

#include <sstream>

namespace paulst
{

Config::Config( const std::string& configString )
{
    std::istringstream stream( configString );

    enum State { WaitingForPropertyName, ReadingPropertyValue };

    State state = WaitingForPropertyName;

    std::string name, value;

    for ( std::string line; std::getline( stream, line ); )
    {
        if ( line.size() && '#' == *(line.begin()) )
            continue;

        switch ( state )
        {
            case WaitingForPropertyName:
                if ( line.size() && ( ':' == *(line.rbegin()) ) )
                {
                    line.resize( line.size() - 1 );
                    name = line;
                    state = ReadingPropertyValue;
                }
                break;
            case ReadingPropertyValue:
                if ( line.empty() )
                {
                    unsigned int pos = 0;
                    while ( value.size() && ( pos < ( value.size() - 1 ) ) )
                    {
                        if ( value[pos] == '\\' )
                        {
                            switch ( value[pos + 1] )
                            {
                                case '\\': value[pos] = '\\'; value.erase( pos + 1, 1 ); break;
                                case 'n' : value[pos] = '\n'; value.erase( pos + 1, 1 ); break;
                                case 'r' : value[pos] = '\r'; value.erase( pos + 1, 1 ); break;
                                case 't' : value[pos] = '\t'; value.erase( pos + 1, 1 ); break;
                            }
                        }
                        ++pos;
                    }
                    m_properties.insert( std::make_pair( name, value ) );
                    name.resize(0);
                    value.resize(0);
                    state = WaitingForPropertyName;
                }
                else
                {
                    value += line;
                }
                break;
        }
    }
}

bool Config::contains( const std::string& name ) const
{
    return m_properties.end() != m_properties.find( name );
}

std::string Config::get( const std::string& name ) const
{
    Properties::const_iterator i = m_properties.find( name );
    if ( i == m_properties.end() )
    {
        throwRuntimeError( ( std::string( "Property not defined: ") + name  ).c_str() );
    }
    return i->second;
}

}


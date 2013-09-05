#include "Properties.h"
#include "Require.h"
#include "StrUtil.h"

namespace paulst
{

Properties::Properties( const std::string& nameValuePairs )
{
    std::string key, value;
    enum state { ReadingKey, ReadingValue } s;
    s = ReadingKey;
    for ( std::string::const_iterator i = nameValuePairs.begin(); 
          i != nameValuePairs.end(); ++i )
    {
        switch ( s )
        {
            case ReadingKey:
                switch( *i )
                {
                    case '=': s = ReadingValue; break;
                    case ' ': break;
                    default: key.push_back(*i);
                }
                break;

            case ReadingValue:
                if ( *i == ',' )
                {
                    paulst::trim(key);
                    paulst::trim(value);
                    require(key.size());
                    m_map.insert( std::make_pair(key, value) );
                    key.resize(0);
                    value.resize(0);
                    s = ReadingKey;
                }
                else 
                {
                    value.push_back(*i);
                }
                break;
        }
    }
}

std::string Properties::getValue( const std::string& name ) const
{
    std::string value;

    if ( m_map.count( name ) )
    {
        Map::const_iterator i = m_map.find( name );
        value = i->second;
    }

    return value;
}

}


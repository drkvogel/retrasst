#include <boost/lexical_cast.hpp>
#include "StrBuilder.h"

std::string operator<<( const std::string& left, const int& right )
{
	return left + boost::lexical_cast<std::string>(right);
}

std::string operator<<( const std::string& left, const char* right )
{
	return left + right;
}

std::string operator<<( const std::string& left, const std::string& right )
{
	return left + right;
}


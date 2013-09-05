#ifndef StrBuilderH
#define StrBuilderH


#include <string>

std::string operator<<( const std::string& left, const int& right );
std::string operator<<( const std::string& left, const char* right );
std::string operator<<( const std::string& left, const std::string& right );

#endif

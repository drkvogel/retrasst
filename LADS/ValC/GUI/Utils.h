//---------------------------------------------------------------------------

#ifndef UtilsH
#define UtilsH


#include <string>
#include <map>
#include <fstream>
#include <sstream>
#include <System.hpp>

/*----------------- Assorted Utilities -----------------------*/


/** Provides assorted string conversion utilities. */
class Utils {
public:
	Utils(void);
	~Utils(void);
	static char Utils::char2Lower(char c);
	static std::string toLowerCase(const std::string s);
	static std::string float2str(float fNumber);
	static UnicodeString float2unicodestr(float f);
	static std::string int2str(int pNumber);
	static UnicodeString Utils::int2unicodestr(int n);
	static bool str2int(std::string str, int *i);
	static bool unicodestr2int(const UnicodeString u);
	static std::string unicodestr2str(const UnicodeString u);
	static UnicodeString str2unicodestr(std::string str);
	static std::auto_ptr<wchar_t> charstr2wcharstr(const char* source);
	static std::auto_ptr<char> wcharstr2charstr(const wchar_t* source);
};

//---------------------------------------------------------------------------
#endif

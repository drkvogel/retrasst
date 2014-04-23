//---------------------------------------------------------------------------
#include<algorithm>
#include<string>

#include "Utils.h"

#pragma hdrstop
//---------------------------------------------------------------------------

#pragma package(smart_init)
//---------------------------------------------------------------------------

Utils::Utils(void) {
}

Utils::~Utils(void) {
}

/** Converts a character to lower-case.
  *
  * @param c      the character to be converted
  */
char Utils::char2Lower(char c)
{	return (char)tolower(c);
}

/** Returns a lowercase version of the given string.
  *
  * @param s      the string to be converted to lower case
  */
std::string Utils::toLowerCase(const std::string s)
{
	std::string str(s);
	std::transform(str.begin(),str.end(),str.begin(),char2Lower);
	return str;
}

std::string Utils::unicodestr2str(const UnicodeString u) {
	std::string str = "";
	const wchar_t *u1 = u.c_str();
	std::auto_ptr<char> u2 =  wcharstr2charstr(u1);
	char *u3 = u2.get();
	str.append(u3);
	return str;
}

UnicodeString Utils::str2unicodestr(std::string str) {
	const char *s = str.c_str();
	std::auto_ptr<wchar_t> w = charstr2wcharstr(s);
	UnicodeString u = w.get();
	return u;
}

std::auto_ptr<wchar_t> Utils::charstr2wcharstr(const char* source)
// from Guru Sohoni
{
	int len = strlen(source);
	std::auto_ptr<wchar_t> wsource( new wchar_t[ len * 4 + 4 ]);
	int err = mbstowcs( &(*wsource), source, len + 1 );
	if (err < 0 )
	{
		throw std::string( "Invalid multibyte character encountered" );
	}
	return wsource;
}

std::auto_ptr<char> Utils::wcharstr2charstr(const wchar_t* wsource)
// from Guru Sohoni
{
	int len = wcslen(wsource);
	std::auto_ptr<char> source( new char[ len * 4 + 4 ]);
	int err = wcstombs( &(*source), wsource, len + 1 );
	if (err < 0 )
	{
		throw std::string( "Invalid multibyte character encountered" );
	}
	return source;
}

std::string Utils::float2str(float fNumber) {
	std::ostringstream oOStrStream;
	oOStrStream << fNumber;
	return oOStrStream.str();
}

UnicodeString Utils::float2unicodestr(float f) {
	return str2unicodestr(float2str(f));
}

std::string Utils::int2str(int pNumber)  {
// adapted from  http://stackoverflow.com/questions/5590381/easiest-way-to-convert-int-to-string-in-c
	std::ostringstream oOStrStream;
	oOStrStream << pNumber;
	return oOStrStream.str();
}

UnicodeString Utils::int2unicodestr(int n) {
	return str2unicodestr(int2str(n));
}

bool Utils::unicodestr2int(const UnicodeString u) {
	int *i;
	str2int(unicodestr2str(u),i);
	return *i;
}

bool Utils::str2int(std::string str, int *i) {
// adapted from http://stackoverflow.com/questions/194465/how-to-parse-a-string-to-an-int-in-c
	bool problem = false;
	char *end;
	long l;
	const char *s = str.c_str();
	errno = 0;
	l = strtol(s, &end, 10);    // reads in the first integer it finds,
								// ignores any further chars
	if (((errno == ERANGE && l == LONG_MAX) || l > INT_MAX)
		||
		 ((errno == ERANGE && l == LONG_MIN) || l < INT_MIN)) {
		problem = true;
	}
	else if (*s == '\0' || *end != '\0') {
		problem = true;
    }
	*i = l;
	std::string check = int2str(*i);
	if (!(check==str)) {     // any further chars after the int will fail this check
		problem = true;
	}

	/*
	std::string pu = "value of l: ";
	pu.append(int2str(l));
	pu.append("\nerrno is : ");
	pu.append(int2str(errno));
	pu.append("\nvalue of i is : ");
	pu.append(int2str(*i));
	addToDebug(pu);
	*/
	return !problem;
}

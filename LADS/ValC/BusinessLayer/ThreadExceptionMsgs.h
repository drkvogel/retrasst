#ifndef ThreadExceptionMsgsH
#define ThreadExceptionMsgsH

#include "CritSec.h"
#include <string>
#include <System.hpp>
#include <vector>

namespace valc
{

class ThreadExceptionMsgs
{
public:
	void add( const std::string& msg );
	void add( const UnicodeString& msg );
	void add( const char* msg );
	void throwFirst();
private:
	paulst::CritSec m_cs;
	std::vector< std::string > m_msgs;
};

};

#endif

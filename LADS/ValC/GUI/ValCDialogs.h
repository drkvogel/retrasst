#ifndef VALCDIALOGSH
#define VALCDIALOGSH

#include <string>
#include <SysUtils.hpp>


// This file provides common dialogue functions,
// e.g. for showing warnings and reporting exceptions,
// that kind of thing.


namespace valcui
{

void reportException( const Exception& e );

/* Returns one of:
WAIT_ABANDONED
WAIT_OBJECT_0
WAIT_TIMEOUT
WAIT_FAILED
For details, refer to msdn.microsoft.com.
*/ 
unsigned long showWaitDialog( HANDLE signal, const std::string& msg, unsigned long waitMillis );

}

#endif


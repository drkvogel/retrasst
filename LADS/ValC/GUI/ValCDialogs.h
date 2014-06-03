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

/* Returns true if signal obtained before timeout. */
bool showWaitDialog( HANDLE signal, const std::string& msg, unsigned long waitMillis );

}

#endif


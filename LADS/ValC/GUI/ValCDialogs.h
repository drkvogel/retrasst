#ifndef VALCDIALOGSH
#define VALCDIALOGSH

#include <string>
#include <SysUtils.hpp>

namespace valcui
{

void reportException( const Exception& e );
/* Returns true if signal obtained before timeout. */
bool showWaitDialog( HANDLE signal, const std::string& msg, unsigned long waitMillis );

}

#endif


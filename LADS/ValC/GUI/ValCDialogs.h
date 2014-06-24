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
void showErrorMsg( const std::string& msg );
void showWarningMsg( const std::string& msg );

}

#endif


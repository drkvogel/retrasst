#include "ComUtil.h"
#include <sstream>
#include <System.hpp>
#include <SysUtils.hpp>
#include <Winerror.h>

namespace paulstdb
{

void throwUnlessOK( long hresult )
{
    if ( ! SUCCEEDED( hresult ) )
    {
        std::ostringstream errorMsg;
        errorMsg << "Bad return value from COM: " << hresult;
        std::string errorMessage = errorMsg.str();
        throw Exception( UnicodeString( errorMessage.c_str() ) );
    }
}

}


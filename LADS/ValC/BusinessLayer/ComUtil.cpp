#include "ComUtil.h"
#include "Require.h"
#include <sstream>
#include <Winerror.h>

namespace valc
{

void throwUnlessOK( long hresult )
{
    if ( ! SUCCEEDED( hresult ) )
    {
        std::ostringstream errorMsg;
        errorMsg << "Bad return value from COM: " << hresult;
        std::string errorMessage = errorMsg.str();
        throwRuntimeError(errorMessage);
    }
}

}


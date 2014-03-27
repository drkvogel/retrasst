#include "DirUtil.h"
#include "ExceptionUtil.h"
#include <FileCtrl.hpp>
#include "Require.h"
#include <Shlobj.h>
#include "StrUtil.h"
#include <System.hpp>
#include <windows.h>


namespace paulst
{

// without trailing '\'
std::string appDataDir()
{
    wchar_t* path;
    // Get path for each computer, non-user specific and non-roaming data.
    throwUnless ( SUCCEEDED( SHGetKnownFolderPath( FOLDERID_LocalAppData, 0, NULL, &path ) ) );

    const UnicodeString dir( path );

    CoTaskMemFree( path );

    require( DirectoryExists( dir ) );

    return AnsiString( dir.c_str() ).c_str();
}

// without any trailing '\'
std::string appDir()
{
    UnicodeString appPath = ParamStr(0);
    std::string appPathStr = paulst::stdstring( appPath.c_str() );
    std::string::iterator i = paulst::find_last( appPathStr.begin(), appPathStr.end(), paulst::equals( '\\' ) );
    require( i != appPathStr.end() );
    const std::string dir( appPathStr.begin(), i );
    require( DirectoryExists( dir.c_str() ) );
    return dir;
}

Dir::Dir( const std::string& path )
    :
    m_path( path )
{
    const bool exists = 
        DirectoryExists( path.c_str() )
        ||
        ( ForceDirectories( path.c_str() ) && DirectoryExists( path.c_str() ) );

    if ( ! exists )
    {
        paulst::exception( "Failed to create directory %s", path.c_str() );
    }
}

std::string Dir::path() const
{
    return m_path;
}

}


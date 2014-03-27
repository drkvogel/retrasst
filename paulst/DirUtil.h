#ifndef DIRUTILH
#define DIRUTILH

#include <string>

namespace paulst
{

/* 
    Returns the Local AppData Dir on a Windows PC.
    
    The returned string has no trailing '\'.
*/
std::string appDataDir();

/* 
    Returns the Dir in which resides the executable of calling VCL app.

    The returned string has no trailing '\'.
*/
std::string appDir();

class Dir
{
public:
    /*
        Creates the directory if it doesn't exist.
        Throws an Exception if the directory doesn't exist but cannot be created.
    */
    Dir( const std::string& path );
    std::string path() const;
private:
    const std::string m_path;
};

}

#endif


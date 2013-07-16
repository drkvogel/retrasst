#ifndef FILEWRITERC
#define FILEWRITERC

#include "LoggingService.h"

#include <stdio.h>

namespace paulst
{

class FileWriterC : public Writer
{
public:
    FileWriterC( const std::string& filename );
    ~FileWriterC();
    void write( const std::string& msg );
private:
    FILE* m_fp;
};

};

#endif


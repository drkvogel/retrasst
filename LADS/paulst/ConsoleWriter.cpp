#include "ConsoleWriter.h"
#include <cstdio>

namespace paulst
{

ConsoleWriter::ConsoleWriter()
{
}

void ConsoleWriter::write( const std::string& msg )
{
    std::puts( msg.c_str() );
    std::puts("\n");
}

}


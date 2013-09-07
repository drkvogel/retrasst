#ifndef CONSOLEWRITERH
#define CONSOLEWRITERH

#include "LoggingService.h"

namespace paulst
{

class ConsoleWriter : public Writer
{
public:
    ConsoleWriter();
    void write( const std::string& msg );
};

};

#endif


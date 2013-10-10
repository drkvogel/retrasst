#ifndef NOLOGGINGH
#define NOLOGGINGH

#include "LoggingService.h"


class NoLogging : public paulst::Writer
{
public:
    void write( const std::string& ) {}
};

#endif



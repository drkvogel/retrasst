#ifndef RESULTDIRECTORYH
#define RESULTDIRECTORYH

#include "API.h"

namespace valc
{

class ResultDirectory
{
public:
    virtual ~ResultDirectory();
    virtual Range<TestResultIterator>   equal_range( int worklistID )                const = 0;
    virtual const TestResult*           findResult( int resultID )                   const = 0;
};

};

#endif


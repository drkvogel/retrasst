#ifndef RESULTDIRECTORYH
#define RESULTDIRECTORYH

namespace valc
{

class TestResult;

class ResultDirectory
{
public:
    virtual ~ResultDirectory();
    virtual const TestResult* findResult( int resultID ) const = 0;
    virtual const TestResult* findResultForWorklistEntry( int worklistID ) const = 0;
};

};

#endif


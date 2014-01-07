#ifndef SAMPLERUNGROUPIDGENERATORUSINGDBH
#define SAMPLERUNGROUPIDGENERATORUSINGDBH

#include "SampleRunGroupModel.h"

namespace paulst
{
    class Config;
}

namespace paulstdb
{
    class AbstractConnectionFactory;
}


namespace valc
{

class SampleRunGroupIDGeneratorUsingDB : public SampleRunGroupIDGenerator
{
public:
    SampleRunGroupIDGeneratorUsingDB( paulstdb::AbstractConnectionFactory* connectionFactory, paulst::Config* config );
    int nextID();
private:
    paulstdb::AbstractConnectionFactory* const  m_connectionFactory;
    const std::string                           m_connectionString;
    const std::string                           m_sessionReadLockSetting;
    const std::string                           m_sqlQuery;
};

}

#endif


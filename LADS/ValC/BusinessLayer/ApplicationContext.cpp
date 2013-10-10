#include "ApplicationContext.h"
#include "Config.h"

namespace valc
{
ApplicationContext::ApplicationContext()
    : 
    snapshot(0),
    localMachineID(0),
    user(0),
    config(0),
    log(0),
    connectionFactory(0)
{
}

ApplicationContext::~ApplicationContext()
{
    delete snapshot;
    delete config;
    delete connectionFactory;
}

std::string ApplicationContext::getProperty( const std::string& name) const
{
    return config->get(name);
}

}


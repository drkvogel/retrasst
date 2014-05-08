#ifndef MODELH
#define MODELH

#include <memory>
#include <SysUtils.hpp>
#include <vector>

class LogManager;

namespace valcui
{

class BusinessLayer;
class ModelEventListener;

class Model
{
public:
    Model();
    void close();
    void registerModelEventListener( ModelEventListener*  );
    void doForceReload();
    void setLog( LogManager* l );
    void setBusinessLayer( BusinessLayer* bl );
    void setSelectedWorklistEntry( int worklistEntryID );

    void __fastcall warningAlarmOn();
    void __fastcall warningAlarmOff();
private:
    typedef std::vector<ModelEventListener*> Listeners;
    Listeners m_listeners;
    std::unique_ptr<BusinessLayer> m_businessLayer;
    LogManager* m_log;
    int m_selectedWorklistEntry;
    
    void handleException( const Exception& e );
};

}

#endif



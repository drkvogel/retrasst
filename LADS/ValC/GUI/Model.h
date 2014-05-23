#ifndef MODELH
#define MODELH

#include "API.h"
#include <memory>
#include <System.Classes.hpp>
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
    void borrowSnapshot( TThreadMethod callback );
    void close();
    // Only use 'getSnapshot' from within a callback via 'borrowSnapshot' (above)
    valc::SnapshotPtr getSnapshot() const;
    void registerModelEventListener( ModelEventListener*  );
    void doForceReload();
    int  getSelectedWorklistEntry() const;
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
    void notifyListeners( int eventID );
};

}

#endif



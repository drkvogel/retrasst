#ifndef MODELEVENTLISTENERH
#define MODELEVENTLISTENERH

namespace valc
{
    class SnapshotPtr;
}

namespace valcui
{

class ModelEventListener
{
public:
    ModelEventListener();
    virtual ~ModelEventListener();
    virtual void onForceReload( valc::SnapshotPtr& );
    virtual void onWarningAlarmOn();
    virtual void onWarningAlarmOff();
    virtual void onWorklistEntrySelected( int worklistEntryID );
};

}

#endif


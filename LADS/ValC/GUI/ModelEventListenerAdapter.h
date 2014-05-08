#ifndef MODELEVENTLISTENERADAPTERH
#define MODELEVENTLISTENERADAPTERH

#include "ModelEventListener.h"

namespace valcui
{

template<class T>
class ModelEventListenerAdapter : public ModelEventListener
{
public:
    ModelEventListenerAdapter(T* t)
        :
        m_t(t)
    {
    }

    void onForceReload( valc::SnapshotPtr& s )
    {
        m_t->onForceReload( s );
    }

    void onWarningAlarmOn()
    {
        m_t->onWarningAlarmOn();
    }

    void onWarningAlarmOff()
    {
        m_t->onWarningAlarmOff();
	}

	void onWorklistEntrySelected( int worklistEntryID )
	{
		m_t->onWorklistEntrySelected( worklistEntryID );
	}

private:
    T* m_t;
};

}

#endif


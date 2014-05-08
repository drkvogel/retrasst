#ifndef QCViewControllerH
#define QCViewControllerH

#include "ModelEventListenerAdapter.h"

class TQCViewFrame;

namespace valcui
{

class Model;

class QCViewController
{
public:
	QCViewController( TQCViewFrame* widgetContainer, Model* m );
	void onForceReload( valc::SnapshotPtr& );
	void onWarningAlarmOn();
	void onWarningAlarmOff();
	void onWorklistEntrySelected( int worklistEntryID );
private:
	TQCViewFrame* m_widgetContainer;
	ModelEventListenerAdapter<QCViewController> m_eventListener;
};

}

#endif

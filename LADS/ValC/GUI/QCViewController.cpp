#include "Model.h"
#include "TQCViewFrame.h"
#include "QCViewController.h"

namespace valcui
{

QCViewController::QCViewController( TQCViewFrame* widgetContainer, Model* m )
	:
	m_widgetContainer( widgetContainer ),
	m_eventListener(this)
{
	m->registerModelEventListener( &m_eventListener );
}

void QCViewController::onForceReload( valc::SnapshotPtr& )
{
	m_widgetContainer->Label1->Text = UnicodeString();
}

void QCViewController::onWarningAlarmOn()
{
}

void QCViewController::onWarningAlarmOff()
{
}

void QCViewController::onWorklistEntrySelected( int worklistEntryID )
{
	m_widgetContainer->Label1->Text = UnicodeString(worklistEntryID);
}

}

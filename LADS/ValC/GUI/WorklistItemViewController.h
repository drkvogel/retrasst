#ifndef WorklistItemViewControllerH
#define WorklistItemViewControllerH

#include "API.h"
#include "CritSec.h"
#include "ModelEventListenerAdapter.h"
#include "WorklistItemViewData.h"
#include <stack>
#include "TWorklistItemViewFrame.h"

namespace stef
{
    class ThreadPool;
}

namespace valcui
{

class Model;


class WorklistItemViewController
{
public:
	WorklistItemViewController( TWorklistItemViewFrame* widgetContainer, Model* m );
    void factoryCallback( bool cancelled, const std::string& error, const WorklistItemViewData& output );
	void onForceReload( valc::SnapshotPtr& );
	void onWarningAlarmOn();
	void onWarningAlarmOff();
	void onWorklistEntrySelected( int worklistEntryID );
    void setThreadPool( stef::ThreadPool* tp );
    void __fastcall useUpdatedViewData();
private:
	TWorklistItemViewFrame* m_widgetContainer;
	ModelEventListenerAdapter<WorklistItemViewController> m_eventListener;
    stef::ThreadPool* m_threadPool;
    valc::SnapshotPtr m_snapshotPtr;
    std::unique_ptr<WorklistItemViewData> m_viewData;
	paulst::CritSec m_critSec;

    void clear();
	void describeResult( const valc::TestResult* r );
    UnicodeString describeTest( int testID ) const;

};

}

#endif


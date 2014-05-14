#ifndef QCViewControllerH
#define QCViewControllerH

#include "API.h"
#include "CritSec.h"
#include "ModelEventListenerAdapter.h"
#include "QCViewData.h"
#include <stack>
#include "TQCViewFrame.h"
#include <vector>

namespace stef
{
    class ThreadPool;
}

namespace valcui
{

class Model;


class QCViewController
{
public:
	QCViewController( TQCViewFrame* widgetContainer, Model* m );
    void factoryCallback( bool cancelled, const std::string& error, const QCViewData& output );
	void onForceReload( valc::SnapshotPtr& );
	void onWarningAlarmOn();
	void onWarningAlarmOff();
	void onWorklistEntrySelected( int worklistEntryID );
    void setThreadPool( stef::ThreadPool* tp );
    void __fastcall useUpdatedViewData();
private:
	TQCViewFrame* m_widgetContainer;
	ModelEventListenerAdapter<QCViewController> m_eventListener;
    stef::ThreadPool* m_threadPool;
    valc::SnapshotPtr m_snapshotPtr;
    std::unique_ptr<QCViewData> m_viewData;
	paulst::CritSec m_critSec;
	std::stack< TTreeViewItem* > m_nodeStack;
	std::vector< TTreeViewItem* > m_expandList;

	void addNodesForControllingQCs(	const valc::QCControls& controls );
	void addNodesForRules( int controlResultID );
    std::string describe( valc::ResultCode rc ) const;
	std::string describeControl( const valc::QCControl& c ) const;
    std::string describeControlResult( const valc::QCControl& c ) const;
	std::string describeTestResult( const valc::TestResult* r ) const;
	std::string describeWorklistEntry() const;
	void popNode();
	void pushNode( const std::string& label, bool expand );
};

}

#endif


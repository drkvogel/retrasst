#ifndef QCViewControllerH
#define QCViewControllerH

#include "API.h"
#include "ModelEventListenerAdapter.h"
#include <stack>
#include "TQCViewFrame.h"
#include <vector>

namespace valcui
{

class Model;
class QCViewData;


class QCViewController
{
public:
	QCViewController( TQCViewFrame* widgetContainer, Model* m );
	void notify( int modelEvent, const EventData& eventData );
    void __fastcall update();
private:
	TQCViewFrame* const                         m_widgetContainer;
	ModelEventListenerAdapter<QCViewController> m_eventListener;
    Model* const                                m_model;
	std::stack< TTreeViewItem* >                m_nodeStack;
	std::vector< TTreeViewItem* >               m_expandList;

	void        addNodesForControllingQCs   ( const QCViewData& d, const valc::SnapshotPtr& s, const valc::QCControls& controls );
	void        addNodesForRules            ( const valc::SnapshotPtr& s, int controlResultID );
    std::string describe                    ( valc::ResultCode rc ) const;
	std::string describeControl             ( const QCViewData& d, const valc::QCControl& c ) const;
    std::string describeControlResult       ( const QCViewData& d, const valc::QCControl& c ) const;
	std::string describeTestResult          ( const valc::TestResult* r ) const;
	std::string describeWorklistEntry       ( const QCViewData& d, const valc::SnapshotPtr& s ) const;
	void        popNode                     ();
	void        pushNode                    ( const std::string& label, bool expand );
};

}

#endif


#ifndef QCViewControllerH
#define QCViewControllerH

#include "API.h"
#include "IdleServiceUserAdapter.h"
#include "ModelEventListenerAdapter.h"
#include <stack>
#include "TQCViewFrame.h"
#include <vector>

namespace valcui
{

class Model;


class QCViewController
{
public:
	QCViewController();
    IdleServiceUser*    getIdleServiceUserInterface();
    ModelEventListener* getModelEventListenerInterface();
    void init();
	void notify( int modelEvent, const EventData& eventData );
    void onIdle();
    void onResize();
    void setModel( Model* m );
    void setView( TQCViewFrame* view );
    void __fastcall update();

    class QCVDisplayMode
    {
    public:
        QCVDisplayMode();
        virtual ~QCVDisplayMode();
        virtual void doUpdate() = 0;
    };

    friend class QCVDisplayModeQC;
    friend class QCVDisplayModeUnknown;

private:
	TQCViewFrame*                               m_view;
	ModelEventListenerAdapter<QCViewController> m_eventListener;
    IdleServiceUserAdapter<QCViewController>    m_idleServiceUser;
    Model*                                      m_model;
	std::stack< TTreeViewItem* >                m_nodeStack;
	std::vector< TTreeViewItem* >               m_expandList;

    std::string describe( valc::ResultCode rc ) const;
    std::string describeWorklistEntry( const valc::WorklistEntry* wle, valc::SnapshotPtr snapshot ) const;
	void popNode();
	void pushNode( const std::string& label, bool expand );
};

}

#endif


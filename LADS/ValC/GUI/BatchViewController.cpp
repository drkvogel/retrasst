#include "BatchElementProfilerSimpleImpl.h"
#include "BatchRenderFunc.h"
#include "BatchViewController.h"
#include "Config.h"
#include <memory>
#include "Model.h"
#include "ModelEventConstants.h"
#include "Require.h"
#include "SABParser.h"
#include "SnapshotUtil.h"
#include "TBatchViewFrame.h"
#include "ValCDialogs.h"

namespace valcui
{

BatchViewController::BatchViewController( const paulst::Config* guiConfig )
    :
    m_idleServiceUser(this),
    m_eventListener(this),
    m_view(0),
	m_model(0),
    m_profileStyles( guiConfig->get("BatchElementProfileStyles") )
{
}

IdleServiceUser* BatchViewController::getIdleServiceUserInterface()
{
    return &m_idleServiceUser;
}

ModelEventListener* BatchViewController::getModelEventListenerInterface()
{
    return &m_eventListener;
}

void BatchViewController::init()
{
}

void BatchViewController::notify( int eventID, const EventData& d )
{
    if ( eventID == MODEL_EVENT::FORCE_RELOAD )
    {
        m_model->borrowSnapshot( update );
    }
}

void BatchViewController::onIdle()
{
}

void BatchViewController::onResize()
{
}

void BatchViewController::setModel( Model* m )
{
    m_model = m;
}

void BatchViewController::setView( TBatchViewFrame* view )
{
	m_view = view;
	m_scrollBox.setContainer( view->scrollBox );
}

void __fastcall BatchViewController::update()
{
	require( m_view );

    valc::SnapshotPtr snapshot( m_model->getSnapshot() );

	m_scrollBox.clear();

    TestNamesAdapter tna( snapshot );

    BatchElementProfilerSimpleImpl profiler;

    BatchRenderFunc f( m_scrollBox, TPANEL_HEIGHT, TPANEL_WIDTH, &tna, updateWorklistEntrySelection, &profiler, &m_profileStyles  );

    SABParser parser( snapshot );

	parser.parse(&f);

}

void __fastcall BatchViewController::updateWorklistEntrySelection( TObject* sender )
{
    TText* t = dynamic_cast<TText*>(sender);
    require( t );
    const int worklistEntryID = t->Tag;
    m_model->setSelectedWorklistEntry( worklistEntryID );
}

}


#include "BatchNavController.h"
#include "BatchNavViewRender.h"
#include <cstdlib>
#include <FMX.Objects.hpp>
#include "Model.h"
#include "ModelEventConstants.h"
#include "Require.h"
#include "SABParser.h"
#include "TBatchNavFrame.h"
#include "ValCDialogs.h"

namespace valcui
{

BatchNavController::BatchNavController()
    :
    m_view(0),
	m_model(0),
	m_selectedWorklistEntry(0),
    m_idleServiceUser(this),
    m_eventListener(this)
{
}

IdleServiceUser* BatchNavController::getIdleServiceUserInterface()
{
    return &m_idleServiceUser;
}

ModelEventListener* BatchNavController::getModelEventListenerInterface()
{
    return &m_eventListener;
}

void BatchNavController::init()
{
    if ( m_model->isSnapshotLoaded() )
    {
        m_model->borrowSnapshot( update );
    }
}

void BatchNavController::onIdle()
{
}

void BatchNavController::onResize()
{
}

void BatchNavController::notify( int modelEvent, const EventData& ed )
{
    if ( modelEvent == MODEL_EVENT::FORCE_RELOAD )
    {
        m_model->borrowSnapshot( update );
	}
	else if ( modelEvent == MODEL_EVENT::WORKLIST_ENTRY_SELECTION_CHANGE )
	{
		// The following condition is to protect against entering the
		// following block in cases when BatchNavController is itself
		// the cause of the worklist_entry_selection_change. Refer to
		// ::onClick
		if ( m_model->getSelectedWorklistEntry() != m_selectedWorklistEntry )
		{
			m_selectedWorklistEntry = m_model->getSelectedWorklistEntry();

			if ( m_worklistEntryOffsets.count( m_selectedWorklistEntry ) )
			{
				int worklistEntryOffset = m_worklistEntryOffsets[m_selectedWorklistEntry];
				const int topOfViewport = m_view->scrollBox->ViewportPosition.Y;
				const int bottomOfViewport = topOfViewport + m_view->scrollBox->Height;

				m_view->scrollBox->ScrollTo(
					0,
					- (worklistEntryOffset - topOfViewport) );

				/*
				std::ostringstream msg;

				msg << "topOfViewport: " << topOfViewport << "\n"
					<< "bottomOfViewport: " << bottomOfViewport << "\n"
					<< "worklistEntryOffset: " << worklistEntryOffset << "\n";

				showInfoMsg( msg.str() );
				*/
			}
		}
	}
}

void __fastcall BatchNavController::onClick( TObject* sender )
{
	TRectangle* r = dynamic_cast<TRectangle*>(sender);
	require( r );

	m_selectedWorklistEntry = r->Tag;
	m_model->setSelectedWorklistEntry( r->Tag );
}

void BatchNavController::setModel( Model* m )
{
    m_model = m;
}

void BatchNavController::setView( TBatchNavFrame* view )
{
    m_view = view;
}

void __fastcall BatchNavController::update()
{
	SABParser parser( m_model->getSnapshot() );
	m_worklistEntryOffsets.clear();
	BatchNavViewRender r( m_view, onClick, m_selectedWorklistEntry,
		&m_worklistEntryOffsets );
	parser.parse(&r);


}

}


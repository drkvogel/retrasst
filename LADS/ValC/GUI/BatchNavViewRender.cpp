#include <algorithm>
#include "BatchNavViewRender.h"
#include <FMX.Objects.hpp>
#include "Require.h"
#include "TBatchNavFrame.h"

namespace valcui
{

BatchNavViewRender::BatchNavViewRender( TBatchNavFrame* view,
	TNotifyEvent clickHandler,
	int selectedWorklistEntry,
	std::map<int,int>* pWorklistEntryOffsets )
	:
	m_view(view),
	m_clickHandler( clickHandler ),
	m_selectedWorklistEntry( selectedWorklistEntry ),
	m_rowCount(0),
	m_pWorklistEntryOffsets( pWorklistEntryOffsets ),
    x(0),
	y(0)
{
	require( m_pWorklistEntryOffsets );
}

void BatchNavViewRender::drawBox( int worklistEntryID )
{
	TScrollBox* sb = m_view->scrollBox;
	TRectangle* r = new TRectangle(sb);
	r->Tag = worklistEntryID;
	r->OnClick = m_clickHandler;
	r->Parent = sb;
	r->Position->X = x;
	r->Position->Y = y;
	r->Height = ROW_HEIGHT;
	r->Width = CELL_WIDTH;
	sb->AddObject(r);
	x+= CELL_WIDTH;
	m_pWorklistEntryOffsets->insert( std::make_pair( worklistEntryID, y ) );
}

void BatchNavViewRender::eof()
{
}

void BatchNavViewRender::group( bool QC, int groupID )
{
}

void BatchNavViewRender::run( const valc::IDToken& runID, const std::string& barcode, bool isOpen )
{
    x = 0;

    if ( 0 == m_rowCount++ )
    {
        y = 0;
    }
    else
    {
        y+= ROW_HEIGHT;
    }
}

void BatchNavViewRender::result(    const valc::TestResult* r,
                                    const paulst::Nullable<valc::RuleResults>& rr, 
                                    const valc::WorklistEntry* associatedWorklistEntry )
{
    drawBox( associatedWorklistEntry->getID() );
}

void BatchNavViewRender::pending( const valc::WorklistEntry* w )
{
	drawBox( w->getID() );
}

}


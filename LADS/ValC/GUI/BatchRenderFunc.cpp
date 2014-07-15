#include "BatchElementProfiler.h"
#include "BatchRenderFunc.h"
#include "Properties.h"
#include "Require.h"
#include "SnapshotUtil.h"

namespace valcui
{

BatchRenderFunc::BatchRenderFunc(   ScrollBox& container,
                                    int panelHeight,
                                    int panelWidth,
                                    TestNames* testNames,
                                    TNotifyEvent updateWorklistEntrySelection,
                                    BatchElementProfiler* profiler,
                                    paulst::Properties* profileStyles )
    :
    m_container  ( container   ),
    m_panelHeight( panelHeight ),
    m_panelWidth ( panelWidth  ),
    m_testNames  ( testNames   ),
    m_updateWorklistEntrySelection( updateWorklistEntrySelection ),
    m_profiler   ( profiler ),
    m_profileStyles( profileStyles ),
	m_x( 2 ),
    m_y( 0 ),
    m_runCount( 0 ),
    m_maxX    ( 0 ),
    m_groupID ( 0 ),
    m_isQCGroup( false )
{
}

void BatchRenderFunc::addBox( const std::string& text, const std::string& profile, const paulst::Nullable<int>& worklistEntryID )
{
	TPanel* p = m_container.addComponent();
    p->Height = m_panelHeight;
    p->Width  = m_panelWidth;
    p->Position->X = m_x;
	p->Position->Y = m_y;

	// Refer to
	// http://ifnotlost.blogspot.co.uk/2014/07/firemonkey-styles-interfere-with.html
    TPanel* p2 = new TPanel( p );
    p2->Align = TAlignLayout::alClient;
    p->AddObject(p2);

    const std::string configuredStyle = m_profileStyles->getValue( profile );
    p2->StyleLookup = configuredStyle.empty() ? "bv_default" : configuredStyle.c_str();

    TText* t = (TText*) p2->FindStyleResource("text");

    if ( t )
    {
        t->Text = text.c_str();

        if ( ! worklistEntryID.isNull() )
        {
            t->Tag     = worklistEntryID.get();
            t->OnClick = m_updateWorklistEntrySelection;
        }
    }

    m_x += m_panelWidth;

    if ( m_x > m_maxX )
    {
        m_maxX = m_x;
    }
}

void BatchRenderFunc::eof()
{
}

void BatchRenderFunc::group( bool QC, int groupID )
{
    m_groupID = groupID;
    m_isQCGroup = QC;
}

void BatchRenderFunc::run( const valc::IDToken& runID, const std::string& barcode, bool isOpen )
{
    m_x = 2;

    if ( 0 != m_runCount++ )
    {
        m_y += m_panelHeight;
    }

    addBox( paulst::toString( m_groupID ), m_profiler->characterizeGroup( isQC(barcode) ) );

    addBox( barcode, m_profiler->characterizeRun( isQC(barcode), isOpen ) );
}

void BatchRenderFunc::result(   const valc::TestResult* r,
                                const paulst::Nullable<valc::RuleResults>& rr, 
                                const valc::WorklistEntry* associatedWorklistEntry )
{
    addBox( m_testNames->getNameFor( r->getTestID() ), 
            m_profiler->characterizeResult( r, rr, associatedWorklistEntry ),
            paulst::Nullable<int>( associatedWorklistEntry->getID() ) );
}

void BatchRenderFunc::pending( const valc::WorklistEntry* w )
{
	addBox( 	m_testNames->getNameFor( w->getTestID() ),
				m_profiler->characterizePending( w ),
				paulst::Nullable<int>( w->getID() ) );
}

}


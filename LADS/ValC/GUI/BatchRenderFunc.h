#ifndef BATCHRENDERFUNCH
#define BATCHRENDERFUNCH

#include <FMX.Layouts.hpp>
#include <FMX.StdCtrls.hpp>
#include "FMXTemplates.h"
#include "Nullable.h"
#include "SAB.h"

namespace paulst
{
    class Properties;
}

namespace valcui
{

class BatchElementProfiler;
class TestNames;

typedef FMXContainer<TScrollBox,TPanel> ScrollBox;

class BatchRenderFunc : public SAB
{
public:
	BatchRenderFunc( 	ScrollBox& container,
                        int panelHeight,
                        int panelWidth,
                        TestNames* testNames,
                        TNotifyEvent updateWorklistEntrySelection,
                        BatchElementProfiler* profiler,
                        paulst::Properties* profileStyles );
    void eof      ();
    void group    (   bool QC, int groupID );
    void run      (   const valc::IDToken& runID, const std::string& barcode, bool isOpen );
    void result   (   const valc::TestResult* r,
                      const paulst::Nullable<valc::RuleResults>& rr, 
                      const valc::WorklistEntry* associatedWorklistEntry );
    void pending  (   const valc::WorklistEntry* );
private:
	ScrollBox&   		    m_container;
    const int               m_panelHeight;
    const int               m_panelWidth;
    TestNames* const        m_testNames;
    TNotifyEvent  const     m_updateWorklistEntrySelection;
    BatchElementProfiler*   m_profiler;
    const paulst::Properties* m_profileStyles;
    int                 m_x;
    int                 m_y;
    int                 m_runCount;
    int                 m_maxX;
    int                 m_groupID;
    bool                m_isQCGroup;

    void addBox(    const std::string& text, 
                    const std::string& profile = "",
                    const paulst::Nullable<int>& worklistEntryID = paulst::Nullable<int>() );
};

}
#endif


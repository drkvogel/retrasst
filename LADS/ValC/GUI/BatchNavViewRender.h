#ifndef BATCHNAVVIEWRENDERH
#define BATCHNAVVIEWRENDERH

#include <map>
#include "SAB.h"
#include <System.Classes.hpp>

class TBatchNavFrame;

namespace valcui
{

class BatchNavViewRender : public SAB
{
public:
	BatchNavViewRender( TBatchNavFrame* view,
						TNotifyEvent clickHandler,
						int selectedWorklistEntry,
						std::map<int,int>* pWorklistEntryOffsets );
    // SAB interface
    void eof      ();
    void group    (   bool QC, int groupID );
    void run      (   const valc::IDToken& runID, const std::string& barcode, bool isOpen );
    void result   (   const valc::TestResult* r,
                      const paulst::Nullable<valc::RuleResults>& rr, 
                      const valc::WorklistEntry* associatedWorklistEntry );
    void pending  (   const valc::WorklistEntry* );

private:
	TBatchNavFrame* const m_view;
	TNotifyEvent m_clickHandler;
	int m_rowCount;
	const int m_selectedWorklistEntry;
	int x,y;
	std::map<int,int>* m_pWorklistEntryOffsets;

	void drawBox( int worklistEntryID );

	static const int CELL_WIDTH = 5;
    static const int ROW_HEIGHT = 2;
};

}

#endif


//---------------------------------------------------------------------------

#ifndef RetrievalListMainListDialogH
#define RetrievalListMainListDialogH
//---------------------------------------------------------------------------
#include <System.Classes.hpp>
#include <Vcl.Controls.hpp>
#include <Vcl.StdCtrls.hpp>
#include <Vcl.Forms.hpp>
#include <Vcl.Grids.hpp>
#include <Vcl.ExtCtrls.hpp>
#include <Vcl.Menus.hpp>

#include <vector>
#include <map>

#include "RetrievalListCtrl.h"

#include <WorkerThread.h>


class bgWorkerThreadTask : public paulst::WorkerThreadTask
{
private:
	std::map<int,std::map<String,String> > *m_pWorkingDataSet;
	retrievalList *m_pRetrievalList;
	int *m_ProgessCounter;
public:
	bgWorkerThreadTask()
	{
	}

	~bgWorkerThreadTask()
	{
	}

	void setUp(std::map<int,std::map<String,String> > &workingDataSet,retrievalList *pRetrievalList, int &progessCounter)
	{
		m_ProgessCounter = &progessCounter;
		m_pWorkingDataSet = &workingDataSet;
		m_pRetrievalList = pRetrievalList;
	}

	bool execute()
	{
		m_pRetrievalList->fillCryovialStatus(*m_pWorkingDataSet,m_ProgessCounter,this);
		return true;
	}
};

//---------------------------------------------------------------------------
class TfrmRetrievalListDialog : public TForm
{
__published:	// IDE-managed Components
	TStringGrid *mainStringGrid;
	TTimer *Timer1;
	TPopupMenu *PopupMenu1;
	TMenuItem *MenuFindSiblings;
	TMenuItem *MenuSearchWithBarcode;
	TMenuItem *MenuSearchWithBarcodeAndAliquot;
	TMenuItem *MenuSearchWithSampleID;
	TMenuItem *MenuCryovialHistory;
	TMenuItem *MenuFindAnyAnalyses;
	TMenuItem *FindanyAnalysescryovialbarcode;
	TMenuItem *SearchWithCryovialSampleid;
	TTimer *AnimTimer;
	TMenuItem *MenuFindAnyAnalysesFromSameSource;
	TMenuItem *NotesSubMenu;
	TMenuItem *NotesFromResultsMenu;
	TMenuItem *NotesFromCryovialMenu;
	TMenuItem *NotesFromCryoStoreMenu;
	TMenuItem *NotesFromSpecimenMenu;
	TMenuItem *NotesFromBoxNameMenu;
	TMenuItem *Anal;
	TMenuItem *Searchforcryovialhistory1;
	TMenuItem *N4;
	TMenuItem *Rebuildlistusing;
	void __fastcall FormResize(TObject *Sender);
	void __fastcall Timer1Timer(TObject *Sender);
	void __fastcall mainStringGridDrawCell(TObject *Sender, int ACol, int ARow, TRect &Rect,
          TGridDrawState State);
	void __fastcall MenuSearchWithBarcodeClick(TObject *Sender);
	void __fastcall MenuSearchWithBarcodeAndAliquotClick(TObject *Sender);
	void __fastcall MenuSearchWithSampleIDClick(TObject *Sender);
	void __fastcall MenuFindAnyAnalysesClick(TObject *Sender);
	void __fastcall MenuCryovialHistoryClick(TObject *Sender);
	void __fastcall FindanyAnalysescryovialbarcodeClick(TObject *Sender);
	void __fastcall MenuFindSiblingsClick(TObject *Sender);
	void __fastcall PopupMenu1Popup(TObject *Sender);
	void __fastcall SearchWithCryovialSampleidClick(TObject *Sender);
	void __fastcall AnimTimerTimer(TObject *Sender);
	void __fastcall MenuFindAnyAnalysesFromSameSourceClick(TObject *Sender);
	void __fastcall mainStringGridMouseDown(TObject *Sender, TMouseButton Button, TShiftState Shift,
          int X, int Y);
	void __fastcall NotesFromResultsMenuClick(TObject *Sender);
	void __fastcall NotesFromCryovialMenuClick(TObject *Sender);
	void __fastcall NotesFromCryoStoreMenuClick(TObject *Sender);
	void __fastcall NotesFromSpecimenMenuClick(TObject *Sender);
	void __fastcall NotesFromBoxNameMenuClick(TObject *Sender);
	void __fastcall FormClose(TObject *Sender, TCloseAction &Action);

private:	// User declarations
	void displayGridStrings();
	void FillCryovialList(std::map<int,std::map<String,String> > &cryolist);

//	bgWorkerThreadTask m_WorkerThreadTask;
	paulst::WorkerThread *m_pWorkerThread;

	std::map<int,std::map<String,String> > m_cryoLists;
	retrievalList *m_pRetrievalList;

	int m_AnimCount;
    int m_BGProcessingCount;
public:		// User declarations
	void setup(retrievalList *retrievalList,std::map<String,String> &data);
	void setup(retrievalList *retrievalList,std::vector<String> &barcodes,String &project_id,String &aliquit_id);
	void setup(retrievalList *retrievalList,std::vector<String> &sample_barcodes,String &project_id);
	__fastcall TfrmRetrievalListDialog(TComponent* Owner);
	~TfrmRetrievalListDialog()
	{
		if (m_pWorkerThread!=NULL)
		{
			delete m_pWorkerThread;
			m_pWorkerThread = NULL;
		}
	}
};
//---------------------------------------------------------------------------
extern PACKAGE TfrmRetrievalListDialog *frmRetrievalListDialog;
//---------------------------------------------------------------------------
#endif

//---------------------------------------------------------------------------

#ifndef StoremainH
#define StoremainH
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
#include <ExtCtrls.hpp>
#include <DB.hpp>
#include <DBTables.hpp>
#include <map>
//---------------------------------------------------------------------------
class TfrmStoremain : public TForm
{
__published:	// IDE-managed Components
	TPanel *Panel1;
	TLabel *Label1;
	TButton *BtnConfigure;
	TButton *BtnBrowse;
	TButton *btnMove;
	TPanel *Panel2;
	TLabel *Label2;
	TButton *BtnXfer;
	TButton *BtnDiscard;
	TButton *BtnAnalyse;
	TButton *BtnReferred;
	TPanel *Panel3;
	TLabel *Label3;
	TButton *BtnCreate;
	TButton *BtnPrevious;
	TButton *BtnRetrieve;
	TPanel *Panel4;
	TLabel *lbBoxesReady;
	TLabel *lbForRemoval;
	TLabel *lbRemoved;
	TLabel *lbRetrievalLists;
	TTimer *timer;
	TLabel *Label4;
	TButton *BtnRationalyse;

	void __fastcall TreeBrowse(TObject *Sender);
	void __fastcall OnCreateList(TObject *Sender);
	void __fastcall ConfigureClick(TObject *Sender);
	void __fastcall BtnDiscardClick(TObject *Sender);
	void __fastcall timerTimer(TObject *Sender);
	void __fastcall FormShow(TObject *Sender);
	void __fastcall BtnXferClick(TObject *Sender);
	void __fastcall BtnAnalyseClick(TObject *Sender);
	void __fastcall btnMoveClick(TObject *Sender);
	void __fastcall BtnReferredClick(TObject *Sender);
    void __fastcall BtnRetrieveClick(TObject *Sender);

private:	// User declarations

	enum { STOPPED, TRANSFERS, JOBCOUNTS, FINISHED } updateStatus;

	unsigned boxesReady, moveJobs, retrievals, discards;

	unsigned nextProject;
	bool switchProject();
	void resetCounts();
	void updateCounts();
	void countBoxes();
	void countJobs();
	void showCounts();

public:		// User declarations

	__fastcall TfrmStoremain(TComponent* Owner);
};
//---------------------------------------------------------------------------
extern PACKAGE TfrmStoremain *frmStoremain;
//---------------------------------------------------------------------------
#endif

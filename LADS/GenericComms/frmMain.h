//---------------------------------------------------------------------------

#ifndef frmMainH
#define frmMainH
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
#include <ComCtrls.hpp>
#include <ExtCtrls.hpp>
#include <DB.hpp>
#include <DBTables.hpp>
#include <ADODB.hpp>
#include <Sysutils.hpp>
#include <Grids.hpp>
#include "LCDbAuditTrail.h"
#include "gcommsObjects.h"
#include <Buttons.hpp>
//---------------------------------------------------------------------------
class TformMain : public TForm
{
__published:	// IDE-managed Components
	TStatusBar *statusBar;
    TListBox *listboxWorklist;
    TPanel *Panel1;
    TDatabase *Database1;
    TQuery *qCentral;
    TQuery *qLocal;
	TButton *btnPopulateWorklist;
	TStringGrid *sgWLColumnHeaders;
	TGroupBox *GroupBox1;
	TComboBox *comboProjects;
	TGroupBox *GroupBox2;
	TButton *btnCreateSampleSelection;
	TGroupBox *GroupBox3;
	TButton *btnEnterResults;
	TGroupBox *groupboxManage;
	TButton *btnManage;
	TButton *btnGetWorklist;
	TBitBtn *BitBtn1;
    void __fastcall FormCreate(TObject *Sender);
    void __fastcall btnGetWorklistClick(TObject *Sender);
    void __fastcall btnCreateSampleSelectionClick(TObject *Sender);
    void __fastcall btnEnterResultsClick(TObject *Sender);
    void __fastcall btnManageClick(TObject *Sender);
	void __fastcall btnPopulateWorklistClick(TObject *Sender);
	void __fastcall FormShow(TObject *Sender);
	void __fastcall comboProjectsChange(TObject *Sender);
	void __fastcall BitBtn1Click(TObject *Sender);
private:	// User declarations
    LCDbAuditTrail *auditTrail;
    void fnLoadWorklist();
    void fnDeleteWorklistTotal(void);
    genericComms::tdvecpWorklistItem      vecpWorklistTotal;
    genericComms::tdvecpWorklistItem      vecpWorklistSelection;
public:		// User declarations
    __fastcall TformMain(TComponent* Owner);
};
//---------------------------------------------------------------------------
extern PACKAGE TformMain *formMain;
//---------------------------------------------------------------------------
#endif

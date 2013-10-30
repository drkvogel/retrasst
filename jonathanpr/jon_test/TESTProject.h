//---------------------------------------------------------------------------

#ifndef TESTProjectH
#define TESTProjectH
//---------------------------------------------------------------------------
#include <System.Classes.hpp>
#include <Vcl.Controls.hpp>
#include <Vcl.StdCtrls.hpp>
#include <Vcl.Forms.hpp>
#include <Vcl.Grids.hpp>

#include "events.h"
#include <System.Actions.hpp>
#include <Vcl.ActnCtrls.hpp>
#include <Vcl.ActnList.hpp>
#include <Vcl.ActnMan.hpp>
#include <Vcl.ActnMenus.hpp>
#include <Vcl.PlatformDefaultStyleActnCtrls.hpp>
#include <Vcl.ToolWin.hpp>
//---------------------------------------------------------------------------
class TForm1 : public TForm
{
__published:	// IDE-managed Components
	TStringGrid *StringGrid;
	TComboBox *EventCombo;
	TLabel *Label1;
	TComboBox *MemberCombobox;
	TButton *Button1;
	TActionMainMenuBar *ActionMainMenuBar1;
	TActionManager *ActionManager1;
	TAction *Action1;
	TAction *Action2;
	void __fastcall FormShow(TObject *Sender);
	void __fastcall AddEventClick(TObject *Sender);
	void __fastcall ComboChanged(TObject *Sender);
	void __fastcall Action1Execute(TObject *Sender);
	void __fastcall Action2Execute(TObject *Sender);
	void __fastcall Button1Click(TObject *Sender);
	void __fastcall StringGridSelectCell(TObject *Sender, int ACol, int ARow, bool &CanSelect);



private:	// User declarations
	events m_events;

	void FillEventList();
	void FillEventInfo();
	void FillMemberList();

public:		// User declarations
	__fastcall TForm1(TComponent* Owner);
};
//---------------------------------------------------------------------------
extern PACKAGE TForm1 *Form1;
//---------------------------------------------------------------------------
#endif

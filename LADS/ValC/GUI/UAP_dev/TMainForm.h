//---------------------------------------------------------------------------

#ifndef TMainFormH
#define TMainFormH
//---------------------------------------------------------------------------
#include <FMX.Controls.hpp>
#include <FMX.Forms.hpp>
#include <FMX.ListView.hpp>
#include <FMX.ListView.Types.hpp>
#include <FMX.Types.hpp>
#include <FMX.Layouts.hpp>
#include <FMX.Memo.hpp>
#include <FMX.StdCtrls.hpp>
//---------------------------------------------------------------------------

#include "ThreadPool.h"
#include "UserAdvisorAdapter.h"
#include "UserAdvisorPanel.h"
#include <System.Classes.hpp>
#include <FMX.Ani.hpp>
#include <FMX.Objects.hpp>

class TMainForm : public TForm
{
__published:	// IDE-managed Components
	TPanel *userAdvisorContainer;
	TRectAnimation *RectAnimation1;
	TRectangle *alarm;
private:	// User declarations
    valcui::UserAdvisorAdapter< valcui::UserAdvisorPanel > incomingMessages;
	valcui::UserAdvisorPanel userAdvisorPanel;
    stef::ThreadPool* m_threadPool;
public:		// User declarations
	__fastcall TMainForm(TComponent* Owner);
	__fastcall ~TMainForm();

    void __fastcall alarmOn();
    void __fastcall alarmOff();
};
//---------------------------------------------------------------------------
extern PACKAGE TMainForm *MainForm;
//---------------------------------------------------------------------------
#endif

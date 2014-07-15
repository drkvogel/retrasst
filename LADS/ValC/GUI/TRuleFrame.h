//---------------------------------------------------------------------------

#ifndef TRuleFrameH
#define TRuleFrameH
//---------------------------------------------------------------------------
#include <System.Classes.hpp>
#include <FMX.Controls.hpp>
#include <FMX.Forms.hpp>
#include <FMX.Layouts.hpp>
#include <FMX.Memo.hpp>
#include <FMX.StdCtrls.hpp>
#include <FMX.Types.hpp>
#include <FMX.Edit.hpp>
#include <FMX.Objects.hpp>
#include <FMX.TabControl.hpp>
//---------------------------------------------------------------------------
class TRuleFrame : public TFrame
{
__published:	// IDE-managed Components
	TPanel *controlPanel;
	TMemo *editor;
	TTabControl *controlPanelTabs;
	TTabItem *tabStart;
	TFlowLayout *tabStartFlow;
	TText *Text1;
	TEdit *tabStartTest;
	TFlowLayoutBreak *FlowLayoutBreak1;
	TText *Text2;
	TEdit *tabStartMachine;
	TFlowLayoutBreak *FlowLayoutBreak2;
	TText *Text3;
	TEdit *tabStartProject;
	TFlowLayoutBreak *FlowLayoutBreak3;
	TButton *viewConfiguredRuleButton;
private:	// User declarations
public:		// User declarations
	__fastcall TRuleFrame(TComponent* Owner);
};
//---------------------------------------------------------------------------
extern PACKAGE TRuleFrame *RuleFrame;
//---------------------------------------------------------------------------
#endif

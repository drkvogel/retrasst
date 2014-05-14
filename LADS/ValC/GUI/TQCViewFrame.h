//---------------------------------------------------------------------------

#ifndef TQCViewFrameH
#define TQCViewFrameH
//---------------------------------------------------------------------------
#include <System.Classes.hpp>
#include <FMX.Controls.hpp>
#include <FMX.Forms.hpp>
#include <FMX.StdCtrls.hpp>
#include <FMX.Types.hpp>
#include <FMX.Layouts.hpp>
#include <FMX.TreeView.hpp>
//---------------------------------------------------------------------------
class TQCViewFrame : public TFrame
{
__published:	// IDE-managed Components
	TPanel *titlePanel;
	TLabel *Label1;
	TTreeView *tree;
private:	// User declarations
public:		// User declarations
	__fastcall TQCViewFrame(TComponent* Owner);
};
//---------------------------------------------------------------------------
extern PACKAGE TQCViewFrame *QCViewFrame;
//---------------------------------------------------------------------------
#endif

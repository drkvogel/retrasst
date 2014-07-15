//---------------------------------------------------------------------------

#ifndef TBatchViewFrameH
#define TBatchViewFrameH
//---------------------------------------------------------------------------
#include <System.Classes.hpp>
#include <FMX.Controls.hpp>
#include <FMX.Forms.hpp>
#include <FMX.Menus.hpp>
#include <FMX.Types.hpp>
#include <FMX.StdCtrls.hpp>
#include <FMX.Layouts.hpp>
//---------------------------------------------------------------------------
class TBatchViewFrame : public TFrame
{
__published:	// IDE-managed Components
	TScrollBox *scrollBox;
private:	// User declarations
public:		// User declarations
	__fastcall TBatchViewFrame(TComponent* Owner);
};
//---------------------------------------------------------------------------
extern PACKAGE TBatchViewFrame *BatchViewFrame;
//---------------------------------------------------------------------------
#endif

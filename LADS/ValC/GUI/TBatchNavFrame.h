//---------------------------------------------------------------------------

#ifndef TBatchNavFrameH
#define TBatchNavFrameH
//---------------------------------------------------------------------------
#include <System.Classes.hpp>
#include <FMX.Controls.hpp>
#include <FMX.Forms.hpp>
#include <FMX.StdCtrls.hpp>
#include <FMX.Types.hpp>
#include <FMX.Layouts.hpp>
//---------------------------------------------------------------------------
class TBatchNavFrame : public TFrame
{
__published:	// IDE-managed Components
	TScrollBox *scrollBox;
private:	// User declarations
public:		// User declarations
	__fastcall TBatchNavFrame(TComponent* Owner);
};
//---------------------------------------------------------------------------
extern PACKAGE TBatchNavFrame *BatchNavFrame;
//---------------------------------------------------------------------------
#endif

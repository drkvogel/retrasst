//---------------------------------------------------------------------------

#ifndef TQCViewFrameH
#define TQCViewFrameH
//---------------------------------------------------------------------------
#include <System.Classes.hpp>
#include <FMX.Controls.hpp>
#include <FMX.Forms.hpp>
#include <FMX.StdCtrls.hpp>
#include <FMX.Types.hpp>
//---------------------------------------------------------------------------
class TQCViewFrame : public TFrame
{
__published:	// IDE-managed Components
	TLabel *Label1;
private:	// User declarations
public:		// User declarations
	__fastcall TQCViewFrame(TComponent* Owner);
};
//---------------------------------------------------------------------------
extern PACKAGE TQCViewFrame *QCViewFrame;
//---------------------------------------------------------------------------
#endif

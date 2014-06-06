//---------------------------------------------------------------------------

#ifndef TSampleRunFrameH
#define TSampleRunFrameH
//---------------------------------------------------------------------------
#include <System.Classes.hpp>
#include <FMX.Controls.hpp>
#include <FMX.Forms.hpp>
#include <FMX.Layouts.hpp>
#include <FMX.Objects.hpp>
#include <FMX.StdCtrls.hpp>
#include <FMX.Types.hpp>
#include <FMX.Menus.hpp>
//---------------------------------------------------------------------------
class TSampleRunFrame : public TFrame
{
__published:	// IDE-managed Components
	TPanel *titlePanel;
	TLabel *title;
	TPanel *barcodePanel;
	TText *Text1;
	TText *barcode;
	TText *Text3;
	TText *sampleID;
	TScrollBox *runContainer;
	TPopupMenu *popupMenu;
private:	// User declarations
public:		// User declarations
	__fastcall TSampleRunFrame(TComponent* Owner);
};
//---------------------------------------------------------------------------
extern PACKAGE TSampleRunFrame *SampleRunFrame;
//---------------------------------------------------------------------------
#endif

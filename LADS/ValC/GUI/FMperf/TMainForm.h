//---------------------------------------------------------------------------

#ifndef TMainFormH
#define TMainFormH
//---------------------------------------------------------------------------
#include <System.Classes.hpp>
#include <FMX.Controls.hpp>
#include <FMX.Forms.hpp>
#include <FMX.Layouts.hpp>
#include <FMX.StdCtrls.hpp>
#include <FMX.Types.hpp>
#include <FMX.Objects.hpp>
//---------------------------------------------------------------------------
class TMainForm : public TForm
{
__published:	// IDE-managed Components
	TPanel *panel;
	TScrollBox *scrollBox;
	TStyleBook *StyleBook2;
	void __fastcall onShow(TObject *Sender);
private:	// User declarations
	bool m_alreadyShown;
public:		// User declarations
	__fastcall TMainForm(TComponent* Owner);
};
//---------------------------------------------------------------------------
extern PACKAGE TMainForm *MainForm;
//---------------------------------------------------------------------------

template<class SubComponentType>
SubComponentType* addSubComponent( TFmxObject* container )
{
	SubComponentType* t = new SubComponentType( container );
	t->Parent = container;
	container->AddObject( t );
	return t;
}
#endif

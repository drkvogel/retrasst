//---------------------------------------------------------------------------
#ifndef ChooseDescriptor1H
#define ChooseDescriptor1H
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
#include <ExtCtrls.hpp>
//---------------------------------------------------------------------------
class TChooseDescriptor : public TForm
{
__published:	// IDE-managed Components
    TListBox *ChooseDescriptorBox;
    TLabel *Label1;
    TPanel *Panel1;
    TButton *OK;
    TButton *Cancel;
    void __fastcall OKClick(TObject *Sender);
    void __fastcall CancelClick(TObject *Sender);
    void __fastcall FormActivate(TObject *Sender);
    
private:	// User declarations
    int descripIndex;
public:		// User declarations
    __fastcall TChooseDescriptor(TComponent* Owner);
    void SetDescripIndex(int index){descripIndex = index;};
    int  GetDescripIndex(){return descripIndex;};
};
//---------------------------------------------------------------------------
extern PACKAGE TChooseDescriptor *ChooseDescriptor;
//---------------------------------------------------------------------------
#endif

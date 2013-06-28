//---------------------------------------------------------------------------
#ifndef ChooseDescriptorValue1H
#define ChooseDescriptorValue1H
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
#include <ExtCtrls.hpp>
//---------------------------------------------------------------------------
class TChooseDescriptorValue : public TForm
{
__published:	// IDE-managed Components
    TLabel *Label1;
    TListBox *lbxDescriptorValue;
    TPanel *Panel1;
    TButton *OK;
    TButton *Cancel;
    TLabel *lblDescriptor;
    TMemo *memMapInfo;
    void __fastcall FormActivate(TObject *Sender);
    void __fastcall OKClick(TObject *Sender);
    void __fastcall CancelClick(TObject *Sender);
private:	// User declarations
    int descriptorIndex;
    int valueID;
    AnsiString strMap;
public:		// User declarations
    __fastcall TChooseDescriptorValue(TComponent* Owner);
    void SetDescriptorIndex( int did ) { descriptorIndex = did; }
    void SetMapInfo( AnsiString sm ) { strMap = sm; }
    int GetValueID() { return valueID; }
};
//---------------------------------------------------------------------------
extern PACKAGE TChooseDescriptorValue *ChooseDescriptorValue;
//---------------------------------------------------------------------------
#endif

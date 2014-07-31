#ifndef RetrAsstWrongBarcodeH
#define RetrAsstWrongBarcodeH

#include <System.Classes.hpp>
#include <Vcl.Controls.hpp>
#include <Vcl.StdCtrls.hpp>
#include <Vcl.Forms.hpp>
#include <Vcl.Buttons.hpp>
#include <Vcl.ExtCtrls.hpp>

#include <string>

class TfrmWrongBarcode : public TForm {
__published:	// IDE-managed Components
    TGroupBox *groupSignOff;
    TEdit *editBarcode;
    TPanel *Panel1;
    TBitBtn *btnAddNote;
    TBitBtn *btnAccept;
    TBitBtn *btnDefer;
    TBitBtn *btnReplace;
    TGroupBox *groupBarcode;
    TLabel *labelExpected;
    TLabel *labelBarcode1;
    TLabel *labelBarcode2;
    TLabel *Label1;
    TLabel *Label2;
    TLabel *Label3;
    void __fastcall btnAcceptClick(TObject *Sender);
    void __fastcall btnDeferClick(TObject *Sender);
    void __fastcall btnAddNoteClick(TObject *Sender);
    void __fastcall FormShow(TObject *Sender);
private:
    std::string         expected, barcode1, barcode2;
public:
    __fastcall TfrmWrongBarcode(TComponent* Owner);
};

extern PACKAGE TfrmWrongBarcode *frmWrongBarcode;

#endif

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
    TLabel *labelPrevious;
    TLabel *labelEntered;
    TLabel *Label1;
    TLabel *Label2;
    TLabel *Label3;
    TLabel *Label4;
    TLabel *Label5;
    TLabel *labelReason;
    void __fastcall btnDeferClick(TObject *Sender);
    void __fastcall btnAddNoteClick(TObject *Sender);
    void __fastcall FormShow(TObject *Sender);
    void __fastcall editBarcodeKeyUp(TObject *Sender, WORD &Key, TShiftState Shift);
    void __fastcall btnAcceptClick(TObject *Sender);
    void __fastcall btnReplaceClick(TObject *Sender);

private:
    //void                accept(String entered);
    void                accept();
    void                refresh();
    void                flash();
public:
    std::string         expected, entered, previous;
    //String              expected, entered, previous;
    __fastcall TfrmWrongBarcode(TComponent* Owner);
};

extern PACKAGE TfrmWrongBarcode *frmWrongBarcode;

#endif

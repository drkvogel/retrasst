//---------------------------------------------------------------------------

#ifndef DebugCommsFrmH
#define DebugCommsFrmH
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>

// #include "NephBytes.h"
#include <ComCtrls.hpp>

//---------------------------------------------------------------------------
class TdebugComms : public TForm
{
__published:	// IDE-managed Components
    TEdit *analyserOutput;
    TButton *sendButton;
    TMemo *receivedBytes;
    TLabel *Label1;
    TButton *btnSendACK;
    TButton *btnSendNAK;
    TButton *btnSendENQ;
    TMemo *Memo1;
    TButton *btnSendHRecord;
    TButton *btnSendQRecord;
    TButton *btnSendLRecord;
    TLabel *Label2;
	TEdit *editBarcode;
    TButton *btnSendPRecord;
    TButton *btnSendORecord;
    TButton *btnSendRRecord;
    TEdit *editOContainer;
    TMemo *Memo2;
    TEdit *editOSID;
    TEdit *editOManu;
    TEdit *editOPriority;
    TEdit *editOAction;
    TLabel *Label3;
    TLabel *Label4;
    TEdit *editOSampleType;
    TLabel *Label5;
    TEdit *editOReportType;
    TLabel *Label6;
    TLabel *Label7;
    TLabel *Label8;
    TLabel *Label9;
    TButton *btnSendEOT;
    TStatusBar *StatusBar1;
    void __fastcall sendButtonClick(TObject *Sender);
    void __fastcall btnSendACKClick(TObject *Sender);
    void __fastcall btnSendNAKClick(TObject *Sender);
    void __fastcall btnSendENQClick(TObject *Sender);
    void __fastcall btnSendHRecordClick(TObject *Sender);
    void __fastcall btnSendQRecordClick(TObject *Sender);
    void __fastcall btnSendLRecordClick(TObject *Sender);
    void __fastcall btnSendPRecordClick(TObject *Sender);
    void __fastcall btnSendORecordClick(TObject *Sender);
    void __fastcall btnSendRRecordClick(TObject *Sender);
    void __fastcall btnSendEOTClick(TObject *Sender);
private:	// User declarations
//    neph::ByteString outBuffer;
public:		// User declarations
    __fastcall TdebugComms(TComponent* Owner);

    unsigned available();
    unsigned readBytes( unsigned char * buffer, unsigned count );
    unsigned sendBytes( const unsigned char * bytes, unsigned count );
    void sendMessage(String text);
    void printMessage(String message);
    void addMessage(String message);
};
//---------------------------------------------------------------------------
//extern PACKAGE TdebugComms *debugComms;
//---------------------------------------------------------------------------
#endif

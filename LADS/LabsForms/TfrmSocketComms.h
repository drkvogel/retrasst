//---------------------------------------------------------------------------

#ifndef TfrmSocketCommsH
#define TfrmSocketCommsH
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
#include "TfrmCommsConfig.h"
#include <Buttons.hpp>
#include <DB.hpp>
#include <DBTables.hpp>
#include <ExtCtrls.hpp>
#include <ScktComp.hpp>
//---------------------------------------------------------------------------

class TfrmNetComms : public TfrmCommsConfig
{
__published:	// IDE-managed Components
	TLabel *Label1;
	TEdit *tbPort;
	TServerSocket *serverSocket;
	TLabel *Label2;
	TEdit *tbRetries;
	TLabel *Label3;
	TEdit *tbRTT;
	
	void __fastcall FormClose(TObject *Sender, TCloseAction &Action);
	void __fastcall serverSocketClientConnect(TObject *Sender, TCustomWinSocket *Socket);
	void __fastcall serverSocketClientDisconnect(TObject *Sender, TCustomWinSocket *Socket);
	void __fastcall serverSocketClientError(TObject *Sender,
		  TCustomWinSocket *Socket, TErrorEvent ErrorEvent, int &ErrorCode);
	void __fastcall FormActivate(TObject *Sender);
	void __fastcall okButtonClick(TObject *Sender);

private:

	TCustomWinSocket *client;

public:

	__fastcall TfrmNetComms(TComponent* Owner);
	String open();
	unsigned available();
	unsigned readBytes( void * buffer, unsigned max );
	unsigned sendBytes( void * bytes, unsigned count );
	short getRetries() const { return tbRetries -> Text.ToIntDef( 3 ); }
	short getRoundTrip() const { return tbRTT -> Text.ToIntDef( 10 ); }
	void clear();
};
//---------------------------------------------------------------------------
extern PACKAGE TfrmNetComms *frmNetComms;
//---------------------------------------------------------------------------
#endif

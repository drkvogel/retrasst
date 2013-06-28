//---------------------------------------------------------------------------
#ifndef CreateProjectDatabase1H
#define CreateProjectDatabase1H
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
#include <Buttons.hpp>
#include <ScktComp.hpp>
//---------------------------------------------------------------------------
#include "rexec_ob.h"
#include <ExtCtrls.hpp>
//---------------------------------------------------------------------------
class TCreateProjectDatabase : public TForm
{
__published:	// IDE-managed Components
    TMemo *ReplyMemo;
    TClientSocket *RexecSocket;
    TPanel *Panel1;
    TButton *btnClose;
    void __fastcall RexecSocketConnect(TObject *Sender,
          TCustomWinSocket *Socket);
    void __fastcall RexecSocketRead(TObject *Sender,
          TCustomWinSocket *Socket);
    void __fastcall RexecSocketDisconnect(TObject *Sender,
          TCustomWinSocket *Socket);
	void __fastcall FormCreate(TObject *Sender);
	void __fastcall FormDestroy(TObject *Sender);
    void __fastcall btnCloseClick(TObject *Sender);
    void __fastcall FormActivate(TObject *Sender);
private:	// User declarations
    AnsiString RexecOutgoingStr, RexecIncomingStr;
    REXEC_OB	*rob;
	AnsiString dbName, dbMajorVersion, dbMinorVersion;
	TCursor	initial_cursor;
public:		// User declarations
    __fastcall TCreateProjectDatabase(TComponent* Owner);
	void SetDatabaseName( String name ) { dbName = name; }
	void SetDatabaseVersion( String versionString );
};
//---------------------------------------------------------------------------
extern PACKAGE TCreateProjectDatabase *CreateProjectDatabase;
//---------------------------------------------------------------------------
#endif

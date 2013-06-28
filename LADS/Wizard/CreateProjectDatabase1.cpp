//---------------------------------------------------------------------------
#include <vcl.h>
#pragma hdrstop

#include "CreateProjectDatabase1.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"
TCreateProjectDatabase *CreateProjectDatabase;
//---------------------------------------------------------------------------
__fastcall TCreateProjectDatabase::TCreateProjectDatabase(TComponent* Owner)
    : TForm(Owner)
{
}
//---------------------------------------------------------------------------

void __fastcall TCreateProjectDatabase::FormCreate(TObject *Sender)
{
	rob = new REXEC_OB( RexecSocket );
}
//---------------------------------------------------------------------------

void __fastcall TCreateProjectDatabase::FormActivate(TObject *Sender)
{
	btnClose->Enabled = false;  	// DISABLE BUTTON
	ReplyMemo->Lines->Clear();  		// CLEAR MEMO
	initial_cursor = Screen->Cursor;
    Screen->Cursor = crSQLWait;

	rob->set_address( "u4" );
	rob->set_username( "lust" );
	rob->set_password( "2baby" );
    AnsiString command = "./version" + dbMajorVersion + "." + dbMinorVersion +
        "/create_db.sh " + dbName;
	rob->execute( command );
}
//---------------------------------------------------------------------------
  /* When the connection to the rexec server is made, send command string */
void __fastcall TCreateProjectDatabase::RexecSocketConnect(TObject *Sender,
      TCustomWinSocket *Socket)
{
	rob->OnConnect_handler( Socket );
}
//---------------------------------------------------------------------------

void TCreateProjectDatabase::SetDatabaseVersion( String versionString )
{
	int point = versionString.Pos( "." );
	if( point == 0 )
	{
		dbMajorVersion = versionString;
		dbMinorVersion = 0;
	}
	else
	{	dbMajorVersion = versionString.SubString( 1, point - 1 );
		dbMinorVersion = versionString.SubString( point + 1, 2 );
	}
}

//---------------------------------------------------------------------------

void __fastcall TCreateProjectDatabase::RexecSocketRead(TObject *Sender,
      TCustomWinSocket *Socket)
{
	String	answer = rob->OnRead_handler( Socket );		// READ INCOMING DATA

    // add one line at at time to memo
    // instead of entire answer
    // ACC, 11/04/03

    AnsiString buffer = answer;
    AnsiString line;
    int poscr = buffer.AnsiPos( "\n" );
    while ( poscr != 0 )
    {
        line = buffer.SubString( 1, poscr-1 );
	    ReplyMemo->Lines->Add( line );
        buffer = buffer.SubString( poscr + 1, buffer.Length() );
        poscr = buffer.AnsiPos( "\n" );
    }
    if ( buffer != NULL )
    {
        ReplyMemo->Lines->Add( buffer );
    }
}
//---------------------------------------------------------------------------
  /* The connection has dropped. Say so and re-enable our button */
void __fastcall TCreateProjectDatabase::RexecSocketDisconnect(TObject *Sender,
      TCustomWinSocket *Socket)
{
	ReplyMemo->Lines->Add( "---- Connection dropped ----" );
	btnClose->Enabled = true;
	Screen->Cursor = initial_cursor;
}
//---------------------------------------------------------------------------

void __fastcall TCreateProjectDatabase::btnCloseClick(TObject *Sender)
{
    // write output to log file
    AnsiString message =  dbName + " project database created";
    Close();
}
//---------------------------------------------------------------------------

void __fastcall TCreateProjectDatabase::FormDestroy(TObject *Sender)
{
	delete rob;
}
//---------------------------------------------------------------------------


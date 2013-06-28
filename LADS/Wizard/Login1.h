//---------------------------------------------------------------------------
#ifndef Login1H
#define Login1H

//---------------------------------------------------------------------------

#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
#include <ExtCtrls.hpp>
#include <Db.hpp>
#include <DBTables.hpp>
#include <fstream.h>

//---------------------------------------------------------------------------
class TLogin : public TForm
{
__published:	// IDE-managed Components

	TPanel *ProjectPanel;
	TLabel *Label2;
	TComboBox *cbProjectName;
	TButton *BtnCentralDb;
	TButton *BtnProjectDb;
	TDatabase *pDatabase;
	TDatabase *dDatabase;
	TPanel *Panel1;
	TButton *Button1;
	TQuery *QueryProject;

	void __fastcall BtnCentralDbClick(TObject *Sender);
	void __fastcall BtnProjectDbClick(TObject *Sender);
	void __fastcall FormActivate(TObject *Sender);
	void __fastcall Button1Click(TObject *Sender);
	void __fastcall cbProjectNameChange(TObject *Sender);

private:	// User declarations

public:		// User declarations

	__fastcall TLogin(TComponent* Owner);
	String setProjectCaption( const String & formName ) const;
};

//---------------------------------------------------------------------------
extern PACKAGE TLogin *Login;
//---------------------------------------------------------------------------

#endif

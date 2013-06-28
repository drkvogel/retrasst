//---------------------------------------------------------------------------

#ifndef AddProject1H
#define AddProject1H

#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
#include <ComCtrls.hpp>
#include <ExtCtrls.hpp>
#include <Db.hpp>
#include <DBTables.hpp>
#include <NMFtp.hpp>
#include <Psock.hpp>

//---------------------------------------------------------------------------

class TAddProject : public TForm
{
__published:	// IDE-managed Components

	TPanel *Panel1;
	TLabel *Label2;
	TLabel *Label3;
	TLabel *Label4;
	TLabel *Label5;
	TLabel *Label6;
	TLabel *Label8;
	TEdit *ProjectDatabase;
	TLabel *Label9;
	TButton *Save;
	TButton *Cancel;
	TLabel *Label1;
	TEdit *ProjectName;
	TMemo *FullName;
	TMemo *memURL;
	TRadioGroup *rgStatus;
	TEdit *edtDbVersion;
	TLabel *Label10;
	TQuery *QueryCentral;
	TLabel *Label13;
	TLabel *Label14;
	TDateTimePicker *dtpFrom;
	TDateTimePicker *dtpTo;
	TDatabase *testDatabase;
	TButton *btnCreateDatabase;

	void __fastcall SaveClick(TObject *Sender);
	void __fastcall FormActivate(TObject *Sender);
	void __fastcall CancelClick(TObject *Sender);
	void __fastcall btnCreateDatabaseClick(TObject *Sender);

private:	// User declarations

	enum { RGS_LIVE, RGS_TEST, RGS_INACTIVE };     	// rgStatus buttons
	int projectID;

	String FindNextDatabaseName();

public:		// User declarations

	__fastcall TAddProject(TComponent* Owner);
	void setProject( int projID ) { projectID = projID; }
};
//---------------------------------------------------------------------------
extern PACKAGE TAddProject *AddProject;
//---------------------------------------------------------------------------
#endif

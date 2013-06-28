//---------------------------------------------------------------------------

#ifndef UtilsH
#define UtilsH
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
#include <ExtCtrls.hpp>
#include <Db.hpp>
#include <DBTables.hpp>

#include <fstream>

#include "FormCommon.h"
//---------------------------------------------------------------------------

#define APPMSG(x, y, z) Application->MessageBox(String(x).c_str(), y, z)

#ifdef _DEBUG
 #define DBGTXT(x) frmUtils->ShowText(x)
 #define MYDEBUG 1
#else
 #define DBGTXT(x) //
 #define MYDEBUG 0
#endif

static String sql, display;

class TfrmUtils : public TForm
{
__published:	// IDE-managed Components
    TPanel *Panel1;
    TMemo *MemoDebug;
    TMemo *MemoErr;
    TLabel *Label1;
    TLabel *Label2;
private:	// User declarations
public:		// User declarations
    __fastcall TfrmUtils(TComponent* Owner);

    bool query_open(TQuery* qry, bool log);
    bool query_exec(TQuery* qry, bool log);
    bool query_open(TQuery* qry);
    bool query_exec(TQuery* qry);

	void ShowText(AnsiString text);
	static String StripQuotes(String theString);
	static String StripChars(String TheString, String TheChars);
};
//---------------------------------------------------------------------------
extern PACKAGE TfrmUtils *frmUtils;
//---------------------------------------------------------------------------
#endif

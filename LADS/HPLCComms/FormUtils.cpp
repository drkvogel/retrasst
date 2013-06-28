//---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop

#include "FormUtils.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"
TfrmUtils *frmUtils;

//---------------------------------------------------------------------------
__fastcall TfrmUtils::TfrmUtils(TComponent* Owner)
    : TForm(Owner)
{
}
//---------------------------------------------------------------------------
bool TfrmUtils::query_exec(TQuery *qry) {
    return query_exec(qry, true);
}

bool TfrmUtils::query_open(TQuery *qry) {
    return query_open(qry, true);
}

bool TfrmUtils::query_exec(TQuery *qry, bool log)
{
    String display;
	bool	attempt = true;
	bool	success = false;
	qry->Close();
	try {
        do {
            Screen->Cursor = crSQLWait;

			try	{
                if (log) {
                    ShowText(qry->SQL->Text);
				}
                qry->ExecSQL();
				success = true;
            } catch ( Exception &e ) {
				Screen->Cursor = crDefault;
                display = "SQL Exec failure:\n\n" + qry->SQL->Text + "\n\n"
                            + e.Message + "\n\nAttempt again?";
                attempt = (IDYES == Application->MessageBox(display.c_str(), NULL, MB_YESNO));
				XMLFile::logError( __FUNC__, qry->SQL->Text );
			}
		} while ( attempt && ! success );
	}
	__finally {
        Screen->Cursor = crDefault;
	}

	return( success );
}

bool TfrmUtils::query_open(TQuery *qry, bool log)
{
    String display;

	bool attempt = true;

	qry->Close();

	try	{
        do {
            Screen->Cursor = crSQLWait;

			try	{
                if (log) {
                    ShowText(qry->SQL->Text);
				}
                qry->Open();
            } catch ( Exception &e ) {
                display = "SQL Exec failure:\n\n" + qry->SQL->Text + "\n\n"
                            + e.Message + "\n\nAttempt again?";
                attempt = (IDYES == Application->MessageBox(display.c_str(), NULL, MB_YESNO));
				XMLFile::logError( __FUNC__, qry->SQL->Text );
			}
        }
			while ( attempt && ! qry->Active );
    } __finally	{
        Screen->Cursor = crDefault;
    }

	return(qry->Active);
}

void TfrmUtils::ShowText(AnsiString text)
{
//    MemoDebug->Lines->Add("");
    MemoDebug->Lines->Add(text);
}

String TfrmUtils::StripQuotes(String theString)
{
    for (int i=1; i<=theString.Length(); i++) {
        if ('\'' == theString[i]) {
            theString.Delete(i, 1);
            //DBGTXT("Strip!", NULL, MB_OK); //???
        }
    }
    return theString;
}

//---------------------------------------------------------------------------



String TfrmUtils::StripChars(String TheString, String TheChars)
{
    int pos;
    for (int i=1; i<=TheChars.Length(); i++) {    //??? Length is length in bytes, not chars
        while (0 != (pos = TheString.AnsiPos(TheChars[i])))
            TheString.Delete(pos, 1);
    }
    return TheString;
}

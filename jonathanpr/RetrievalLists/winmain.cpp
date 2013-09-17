//---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop
#include <tchar.h>
//---------------------------------------------------------------------------
USEFORM("pwdDialog.cpp", PasswordDialog);
USEFORM("TcryovialHistoryDialog.cpp", cryovialHistoryDialog);
USEFORM("mainDialog.cpp", Form1);
USEFORM("mainListDialog.cpp", listDialog);
USEFORM("TDbSelectionDialog.cpp", DbSelectionDialog);
USEFORM("TResultNotesDialog.cpp", ResultNotesDialog);
USEFORM("TResultsDialog.cpp", ResultsDialog);
//---------------------------------------------------------------------------
int WINAPI _tWinMain(HINSTANCE, HINSTANCE, LPTSTR, int)
{
	try
	{
		Application->Initialize();
		Application->MainFormOnTaskBar = true;
		Application->CreateForm(__classid(TForm1), &Form1);
		Application->CreateForm(__classid(TPasswordDialog), &PasswordDialog);
		Application->CreateForm(__classid(TlistDialog), &listDialog);
		Application->CreateForm(__classid(TResultsDialog), &ResultsDialog);
		Application->CreateForm(__classid(TcryovialHistoryDialog), &cryovialHistoryDialog);
		Application->CreateForm(__classid(TResultNotesDialog), &ResultNotesDialog);
		Application->CreateForm(__classid(TDbSelectionDialog), &DbSelectionDialog);
		Application->Run();
	}
	catch (Exception &exception)
	{
		Application->ShowException(&exception);
	}
	catch (...)
	{
		try
		{
			throw Exception("");
		}
		catch (Exception &exception)
		{
			Application->ShowException(&exception);
		}
	}
	return 0;
}
//---------------------------------------------------------------------------

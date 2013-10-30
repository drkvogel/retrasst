//---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop
#include <tchar.h>
//---------------------------------------------------------------------------
USEFORM("TESTProject.cpp", Form1);
USEFORM("passwordForm.cpp", Password);
USEFORM("NewMemberDialog.cpp", NewMemberDlg);
USEFORM("NewEventDialog.cpp", eventDialog);
//---------------------------------------------------------------------------
#include "rosetta.h"


int WINAPI _tWinMain(HINSTANCE, HINSTANCE, LPTSTR, int)
{
	try
	{
		Application->Initialize();
		Application->MainFormOnTaskBar = true;
		Application->CreateForm(__classid(TForm1), &Form1);
		Application->CreateForm(__classid(TPassword), &Password);
		Application->CreateForm(__classid(TNewMemberDlg), &NewMemberDlg);
		Application->CreateForm(__classid(TeventDialog), &eventDialog);
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

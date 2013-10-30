//---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop
#include <tchar.h>
//---------------------------------------------------------------------------
USEFORM("signIn.cpp", tSignIn);
USEFORM("tmain.cpp", Form1);
USEFORM("tdbSelection.cpp", DbSelectionDialog);
USEFORM("tCreateRetrievalJob.cpp", CreateRetrievalJobForm);
USEFORM("boxSelection.cpp", tBoxSelection);
//---------------------------------------------------------------------------
int WINAPI _tWinMain(HINSTANCE, HINSTANCE, LPTSTR, int)
{
	try
	{
		Application->Initialize();
		Application->MainFormOnTaskBar = true;
		Application->CreateForm(__classid(TForm1), &Form1);
		Application->CreateForm(__classid(TDbSelectionDialog), &DbSelectionDialog);
		Application->CreateForm(__classid(TtSignIn), &tSignIn);
		Application->CreateForm(__classid(TCreateRetrievalJobForm), &CreateRetrievalJobForm);
		Application->CreateForm(__classid(TtBoxSelection), &tBoxSelection);
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

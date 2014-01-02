//---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop
#include <tchar.h>
//---------------------------------------------------------------------------
USEFORM("createExercise.cpp", retrivalExerciseDialog);
USEFORM("tdbSelection.cpp", DbSelectionDialog);
USEFORM("tmain.cpp", Form1);
USEFORM("signIn.cpp", tSignIn);
USEFORM("tCreateRetrievalJob.cpp", CreateRetrievalJobForm);
USEFORM("boxSelection.cpp", tBoxSelection);
USEFORM("boxType.cpp", tBoxType);
USEFORM("PleaseWait.cpp", tPleaseWait);
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
		Application->CreateForm(__classid(TtBoxType), &tBoxType);
		Application->CreateForm(__classid(TretrivalExerciseDialog), &retrivalExerciseDialog);
		Application->CreateForm(__classid(TtPleaseWait), &tPleaseWait);
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

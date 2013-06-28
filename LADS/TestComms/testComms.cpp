//---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop

USEFORM("testMain.cpp", testForm);
USEFORM("..\LabsForms\TfrmLogin.cpp", frmLogin);
USEFORM("..\LabsForms\TfrmPassword.cpp", frmPassword);
//---------------------------------------------------------------------------
WINAPI WinMain(HINSTANCE, HINSTANCE, LPSTR, int)
{
	try
	{   Application->Initialize();
		Application->Title = "Dummy Analyser";
		Application->CreateForm(__classid(TfrmLogin), &frmLogin);
		Application->CreateForm(__classid(TtestForm), &testForm);
		Application->CreateForm(__classid(TfrmPassword), &frmPassword);
		Application->Run();
	}
	catch (Exception &exception)
	{
		Application->ShowException(&exception);
	}
	return 0;
}

//---------------------------------------------------------------------------

//---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop
USEFORM("..\LabsForms\TfrmLoginBase.cpp", frmLoginBase);
USEFORM("..\LabsForms\TfrmPassword.cpp", frmPassword);
USEFORM("..\LabsForms\BuddyLogin.cpp", frmBuddyLogin);
USEFORM("frmMain.cpp", formMain);
USEFORM("frmClipboard.cpp", formClipboard);
USEFORM("frmEnterResults.cpp", formEnterResults);
USEFORM("frmManage.cpp", formManage);
USEFORM("frmEditFormat.cpp", formEditFormat);
USEFORM("..\LabsForms\TfrmAboutBox.cpp", frmAboutBox);
USEFORM("frmTest.cpp", FormTest);
//---------------------------------------------------------------------------
WINAPI WinMain(HINSTANCE, HINSTANCE, LPSTR, int)
{
    try
    {
         Application->Initialize();
         Application->Title = "Generic Communicator";
		Application->CreateForm(__classid(TfrmBuddyLogin), &frmBuddyLogin);
		Application->CreateForm(__classid(TformMain), &formMain);
		Application->CreateForm(__classid(TfrmAboutBox), &frmAboutBox);
		Application->CreateForm(__classid(TfrmPassword), &frmPassword);
		Application->CreateForm(__classid(TformClipboard), &formClipboard);
		Application->CreateForm(__classid(TformEnterResults), &formEnterResults);
		Application->CreateForm(__classid(TformManage), &formManage);
		Application->CreateForm(__classid(TformEditFormat), &formEditFormat);
		Application->CreateForm(__classid(TFormTest), &FormTest);
		Application->Run();
	}
    catch (Exception &exception)
    {
         Application->ShowException(&exception);
    }
    return 0;
}
//---------------------------------------------------------------------------

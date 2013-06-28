//---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop
USEFORM("frmCommon.cpp", fmCommon);
USEFORM("fmMain.cpp", frmMain);
USEFORM("..\LabsForms\TfrmAboutBox.cpp", frmAboutBox);
USEFORM("..\LabsForms\TfrmPassword.cpp", frmPassword);
USEFORM("..\LabsForms\TfrmLoginBase.cpp", frmLoginBase);
USEFORM("fmResolve.cpp", frmResolve);
USEFORM("fmLogin.cpp", frmLoginBoxBridge);
USEFORM("fmReport.cpp", frmReport);
//---------------------------------------------------------------------------
WINAPI WinMain(HINSTANCE, HINSTANCE, LPSTR, int)
{
    try
    {
         Application->Initialize();
         Application->Title = "Storage Sync";
         Application->CreateForm(__classid(TfrmLoginBoxBridge), &frmLoginBoxBridge);
		Application->CreateForm(__classid(TfrmMain), &frmMain);
		Application->CreateForm(__classid(TfmCommon), &fmCommon);
		Application->CreateForm(__classid(TfrmAboutBox), &frmAboutBox);
		Application->CreateForm(__classid(TfrmResolve), &frmResolve);
		Application->CreateForm(__classid(TfrmPassword), &frmPassword);
		Application->CreateForm(__classid(TfrmReport), &frmReport);
		Application->Run();
    }
    catch (Exception &exception)
    {
         Application->ShowException(&exception);
    }
    return 0;
}
//---------------------------------------------------------------------------

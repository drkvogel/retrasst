//---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop
USEFORM("FormFront.cpp", frmFront);
USEFORM("FormUtils.cpp", frmUtils);
USEFORM("FormCutNPaste.cpp", frmClipboard);
USEFORM("FormCommon.cpp", frmCommon);
USEFORM("FormEnterResults.cpp", frmEnterResults);
USEFORM("..\LabsForms\TfrmAboutBox.cpp", frmAboutBox);
//---------------------------------------------------------------------------
WINAPI WinMain(HINSTANCE, HINSTANCE, LPSTR, int)
{
    try
    {
         Application->Initialize();
         Application->Title = "HPLC Communicator";
         Application->CreateForm(__classid(TfrmFront), &frmFront);
		Application->CreateForm(__classid(TfrmUtils), &frmUtils);
		Application->CreateForm(__classid(TfrmClipboard), &frmClipboard);
		Application->CreateForm(__classid(TfrmEnterResults), &frmEnterResults);
		Application->CreateForm(__classid(TfrmCommon), &frmCommon);
		Application->CreateForm(__classid(TfrmAboutBox), &frmAboutBox);
		Application->Run();
    }
    catch (Exception &exception)
    {
         Application->ShowException(&exception);
    }
    return 0;
}
//---------------------------------------------------------------------------

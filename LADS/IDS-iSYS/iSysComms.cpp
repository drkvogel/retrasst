//---------------------------------------------------------------------------

#include <vcl.h>
//#include "isysMainForm.h"
#pragma hdrstop
//---------------------------------------------------------------------------
USEFORM("..\LabsForms\TfrmAboutBox.cpp", frmAboutBox);
USEFORM("..\LabsForms\TfrmCommsMain.cpp", mainForm);
USEFORM("..\LabsForms\TfrmCommsConfig.cpp", frmSerialConfig);
USEFORM("..\LabsForms\TfrmSerialConfig.cpp", frmSerialConfig);
USEFORM("isysMainForm.cpp", isysMainForm);
//---------------------------------------------------------------------------
WINAPI WinMain(HINSTANCE, HINSTANCE, LPSTR, int)
{
	try
	{
		Application->Initialize();
		//SetApplicationMainFormOnTaskBar(Application, true);
		Application->Title = "IDS-iSYS Comms";
		 Application->CreateForm(__classid(TisysMainForm), &isysMainForm);
		Application->CreateForm(__classid(TfrmAboutBox), &frmAboutBox);
		Application->CreateForm(__classid(TfrmSerialConfig), &frmSerialConfig);
		Application->Run();
	}
	catch (Exception &exception)
	{
		Application->ShowException(&exception);
	}
	return 0;
}
//---------------------------------------------------------------------------

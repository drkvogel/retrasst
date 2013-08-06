//---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop
#include <tchar.h>

//---------------------------------------------------------------------------



















USEFORM("DiscardSamples.cpp", frmDiscardSamples);
USEFORM("DiscardSearch.cpp", frmSearch);
USEFORM("DiscardReason.cpp", frmDiscardReason);
USEFORM("DiscardNote.cpp", frmDiscardNote);
USEFORM("DiscardPrologue.cpp", frmDiscardPrologue);
USEFORM("FindSample.cpp", frmFind);
USEFORM("DiscardMethod.cpp", frmDiscardMethod);
USEFORM("AnalyseSamples.cpp", frmAnalyseSpecimens);
USEFORM("AddSpecimens.cpp", frmAddSpecimens);
USEFORM("BoxTransfer.cpp", frmBoxList);
USEFORM("BrowseSample.cpp", frmBrowse);
USEFORM("Configure.cpp", frmConfigure);
USEFORM("MoveJobs.cpp", frmSelectJob);
USEFORM("ShowSamples.cpp", frmAliquotTypes);
USEFORM("SMLogin.cpp", frmLogin);
USEFORM("SelectSamples.cpp", frmSelectBoxes);
USEFORM("SampleMove.cpp", frmMove);
USEFORM("Temp.cpp", frmTest);
USEFORM("Storemain.cpp", frmStoremain);
USEFORM("SampleEntry.cpp", frmRetrieveMain);
USEFORM("NewTank.cpp", frmNewTank);
USEFORM("NewType.cpp", frmNewType);
USEFORM("NewSite.cpp", frmNewSite);
USEFORM("NewJob.cpp", frmNewJob);
USEFORM("NewSection.cpp", frmNewSection);
USEFORM("ReferredBoxes.cpp", frmReferred);
USEFORM("RetrievalAssistantProcess.cpp", frmProcess);
USEFORM("RetrieveSamples.cpp", frmRetrieved);
USEFORM("RetrievalAssistantBoxes.cpp", frmBoxes);
USEFORM("ReferredBoxesSummary.cpp", frmReferredBoxesSummary);
USEFORM("RetrievalAssistant.cpp", frmRetrievalAssistant);
USEFORM("..\LabsForms\TfrmConfirm.cpp", frmConfirm);
USEFORM("..\LabsForms\TfrmLoginBase.cpp", frmLoginBase);
USEFORM("..\LabsForms\TfrmAboutBox.cpp", frmAboutBox);
USEFORM("..\LabsForms\dvSelector.cpp", selectorFrame); /* TFrame: File Type */
USEFORM("..\LabsForms\TfrmPassword.cpp", frmPassword);
USEFORM("RetrievalAssistantSamples.cpp", frmSamples);
//---------------------------------------------------------------------------
WINAPI _tWinMain(HINSTANCE, HINSTANCE, LPTSTR, int)
{
	try
	{	Application->Initialize();
		Application->MainFormOnTaskBar = true;
		Application->Title = "Storage Management";
		Application->CreateForm(__classid(TfrmLogin), &frmLogin);
         Application->CreateForm(__classid(TfrmBoxList), &frmBoxList);
         Application->CreateForm(__classid(TfrmBrowse), &frmBrowse);
         Application->CreateForm(__classid(TfrmConfigure), &frmConfigure);
         Application->CreateForm(__classid(TfrmDiscardMethod), &frmDiscardMethod);
         Application->CreateForm(__classid(TfrmDiscardNote), &frmDiscardNote);
         Application->CreateForm(__classid(TfrmDiscardPrologue), &frmDiscardPrologue);
         Application->CreateForm(__classid(TfrmDiscardReason), &frmDiscardReason);
         Application->CreateForm(__classid(TfrmDiscardSamples), &frmDiscardSamples);
         Application->CreateForm(__classid(TfrmSearch), &frmSearch);
         Application->CreateForm(__classid(TfrmNewJob), &frmNewJob);
         Application->CreateForm(__classid(TfrmSelectJob), &frmSelectJob);
         Application->CreateForm(__classid(TfrmNewSection), &frmNewSection);
         Application->CreateForm(__classid(TfrmNewType), &frmNewType);
         Application->CreateForm(__classid(TfrmNewTank), &frmNewTank);
         Application->CreateForm(__classid(TfrmRetrieveMain), &frmRetrieveMain);
         Application->CreateForm(__classid(TfrmMove), &frmMove);
         Application->CreateForm(__classid(TfrmAnalyseSpecimens), &frmAnalyseSpecimens);
         Application->CreateForm(__classid(TfrmStoremain), &frmStoremain);
         Application->CreateForm(__classid(TfrmConfirm), &frmConfirm);
         Application->CreateForm(__classid(TfrmAboutBox), &frmAboutBox);
         Application->CreateForm(__classid(TfrmPassword), &frmPassword);
         Application->CreateForm(__classid(TfrmAliquotTypes), &frmAliquotTypes);
         Application->CreateForm(__classid(TfrmRetrieved), &frmRetrieved);
         Application->CreateForm(__classid(TfrmAddSpecimens), &frmAddSpecimens);
         Application->CreateForm(__classid(TfrmNewSite), &frmNewSite);
         Application->CreateForm(__classid(TfrmFind), &frmFind);
         Application->CreateForm(__classid(TfrmReferred), &frmReferred);
         Application->CreateForm(__classid(TfrmSelectBoxes), &frmSelectBoxes);
         Application->CreateForm(__classid(TfrmReferredBoxesSummary), &frmReferredBoxesSummary);
         Application->CreateForm(__classid(TfrmTest), &frmTest);
         Application->CreateForm(__classid(TfrmBoxes), &frmBoxes);
         Application->CreateForm(__classid(TfrmRetrievalAssistant), &frmRetrievalAssistant);
         Application->CreateForm(__classid(TfrmProcess), &frmProcess);
         Application->CreateForm(__classid(TfrmSamples), &frmSamples);
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
			throw Exception("Unknown exception");
		}
		catch (Exception &exception)
		{
			Application->ShowException(&exception);
		}
	}
	return 0;
}

//---------------------------------------------------------------------------


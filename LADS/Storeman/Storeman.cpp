//---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop
#include <tchar.h>

//---------------------------------------------------------------------------





USEFORM("FindSample.cpp", frmFind);
USEFORM("DiscardSamples.cpp", frmDiscardSamples);
USEFORM("DiscardSearch.cpp", frmSearch);
USEFORM("NewSection.cpp", frmNewSection);
USEFORM("NewSite.cpp", frmNewSite);
USEFORM("NewJob.cpp", frmNewJob);
USEFORM("MoveJobs.cpp", frmSelectJob);
USEFORM("NewExercise.cpp", frmNewExercise);
USEFORM("DiscardReason.cpp", frmDiscardReason);
USEFORM("BrowseSample.cpp", frmBrowse);
USEFORM("Configure.cpp", frmConfigure);
USEFORM("BoxTransfer.cpp", frmBoxList);
USEFORM("AnalyseSamples.cpp", frmAnalyseSpecimens);
USEFORM("DiscardNote.cpp", frmDiscardNote);
USEFORM("DiscardPrologue.cpp", frmDiscardPrologue);
USEFORM("DiscardMethod.cpp", frmDiscardMethod);
USEFORM("NewTank.cpp", frmNewTank);
USEFORM("Storemain.cpp", frmStoremain);
USEFORM("ShowSamples.cpp", frmAliquotTypes);
USEFORM("SMLogin.cpp", frmSMLogin);
USEFORM("SelectSamples.cpp", frmSelectBoxes);
USEFORM("RetrievalAssistantAutoChunk.cpp", frmAutoChunk);
USEFORM("RetrievalAssistantBoxes.cpp", frmBoxes);
USEFORM("RetrievalAssistant.cpp", frmRetrievalAssistant);
USEFORM("ReferredBoxes.cpp", frmReferred);
USEFORM("ReferredBoxesSummary.cpp", frmReferredBoxesSummary);
USEFORM("RetrievalAssistantProcess.cpp", frmProcess);
USEFORM("SampleMove.cpp", frmMove);
USEFORM("SampleEntry.cpp", frmRetrieveMain);
USEFORM("RetrievalAssistantSamples.cpp", frmSamples);
USEFORM("RetrieveSamples.cpp", frmRetrieved);
USEFORM("AddSpecimens.cpp", frmAddSpecimens);
USEFORM("..\LabsForms\TfrmLoginBase.cpp", frmLoginBase);
USEFORM("..\LabsForms\TfrmPassword.cpp", frmPassword);
USEFORM("..\LabsForms\TfrmConfirm.cpp", frmConfirm);
USEFORM("..\LabsForms\dvSelector.cpp", selectorFrame); /* TFrame: File Type */
USEFORM("..\LabsForms\TfrmAboutBox.cpp", frmAboutBox);
USEFORM("NewType.cpp", frmNewType);
//---------------------------------------------------------------------------
WINAPI _tWinMain(HINSTANCE, HINSTANCE, LPTSTR, int)
{
	try
	{	Application->Initialize();
		Application->MainFormOnTaskBar = true;
		Application->Title = "Storage Management";
		Application->CreateForm(__classid(TfrmSMLogin), &frmSMLogin);
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
		Application->CreateForm(__classid(TfrmNewExercise), &frmNewExercise);
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
		Application->CreateForm(__classid(TfrmSelectBoxes), &frmSelectBoxes);
		Application->CreateForm(__classid(TfrmReferredBoxesSummary), &frmReferredBoxesSummary);
		Application->CreateForm(__classid(TfrmRetrievalAssistant), &frmRetrievalAssistant);
		Application->CreateForm(__classid(TfrmAutoChunk), &frmAutoChunk);
		Application->CreateForm(__classid(TfrmBoxes), &frmBoxes);
		Application->CreateForm(__classid(TfrmProcess), &frmProcess);
		Application->CreateForm(__classid(TfrmSamples), &frmSamples);
		Application->CreateForm(__classid(TfrmReferred), &frmReferred);
		Application->CreateForm(__classid(TfrmNewType), &frmNewType);
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


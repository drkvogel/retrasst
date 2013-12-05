//---------------------------------------------------------------------------

#include <vcl.h>
#include <tchar.h>
#pragma hdrstop

//---------------------------------------------------------------------------




USEFORM("FindSample.cpp", frmFind);
USEFORM("DiscardReason.cpp", frmDiscardReason);
USEFORM("DiscardSamples.cpp", frmDiscardSamples);
USEFORM("DiscardSearch.cpp", frmSearch);
USEFORM("NewJob.cpp", frmNewJob);
USEFORM("NewSection.cpp", frmNewSection);
USEFORM("NewSite.cpp", frmNewSite);
USEFORM("MoveJobs.cpp", frmSelectJob);
USEFORM("NewBoxType.cpp", frmNewBoxType);
USEFORM("NewExercise.cpp", frmNewExercise);
USEFORM("DiscardPrologue.cpp", frmDiscardPrologue);
USEFORM("AnalyseSamples.cpp", frmAnalyseSpecimens);
USEFORM("BoxTransfer.cpp", frmBoxList);
USEFORM("AddSpecimens.cpp", frmAddSpecimens);
USEFORM("BrowseSample.cpp", frmBrowse);
USEFORM("DiscardMethod.cpp", frmDiscardMethod);
USEFORM("DiscardNote.cpp", frmDiscardNote);
USEFORM("Configure.cpp", frmConfigure);
USEFORM("Storemain.cpp", frmStoremain);
USEFORM("SelectSamples.cpp", frmSelectBoxes);
USEFORM("showsamples.cpp", frmAliquotTypes);
USEFORM("SMLogin.cpp", frmSMLogin);
USEFORM("SampleMove.cpp", frmMove);
USEFORM("ReferredBoxesSummary.cpp", frmReferredBoxesSummary);
USEFORM("RetrievalAssistant.cpp", frmRetrievalAssistant);
USEFORM("NewTank.cpp", frmNewTank);
USEFORM("NewType.cpp", frmNewStoreType);
USEFORM("ReferredBoxes.cpp", frmReferred);
USEFORM("RetrievalAssistantBoxes.cpp", frmBoxes);
USEFORM("RetrieveSamples.cpp", frmRetrieved);
USEFORM("SampleEntry.cpp", frmRetrieveMain);
USEFORM("RetrievalAssistantProcess.cpp", frmProcess);
USEFORM("RetrievalAssistantProcessBoxes.cpp", frmProcessBoxes);
USEFORM("RetrievalAssistantSamples.cpp", frmSamples);
USEFORM("..\..\jonathanpr\RetrievalLists\TcryovialHistoryDialog.cpp", cryovialHistoryDialog);
USEFORM("..\..\jonathanpr\RetrievalLists\TResultNotesDialog.cpp", ResultNotesDialog);
USEFORM("..\..\jonathanpr\RetrievalLists\mainDialog.cpp", frmViewListMain);
USEFORM("..\..\jonathanpr\RetrievalLists\mainListDialog.cpp", listDialog);
USEFORM("..\..\jonathanpr\RetrievalLists\TResultsDialog.cpp", ResultsDialog);
USEFORM("..\LabsForms\dvSelector.cpp", selectorFrame); /* TFrame: File Type */
USEFORM("..\LabsForms\TfrmAboutBox.cpp", frmAboutBox);
USEFORM("..\LabsForms\TfrmConfirm.cpp", frmConfirm);
USEFORM("..\LabsForms\TfrmLoginBase.cpp", frmLoginBase);
USEFORM("..\LabsForms\TfrmPassword.cpp", frmPassword);
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
         Application->CreateForm(__classid(TfrmBoxes), &frmBoxes);
         Application->CreateForm(__classid(TfrmProcess), &frmProcess);
         Application->CreateForm(__classid(TfrmSamples), &frmSamples);
         Application->CreateForm(__classid(TfrmReferred), &frmReferred);
         Application->CreateForm(__classid(TfrmNewStoreType), &frmNewStoreType);
         Application->CreateForm(__classid(TfrmNewBoxType), &frmNewBoxType);
         Application->CreateForm(__classid(TfrmProcessBoxes), &frmProcessBoxes);
         Application->CreateForm(__classid(TfrmProcessBoxes), &frmProcessBoxes);
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



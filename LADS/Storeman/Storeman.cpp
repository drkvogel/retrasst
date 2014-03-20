//---------------------------------------------------------------------------

#include <vcl.h>
#include <tchar.h>
#pragma hdrstop

//---------------------------------------------------------------------------

USEFORM("RationaliseTanks\RatTanksCreateRetrievalJob.cpp", frmRatTankCreateRetrievalJob);
USEFORM("RationaliseTanks\RatTanksdbSelection.cpp", frmRatTankDbSelectionDialog);
USEFORM("RationaliseTanks\RatTanksBoxSelection.cpp", frmRatTanksBoxSelection);
USEFORM("NewType.cpp", frmNewStoreType);
USEFORM("RationaliseTanks\RatTanksCreateExercise.cpp", frmRatTanksRetrivalExercise);
USEFORM("RationaliseTanks\RatTanksBoxType.cpp", frmRatTanksBoxType);
USEFORM("ReferredBoxesSummary.cpp", frmReferredBoxesSummary);
USEFORM("ReferredBoxes.cpp", frmReferred);
USEFORM("RetrAsstCollectSamples.cpp", frmProcess);
USEFORM("RetrAsstCollectBoxes.cpp", frmProcessBoxes);
USEFORM("RationaliseTanks\RatTanksMainDialog.cpp", frmRatTanksMainDialog);
USEFORM("RationaliseTanks\RatTanksSignIn.cpp", frmRatTankSignIn);
USEFORM("RationaliseTanks\RatTanksPleaseWait.cpp", frmRatTankPleaseWait);
USEFORM("DiscardSearch.cpp", frmSearch);
USEFORM("FindSample.cpp", frmFind);
USEFORM("DiscardPrologue.cpp", frmDiscardPrologue);
USEFORM("DiscardNote.cpp", frmDiscardNote);
USEFORM("DiscardSamples.cpp", frmDiscardSamples);
USEFORM("DiscardReason.cpp", frmDiscardReason);
USEFORM("NewSection.cpp", frmNewSection);
USEFORM("NewJob.cpp", frmNewJob);
USEFORM("NewTank.cpp", frmNewTank);
USEFORM("NewSite.cpp", frmNewSite);
USEFORM("MoveJobs.cpp", frmSelectJob);
USEFORM("NewExercise.cpp", frmNewExercise);
USEFORM("NewBoxType.cpp", frmNewBoxType);
USEFORM("RetrAsstMain.cpp", frmRetrievalAssistant);
USEFORM("SMLogin.cpp", frmSMLogin);
USEFORM("Storemain.cpp", frmStoremain);
USEFORM("RetrievalLists\RetrievalListMainListDialog.cpp", frmRetrievalListDialog);
USEFORM("RetrievalLists\retrievalListMainDialog.cpp", frmRetrievalListViewListMain);
USEFORM("RetrAsstPlanSamples.cpp", frmSamples);
USEFORM("RetrAsstPlanBoxes.cpp", frmBoxes);
USEFORM("RetrievalLists\RetrievalListCryovialHistoryDialog.cpp", frmRetrievalListCryovialHistoryDialog);
USEFORM("SampleMove.cpp", frmMove);
USEFORM("showsamples.cpp", frmAliquotTypes);
USEFORM("SelectSamples.cpp", frmSelectBoxes);
USEFORM("RetrievalLists\RetrievalListResultsDialog.cpp", frmRetrievalListResultsDialog);
USEFORM("RetrievalLists\RetrievalListResultNotesDialog.cpp", frmRetrievalListResultNotesDialog);
USEFORM("SampleEntry.cpp", frmRetrieveMain);
USEFORM("RetrieveSamples.cpp", frmRetrieved);
USEFORM("AddSpecimens.cpp", frmAddSpecimens);
USEFORM("AnalyseSamples.cpp", frmAnalyseSpecimens);
USEFORM("DiscardMethod.cpp", frmDiscardMethod);
USEFORM("BoxTransfer.cpp", frmBoxList);
USEFORM("Configure.cpp", frmConfigure);
USEFORM("BrowseSample.cpp", frmBrowse);
USEFORM("..\LabsForms\TfrmLoginBase.cpp", frmLoginBase);
USEFORM("..\LabsForms\TfrmPassword.cpp", frmPassword);
USEFORM("..\LabsForms\dvSelector.cpp", selectorFrame); /* TFrame: File Type */
USEFORM("..\LabsForms\TfrmConfirm.cpp", frmConfirm);
USEFORM("..\LabsForms\TfrmAboutBox.cpp", frmAboutBox);
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
		Application->CreateForm(__classid(TfrmReferred), &frmReferred);
		Application->CreateForm(__classid(TfrmNewStoreType), &frmNewStoreType);
		Application->CreateForm(__classid(TfrmNewBoxType), &frmNewBoxType);
		Application->CreateForm(__classid(TfrmProcessBoxes), &frmProcessBoxes);
		Application->CreateForm(__classid(TfrmProcessBoxes), &frmProcessBoxes);
		Application->CreateForm(__classid(TfrmRatTanksBoxType), &frmRatTanksBoxType);
		Application->CreateForm(__classid(TfrmRatTanksRetrivalExercise), &frmRatTanksRetrivalExercise);
		Application->CreateForm(__classid(TfrmRatTankCreateRetrievalJob), &frmRatTankCreateRetrievalJob);
		Application->CreateForm(__classid(TfrmRatTankDbSelectionDialog), &frmRatTankDbSelectionDialog);
		Application->CreateForm(__classid(TfrmRatTanksMainDialog), &frmRatTanksMainDialog);
		Application->CreateForm(__classid(TfrmRatTankPleaseWait), &frmRatTankPleaseWait);
		Application->CreateForm(__classid(TfrmRatTankSignIn), &frmRatTankSignIn);
		Application->CreateForm(__classid(TfrmRetrievalListCryovialHistoryDialog), &frmRetrievalListCryovialHistoryDialog);
		Application->CreateForm(__classid(TfrmRetrievalListViewListMain), &frmRetrievalListViewListMain);
		Application->CreateForm(__classid(TfrmRetrievalListResultNotesDialog), &frmRetrievalListResultNotesDialog);
		Application->CreateForm(__classid(TfrmProcessBoxes), &frmProcessBoxes);
		Application->CreateForm(__classid(TfrmProcess), &frmProcess);
		Application->CreateForm(__classid(TfrmRetrievalAssistant), &frmRetrievalAssistant);
		Application->CreateForm(__classid(TfrmBoxes), &frmBoxes);
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



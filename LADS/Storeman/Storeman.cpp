//---------------------------------------------------------------------------

#include <vcl.h>
#include <tchar.h>
#pragma hdrstop

//---------------------------------------------------------------------------

USEFORM("RationaliseTanks\RatTanksMainDialog.cpp", frmRatTanksMainDialog);
USEFORM("RationaliseTanks\RatTanksdbSelection.cpp", frmRatTankDbSelectionDialog);
USEFORM("RationaliseTanks\RatTanksBoxType.cpp", frmRatTanksBoxType);
USEFORM("RationaliseTanks\RatTanksBoxSelection.cpp", frmRatTanksBoxSelection);
USEFORM("RationaliseTanks\RatTanksCreateRetrievalJob.cpp", frmRatTankCreateRetrievalJob);
USEFORM("RationaliseTanks\RatTanksCreateExercise.cpp", frmRatTanksRetrivalExercise);
USEFORM("RetrAsstCollectBoxes.cpp", frmRetrAsstCollectBoxes);
USEFORM("ReferredBoxesSummary.cpp", frmReferredBoxesSummary);
USEFORM("RetrAsstCollectSamples.cpp", frmRetrAsstCollectVials);
USEFORM("RetrAsstCollectEmpties.cpp", frmCollectEmpties);
USEFORM("RationaliseTanks\RatTanksPleaseWait.cpp", frmRatTankPleaseWait);
USEFORM("ReferredBoxes.cpp", frmReferred);
USEFORM("RationaliseTanks\RatTanksSignIn.cpp", frmRatTankSignIn);
USEFORM("FindSample.cpp", frmFind);
USEFORM("DiscardReason.cpp", frmDiscardReason);
USEFORM("DiscardPrologue.cpp", frmDiscardPrologue);
USEFORM("DiscardSearch.cpp", frmSearch);
USEFORM("DiscardSamples.cpp", frmDiscardSamples);
USEFORM("NewSite.cpp", frmNewSite);
USEFORM("NewSection.cpp", frmNewSection);
USEFORM("NewType.cpp", frmNewStoreType);
USEFORM("NewTank.cpp", frmNewTank);
USEFORM("NewBoxType.cpp", frmNewBoxType);
USEFORM("MoveJobs.cpp", frmSelectJob);
USEFORM("NewJob.cpp", frmNewJob);
USEFORM("NewExercise.cpp", frmNewExercise);
USEFORM("SMLogin.cpp", frmSMLogin);
USEFORM("showsamples.cpp", frmAliquotTypes);
USEFORM("Storemain.cpp", frmStoremain);
USEFORM("RetrievalLists\retrievalListMainDialog.cpp", frmRetrievalListViewListMain);
USEFORM("RetrAsstPlanBoxes.cpp", frmBoxes);
USEFORM("RetrAsstMain.cpp", frmRetrievalAssistant);
USEFORM("RetrievalLists\RetrievalListCryovialHistoryDialog.cpp", frmRetrievalListCryovialHistoryDialog);
USEFORM("RetrAsstPlanSamples.cpp", frmRetrAsstPlanVials);
USEFORM("SampleEntry.cpp", frmRetrieveMain);
USEFORM("SelectSamples.cpp", frmSelectBoxes);
USEFORM("SampleMove.cpp", frmMove);
USEFORM("RetrievalLists\RetrievalListResultNotesDialog.cpp", frmRetrievalListResultNotesDialog);
USEFORM("RetrievalLists\RetrievalListMainListDialog.cpp", frmRetrievalListDialog);
USEFORM("RetrieveSamples.cpp", frmRetrieved);
USEFORM("RetrievalLists\RetrievalListResultsDialog.cpp", frmRetrievalListResultsDialog);
USEFORM("AddSpecimens.cpp", frmAddSpecimens);
USEFORM("AnalyseSamples.cpp", frmAnalyseSpecimens);
USEFORM("DiscardNote.cpp", frmDiscardNote);
USEFORM("DiscardMethod.cpp", frmDiscardMethod);
USEFORM("BrowseSample.cpp", frmBrowse);
USEFORM("BoxTransfer.cpp", frmBoxList);
USEFORM("Configure.cpp", frmConfigure);
USEFORM("..\LabsForms\TfrmConfirm.cpp", frmConfirm);
USEFORM("..\LabsForms\TfrmAboutBox.cpp", frmAboutBox);
USEFORM("..\LabsForms\TfrmPassword.cpp", frmPassword);
USEFORM("..\LabsForms\TfrmLoginBase.cpp", frmLoginBase);
USEFORM("..\LabsForms\dvSelector.cpp", selectorFrame); /* TFrame: File Type */
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
         Application->CreateForm(__classid(TfrmRetrAsstCollectBoxes), &frmRetrAsstCollectBoxes);
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
         Application->CreateForm(__classid(TfrmRetrAsstCollectVials), &frmRetrAsstCollectVials);
         Application->CreateForm(__classid(TfrmRetrievalAssistant), &frmRetrievalAssistant);
         Application->CreateForm(__classid(TfrmBoxes), &frmBoxes);
         Application->CreateForm(__classid(TfrmRetrAsstPlanVials), &frmRetrAsstPlanVials);
         Application->CreateForm(__classid(TfrmCollectEmpties), &frmCollectEmpties);
         Application->CreateForm(__classid(TfrmCollectEmpties), &frmCollectEmpties);
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



//---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop
#include <tchar.h>
//---------------------------------------------------------------------------
USEFORM("RationaliseTanks\RatTanksdbSelection.cpp", frmRatTankDbSelectionDialog);
USEFORM("RationaliseTanks\RatTanksMainDialog.cpp", frmRatTanksMainDialog);
USEFORM("RationaliseTanks\RatTanksCreateExercise.cpp", frmRatTanksRetrivalExercise);
USEFORM("RationaliseTanks\RatTanksBoxType.cpp", frmRatTanksBoxType);
USEFORM("RationaliseTanks\RatTanksCreateRetrievalJob.cpp", frmRatTankCreateRetrievalJob);
USEFORM("RetrAsstCollectEmpties.cpp", frmCollectEmpties);
USEFORM("RetrAsstCollectBoxes.cpp", frmRetrAsstCollectBoxes);
USEFORM("RetrAsstMain.cpp", frmRetrievalAssistant);
USEFORM("RetrAsstCollectSamples.cpp", frmRetrAsstCollectSamples);
USEFORM("RationaliseTanks\RatTanksSignIn.cpp", frmRatTankSignIn);
USEFORM("RationaliseTanks\RatTanksPleaseWait.cpp", frmRatTankPleaseWait);
USEFORM("ReferredBoxesSummary.cpp", frmReferredBoxesSummary);
USEFORM("ReferredBoxes.cpp", frmReferred);
USEFORM("RationaliseTanks\RatTanksBoxSelection.cpp", frmRatTanksBoxSelection);
USEFORM("ManageLists.cpp", frmManageLists);
USEFORM("MoveJobs.cpp", frmSelectJob);
USEFORM("ManageListsShowList.cpp", frmManListsShow);
USEFORM("DiscardSearch.cpp", frmSearch);
USEFORM("FindSample.cpp", frmFind);
USEFORM("NewSite.cpp", frmNewSite);
USEFORM("NewSection.cpp", frmNewSection);
USEFORM("NewType.cpp", frmNewStoreType);
USEFORM("NewTank.cpp", frmNewTank);
USEFORM("NewExercise.cpp", frmNewExercise);
USEFORM("NewBoxType.cpp", frmNewBoxType);
USEFORM("NewReason.cpp", frmNewReason);
USEFORM("NewJob.cpp", frmNewJob);
USEFORM("RetrAsstPlanBoxes.cpp", frmRetrAsstPlanBoxes);
USEFORM("SMLogin.cpp", frmSMLogin);
USEFORM("showsamples.cpp", frmAliquotTypes);
USEFORM("Storemain.cpp", frmStoremain);
USEFORM("SelectSamples.cpp", frmSelectBoxes);
USEFORM("RetrievalLists\retrievalListMainDialog.cpp", frmRetrievalListViewListMain);
USEFORM("RetrAsstWrongBarcode.cpp", frmWrongBarcode);
USEFORM("RetrAsstPlanSamples.cpp", frmRetrAsstPlanSamples);
USEFORM("RetrievalLists\RetrievalListCryovialHistoryDialog.cpp", frmRetrievalListCryovialHistoryDialog);
USEFORM("RetrievalListBase.cpp", frmRetrievalJobList);
USEFORM("SampleEntry.cpp", frmRetrieveMain);
USEFORM("RetrieveSamples.cpp", frmRetrieved);
USEFORM("SampleMove.cpp", frmMove);
USEFORM("RetrievalLists\RetrievalListResultNotesDialog.cpp", frmRetrievalListResultNotesDialog);
USEFORM("RetrievalLists\RetrievalListMainListDialog.cpp", frmRetrievalListDialog);
USEFORM("RetrievalListTreeView.cpp", frmRetrievalTreeView);
USEFORM("RetrievalLists\RetrievalListResultsDialog.cpp", frmRetrievalListResultsDialog);
USEFORM("DiscardSamples.cpp", frmDiscardSamples);
USEFORM("AnalyseSamples.cpp", frmAnalyseSpecimens);
USEFORM("AddSpecimens.cpp", frmAddSpecimens);
USEFORM("BoxTransfer.cpp", frmBoxList);
USEFORM("DiscardMethod.cpp", frmDiscardMethod);
USEFORM("DiscardPrologue.cpp", frmDiscardPrologue);
USEFORM("DiscardNote.cpp", frmDiscardNote);
USEFORM("Configure.cpp", frmConfigure);
USEFORM("BrowseSample.cpp", frmBrowse);
USEFORM("..\LabsForms\TfrmPassword.cpp", frmPassword);
USEFORM("..\LabsForms\TfrmLoginBase.cpp", frmLoginBase);
USEFORM("..\LabsForms\TfrmSelectDbS.cpp", frmSelectDbSystem);
USEFORM("..\LabsForms\dvSelector.cpp", selectorFrame); /* TFrame: File Type */
USEFORM("..\LabsForms\TfrmConfirm.cpp", frmConfirm);
USEFORM("..\LabsForms\TfrmAboutBox.cpp", frmAboutBox);
USEFORM("RetrAsstAddNote.cpp", frmRetrAsstAddNote);
//---------------------------------------------------------------------------
int WINAPI _tWinMain(HINSTANCE, HINSTANCE, LPTSTR, int)
{
	try
	{
		Application->Initialize();
		Application->MainFormOnTaskBar = true;
		Application->CreateForm(__classid(TfrmSMLogin), &frmSMLogin);
         Application->CreateForm(__classid(TfrmAboutBox), &frmAboutBox);
         Application->CreateForm(__classid(TfrmConfirm), &frmConfirm);
         Application->CreateForm(__classid(TfrmAddSpecimens), &frmAddSpecimens);
         Application->CreateForm(__classid(TfrmAnalyseSpecimens), &frmAnalyseSpecimens);
         Application->CreateForm(__classid(TfrmBoxList), &frmBoxList);
         Application->CreateForm(__classid(TfrmBrowse), &frmBrowse);
         Application->CreateForm(__classid(TfrmConfigure), &frmConfigure);
         Application->CreateForm(__classid(TfrmDiscardMethod), &frmDiscardMethod);
         Application->CreateForm(__classid(TfrmDiscardNote), &frmDiscardNote);
         Application->CreateForm(__classid(TfrmDiscardPrologue), &frmDiscardPrologue);
         Application->CreateForm(__classid(TfrmDiscardSamples), &frmDiscardSamples);
         Application->CreateForm(__classid(TfrmSearch), &frmSearch);
         Application->CreateForm(__classid(TfrmFind), &frmFind);
         Application->CreateForm(__classid(TfrmSelectJob), &frmSelectJob);
         Application->CreateForm(__classid(TfrmNewBoxType), &frmNewBoxType);
         Application->CreateForm(__classid(TfrmNewExercise), &frmNewExercise);
         Application->CreateForm(__classid(TfrmNewJob), &frmNewJob);
         Application->CreateForm(__classid(TfrmNewSection), &frmNewSection);
         Application->CreateForm(__classid(TfrmNewSite), &frmNewSite);
         Application->CreateForm(__classid(TfrmNewTank), &frmNewTank);
         Application->CreateForm(__classid(TfrmNewStoreType), &frmNewStoreType);
         Application->CreateForm(__classid(TfrmNewReason), &frmNewReason);
         Application->CreateForm(__classid(TfrmReferredBoxesSummary), &frmReferredBoxesSummary);
         Application->CreateForm(__classid(TfrmRetrAsstCollectBoxes), &frmRetrAsstCollectBoxes);
         Application->CreateForm(__classid(TfrmCollectEmpties), &frmCollectEmpties);
         Application->CreateForm(__classid(TfrmRetrAsstCollectSamples), &frmRetrAsstCollectSamples);
         Application->CreateForm(__classid(TfrmRetrAsstPlanBoxes), &frmRetrAsstPlanBoxes);
         Application->CreateForm(__classid(TfrmRetrAsstPlanSamples), &frmRetrAsstPlanSamples);
         Application->CreateForm(__classid(TfrmRetrieved), &frmRetrieved);
         Application->CreateForm(__classid(TfrmRetrieveMain), &frmRetrieveMain);
         Application->CreateForm(__classid(TfrmMove), &frmMove);
         Application->CreateForm(__classid(TfrmSelectBoxes), &frmSelectBoxes);
         Application->CreateForm(__classid(TfrmAliquotTypes), &frmAliquotTypes);
         Application->CreateForm(__classid(TfrmStoremain), &frmStoremain);
         Application->CreateForm(__classid(TfrmSelectDbSystem), &frmSelectDbSystem);
         Application->CreateForm(__classid(TfrmPassword), &frmPassword);
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
         Application->CreateForm(__classid(TfrmReferred), &frmReferred);
         Application->CreateForm(__classid(TfrmNewReason), &frmNewReason);
         Application->CreateForm(__classid(TfrmRetrievalAssistant), &frmRetrievalAssistant);
         Application->CreateForm(__classid(TfrmManageLists), &frmManageLists);
         Application->CreateForm(__classid(TfrmManListsShow), &frmManListsShow);
         Application->CreateForm(__classid(TfrmRetrievalTreeView), &frmRetrievalTreeView);
         Application->CreateForm(__classid(TfrmWrongBarcode), &frmWrongBarcode);
         Application->CreateForm(__classid(TfrmRetrAsstAddNote), &frmRetrAsstAddNote);
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
			throw Exception("");
		}
		catch (Exception &exception)
		{
			Application->ShowException(&exception);
		}
	}
	return 0;
}
//---------------------------------------------------------------------------

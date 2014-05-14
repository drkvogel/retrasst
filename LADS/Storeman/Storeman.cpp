//---------------------------------------------------------------------------

#include <vcl.h>
#include <tchar.h>
#include <exception>
#include <string>
#pragma hdrstop

//---------------------------------------------------------------------------

USEFORM("RationaliseTanks\RatTanksdbSelection.cpp", frmRatTankDbSelectionDialog);
USEFORM("RationaliseTanks\RatTanksCreateRetrievalJob.cpp", frmRatTankCreateRetrievalJob);
USEFORM("RationaliseTanks\RatTanksBoxSelection.cpp", frmRatTanksBoxSelection);
USEFORM("RationaliseTanks\RatTanksBoxType.cpp", frmRatTanksBoxType);
USEFORM("RationaliseTanks\RatTanksCreateExercise.cpp", frmRatTanksRetrivalExercise);
USEFORM("RationaliseTanks\RatTanksMainDialog.cpp", frmRatTanksMainDialog);
USEFORM("ReferredBoxesSummary.cpp", frmReferredBoxesSummary);
USEFORM("RetrAsstCollectBoxes.cpp", frmRetrAsstCollectBoxes);
USEFORM("RetrAsstCollectSamples.cpp", frmRetrAsstCollectSamples);
USEFORM("ReferredBoxes.cpp", frmReferred);
USEFORM("RationaliseTanks\RatTanksPleaseWait.cpp", frmRatTankPleaseWait);
USEFORM("RationaliseTanks\RatTanksSignIn.cpp", frmRatTankSignIn);
USEFORM("NewType.cpp", frmNewStoreType);
USEFORM("FindSample.cpp", frmFind);
USEFORM("DiscardSearch.cpp", frmSearch);
USEFORM("DiscardPrologue.cpp", frmDiscardPrologue);
USEFORM("DiscardSamples.cpp", frmDiscardSamples);
USEFORM("NewSection.cpp", frmNewSection);
USEFORM("NewSite.cpp", frmNewSite);
USEFORM("NewTank.cpp", frmNewTank);
USEFORM("NewJob.cpp", frmNewJob);
USEFORM("MoveJobs.cpp", frmSelectJob);
USEFORM("NewBoxType.cpp", frmNewBoxType);
USEFORM("NewExercise.cpp", frmNewExercise);
USEFORM("RetrAsstMain.cpp", frmRetrievalAssistant);
USEFORM("SMLogin.cpp", frmSMLogin);
USEFORM("Storemain.cpp", frmStoremain);
USEFORM("showsamples.cpp", frmAliquotTypes);
USEFORM("RetrievalLists\retrievalListMainDialog.cpp", frmRetrievalListViewListMain);
USEFORM("RetrAsstPlanBoxes.cpp", frmRetrAsstPlanBoxes);
USEFORM("RetrAsstPlanSamples.cpp", frmRetrAsstPlanSamples);
USEFORM("RetrievalLists\RetrievalListCryovialHistoryDialog.cpp", frmRetrievalListCryovialHistoryDialog);
USEFORM("RetrievalLists\RetrievalListMainListDialog.cpp", frmRetrievalListDialog);
USEFORM("SampleMove.cpp", frmMove);
USEFORM("SelectSamples.cpp", frmSelectBoxes);
USEFORM("SampleEntry.cpp", frmRetrieveMain);
USEFORM("RetrievalLists\RetrievalListResultNotesDialog.cpp", frmRetrievalListResultNotesDialog);
USEFORM("RetrievalLists\RetrievalListResultsDialog.cpp", frmRetrievalListResultsDialog);
USEFORM("RetrieveSamples.cpp", frmRetrieved);
USEFORM("DiscardNote.cpp", frmDiscardNote);
USEFORM("AddSpecimens.cpp", frmAddSpecimens);
USEFORM("AnalyseSamples.cpp", frmAnalyseSpecimens);
USEFORM("DiscardMethod.cpp", frmDiscardMethod);
USEFORM("BoxTransfer.cpp", frmBoxList);
USEFORM("BrowseSample.cpp", frmBrowse);
USEFORM("Configure.cpp", frmConfigure);
USEFORM("..\LabsForms\TfrmConfirm.cpp", frmConfirm);
USEFORM("..\LabsForms\TfrmLoginBase.cpp", frmLoginBase);
USEFORM("..\LabsForms\TfrmPassword.cpp", frmPassword);
USEFORM("..\LabsForms\TfrmAboutBox.cpp", frmAboutBox);
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
         Application->CreateForm(__classid(TfrmRatTanksBoxType), &frmRatTanksBoxType);
         Application->CreateForm(__classid(TfrmRatTanksRetrivalExercise), &frmRatTanksRetrivalExercise);
         Application->CreateForm(__classid(TfrmRatTankCreateRetrievalJob), &frmRatTankCreateRetrievalJob);
         Application->CreateForm(__classid(TfrmRatTankDbSelectionDialog), &frmRatTankDbSelectionDialog);
         Application->CreateForm(__classid(TfrmRatTanksMainDialog), &frmRatTanksMainDialog);
         Application->CreateForm(__classid(TfrmRatTankPleaseWait), &frmRatTankPleaseWait);
         Application->CreateForm(__classid(TfrmRetrievalListCryovialHistoryDialog), &frmRetrievalListCryovialHistoryDialog);
         Application->CreateForm(__classid(TfrmRetrievalListViewListMain), &frmRetrievalListViewListMain);
         Application->CreateForm(__classid(TfrmRetrievalListResultNotesDialog), &frmRetrievalListResultNotesDialog);
         Application->CreateForm(__classid(TfrmRetrievalAssistant), &frmRetrievalAssistant);
         Application->CreateForm(__classid(TfrmRetrAsstPlanSamples), &frmRetrAsstPlanSamples);
         Application->CreateForm(__classid(TfrmRetrAsstCollectSamples), &frmRetrAsstCollectSamples);
         Application->CreateForm(__classid(TfrmRetrAsstPlanBoxes), &frmRetrAsstPlanBoxes);
         Application->CreateForm(__classid(TfrmRetrAsstCollectBoxes), &frmRetrAsstCollectBoxes);
         Application->CreateForm(__classid(TfrmRatTankSignIn), &frmRatTankSignIn);
		Application->Run();
	}
	catch (Exception &exception) {
		Application->ShowException(&exception);
	}
	catch (const std::exception & e) {
		try	{
			throw Exception(e.what());
		}
		catch (Exception &exception){
			Application->ShowException(&exception);
		}
	}
	catch (const std::string & what) {
		try	{
			throw Exception(what.c_str());
		}
		catch (Exception &exception){
			Application->ShowException(&exception);
		}
	}
	catch (const char * what) {
		try	{
			throw Exception(what);
		}
		catch (Exception &exception){
			Application->ShowException(&exception);
		}
	}
	catch (...) {
		try	{
			throw Exception("Unknown exception");
		}
		catch (Exception &exception) {
			Application->ShowException(&exception);
		}
	}
	return 0;
}

//---------------------------------------------------------------------------



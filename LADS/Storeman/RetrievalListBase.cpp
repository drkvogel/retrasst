#include <vcl.h>
#include "RetrievalListBase.h"
#include "LCDbObject.h"
#include "LCDbProject.h"
#include "RetrAsstPlanSamples.h"
#include "RetrAsstPlanBoxes.h"
#include "RetrAsstCollectSamples.h"
#include "RetrAsstCollectBoxes.h"
#include "StoreUtil.h"
#include "StoreDAO.h"
#include "LCDbTankMap.h"
#pragma hdrstop

#pragma package(smart_init)
#pragma resource "*.dfm"

TfrmRetrievalJobList *frmRetrievalJobList;

__fastcall TfrmRetrievalJobList::TfrmRetrievalJobList(TComponent* Owner) : TForm(Owner) { }

// moved from header file: linker error under 64-bit XE4
void TfrmRetrievalJobList::msgbox(string main, string title) {
	Application->MessageBoxW(String(main.c_str()).c_str(), String(title.c_str()).c_str(), MB_OK);
}

void __fastcall TfrmRetrievalJobList::FormResize(TObject *Sender) { sgwJobs->resize(); }

void __fastcall TfrmRetrievalJobList::cbLogClick(TObject *Sender) { panelDebug->Visible = cbLog->Checked; }

void __fastcall TfrmRetrievalJobList::FormKeyUp(TObject *Sender, WORD &Key, TShiftState Shift) {
/** form's KeyPreview property needs to be set to see this */
    if (Key == ' ') toggleLog();
}

void TfrmRetrievalJobList::toggleLog() {
    cbLog->Checked          = !(cbLog->Checked);
    panelDebug->Visible     = cbLog->Checked;
    splitterDebug->Visible  = cbLog->Checked;
}

void __fastcall TfrmRetrievalJobList::btnExitClick(TObject *Sender) { Close(); }

void __fastcall TfrmRetrievalJobList::cbNewJobClick(TObject *Sender) { loadJobs(); }

void __fastcall TfrmRetrievalJobList::cbInProgressClick(TObject *Sender) { loadJobs(); }

void __fastcall TfrmRetrievalJobList::cbDoneClick(TObject *Sender) { loadJobs(); }

void __fastcall TfrmRetrievalJobList::cbDeletedClick(TObject *Sender) { loadJobs(); }

void __fastcall TfrmRetrievalJobList::cbRejectedClick(TObject *Sender) { loadJobs(); }

void __fastcall TfrmRetrievalJobList::cbBoxRetrievalClick(TObject *Sender) { loadJobs(); }

void __fastcall TfrmRetrievalJobList::cbSampleRetrievalClick(TObject *Sender) { loadJobs(); }

void __fastcall TfrmRetrievalJobList::cbBoxMoveClick(TObject *Sender) { loadJobs(); }

void __fastcall TfrmRetrievalJobList::cbBoxDiscardClick(TObject *Sender) { loadJobs(); }

void __fastcall TfrmRetrievalJobList::cbSampleDiscardClick(TObject *Sender) { loadJobs(); }

void __fastcall TfrmRetrievalJobList::sgJobsDrawCell(TObject *Sender, int ACol, int ARow, TRect &Rect, TGridDrawState State) {
    LCDbCryoJob * job;
    TColor background = clWindow;
    if (0 == ARow)
        job = NULL;
    else
        job = (LCDbCryoJob *)sgJobs->Objects[0][ARow];
    if (NULL == job) {
        if (0 == ARow) {
            background = clBtnFace; // header row
        } else {
            background = RETRIEVAL_ASSISTANT_ERROR_COLOUR; // error
        }
    } else {
        switch (job->getStatus()) {
        case LCDbCryoJob::Status::NEW_JOB:
            background = RETRIEVAL_ASSISTANT_JOB_NEW_COLOUR; break;
        case LCDbCryoJob::Status::INPROGRESS:
            background = RETRIEVAL_ASSISTANT_JOB_INPROGRESS_COLOUR; break;
        case LCDbCryoJob::Status::DONE:
            background = RETRIEVAL_ASSISTANT_JOB_COMPLETED_COLOUR; break;
        case LCDbCryoJob::Status::DELETED:
            background = RETRIEVAL_ASSISTANT_JOB_DELETED_COLOUR; break;
        default:
            background = RETRIEVAL_ASSISTANT_ERROR_COLOUR;
        }
    }
    TCanvas * cnv = sgJobs->Canvas;
	cnv->Brush->Color = background;
	cnv->FillRect(Rect);
    if (State.Contains(gdSelected)) {
        TFontStyles oldFontStyle = cnv->Font->Style;
        TPenStyle oldPenStyle = cnv->Pen->Style;
        cnv->Pen->Style = psDot;
        cnv->Rectangle(Rect.Left+1, Rect.Top+1, Rect.Right-1, Rect.Bottom-1);
        cnv->Font->Style = TFontStyles() << fsBold; // << fsItalic;
    	cnv->TextOut(Rect.Left+5, Rect.Top+5, sgJobs->Cells[ACol][ARow]);
        cnv->Pen->Style     = oldPenStyle;
        cnv->Font->Style    = oldFontStyle;
	} else {
        cnv->TextOut(Rect.Left+5, Rect.Top+5, sgJobs->Cells[ACol][ARow]);
    }
}

void __fastcall TfrmRetrievalJobList::sgJobsDblClick(TObject *Sender) {
    // to be overridden;
}

void __fastcall TfrmRetrievalJobList::sgJobsClick(TObject *Sender) {
    ostringstream oss;
    LCDbCryoJob * job = ((LCDbCryoJob *)(sgJobs->Objects[0][sgJobs->Row]));
    if (NULL == job) return;
    oss<<job->str();
    debugLog(oss.str().c_str());
}

void __fastcall TfrmRetrievalJobList::FormClose(TObject *Sender, TCloseAction &Action) {
    delete_referenced<tdvecpJob>(vecJobs);
    delete sgwJobs;
}

string TfrmRetrievalJobList::getProjectDescription(int project_cid) {
    if (0 == project_cid) return "Project not specified";
    try {
        return LCDbProjects::records().get(project_cid).getName().c_str();
    } catch (...) {
        ostringstream oss; oss<<"Project ID "<<project_cid<<" not found"; return oss.str();
    }
}

string TfrmRetrievalJobList::getAuditInfo(int process_cid) {
    // c_audit_trail
    //jr->getUserID();
    //LCDbCryoJob::getUserID();
    return "";
}

string TfrmRetrievalJobList::getExerciseDescription(int exercise_cid) { // c_object_name: 20: storage exercise
    ostringstream oss;
    const LCDbObject * exercise = LCDbObjects::records().findByID(exercise_cid);
    oss << exercise->getName().c_str(); return oss.str();
}

void TfrmRetrievalJobList::debugLog(String s) { memoDebug->Lines->Add(s); }

void TfrmRetrievalJobList::init() {
    cbLog->Checked      = false;//RETRASSTDEBUG;
    cbLog->Visible      = true;//= RETRASSTDEBUG;
	panelDebug->Visible = cbLog->Checked;

    sgwJobs = new StringGridWrapper<LCDbCryoJob>(sgJobs, &vecJobs);
    sgwJobs->addCol("boxset",   "Box set",          30);
    sgwJobs->addCol("desc",     "Description",      312);
    sgwJobs->addCol("type",     "Job type",         95);
    sgwJobs->addCol("status",   "Status",           82);
    sgwJobs->addCol("primary",  "Primary Aliquot",  84);
    sgwJobs->addCol("secondary","Secondary Aliquot",94);
    sgwJobs->addCol("project",  "Project",          103);
    sgwJobs->addCol("reason",   "Reason",           155);
    sgwJobs->addCol("start",    "Started",          74);
    sgwJobs->addCol("finish",   "Finished",         74);
    sgwJobs->addCol("claimed",  "Claimed until",    74);
    sgwJobs->init();

    initCustom();
    loadJobs();
}

void TfrmRetrievalJobList::initCustom() { // to override if necessary
    //
}

void TfrmRetrievalJobList::loadJobs() {
    Screen->Cursor = crSQLWait;
    LQuery qc(LIMSDatabase::getCentralDb());
    jobs = LCDbCryoJobs::records();
	jobs.read(LIMSDatabase::getCentralDb(), LCDbCryoJob::UNKNOWN, true); // $2 true: readall
	delete_referenced<tdvecpJob>(vecJobs);
    for (Range< LCDbCryoJob > jr = jobs; jr.isValid(); ++jr) {
        if (!jr->isAvailable()) continue;
        if (jr->getStatus() == LCDbCryoJob::Status::NEW_JOB             && !cbNewJob->Checked) continue;
        if (jr->getStatus() == LCDbCryoJob::Status::INPROGRESS          && !cbInProgress->Checked) continue;
        if (jr->getStatus() == LCDbCryoJob::Status::DONE                && !cbDone->Checked) continue;
        if (jr->getStatus() == LCDbCryoJob::Status::DELETED             && !cbDeleted->Checked) continue;
        if (jr->getStatus() == LCDbCryoJob::Status::REJECTED            && !cbRejected->Checked) continue;
        if (jr->getJobType() == LCDbCryoJob::JobKind::BOX_RETRIEVAL     && !cbBoxRetrieval->Checked) continue;
        if (jr->getJobType() == LCDbCryoJob::JobKind::SAMPLE_RETRIEVAL  && !cbSampleRetrieval->Checked) continue;
        if (jr->getJobType() == LCDbCryoJob::JobKind::BOX_MOVE          && !cbBoxMove->Checked) continue;
        if (jr->getJobType() == LCDbCryoJob::JobKind::BOX_DISCARD       && !cbBoxDiscard->Checked) continue;
        if (jr->getJobType() == LCDbCryoJob::JobKind::SAMPLE_DISCARD    && !cbSampleDiscard->Checked) continue;
        LCDbCryoJob * job = new LCDbCryoJob(); *job = *jr;
        vecJobs.push_back(job);
    }
    Screen->Cursor = crDefault;
    sgJobs->RowCount = vecJobs.size() + 1;
    tdvecpJob::const_iterator it;
    int row = 1;
    for (it = vecJobs.begin(); it != vecJobs.end(); it++, row++) {
        LCDbCryoJob * job = *it;
        sgJobs->Cells[sgwJobs->colNameToInt("boxset" )]  [row] = job->getBoxSet(); // short
        sgJobs->Cells[sgwJobs->colNameToInt("desc" )]    [row] = job->getDescription().c_str();
        sgJobs->Cells[sgwJobs->colNameToInt("type")]     [row] = jobTypeString(job->getJobType()); // UNKNOWN, BOX_MOVE, BOX_RETRIEVAL, BOX_DISCARD, SAMPLE_RETRIEVAL, SAMPLE_DISCARD, NUM_TYPES
        sgJobs->Cells[sgwJobs->colNameToInt("status")]   [row] = jobStatusString(job->getStatus()); // NEW_JOB, INPROGRESS, DONE, DELETED = 99
        sgJobs->Cells[sgwJobs->colNameToInt("primary")]  [row] = Util::getAliquotDescription(job->getPrimaryAliquot()).c_str(); // int
        sgJobs->Cells[sgwJobs->colNameToInt("secondary")][row] = Util::getAliquotDescription(job->getSecondaryAliquot()).c_str(); // int
        sgJobs->Cells[sgwJobs->colNameToInt("project")]  [row] = getProjectDescription(job->getProjectID()).c_str();
        sgJobs->Cells[sgwJobs->colNameToInt("reason")]   [row] = job->getReason().c_str();
        sgJobs->Cells[sgwJobs->colNameToInt("start")]    [row] = job->getStartDate().DateTimeString();
        sgJobs->Cells[sgwJobs->colNameToInt("finish")]   [row] = job->getFinishDate().DateTimeString();
        sgJobs->Cells[sgwJobs->colNameToInt("claimed")]  [row] = job->getClaimedUntil().DateTimeString();
        sgJobs->Objects[0][row] = (TObject *)job;
    }
}

void __fastcall TfrmRetrievalJobList::btnResetJobsClick(TObject *Sender) {
/** for debugging: set all retrieval jobs back to their initial states */
    ostringstream oss; oss<<__FUNC__<<": reset jobs"; debugLog(oss.str().c_str());

    // might be good to be able to delete specific jobs,
    // though would have to iterate through boxes to delete lcr records
    LCDbCryoJob * job = ((LCDbCryoJob *)(sgJobs->Objects[0][sgJobs->Row]));
    if (NULL == job) return;

    LQuery qc(LIMSDatabase::getCentralDb()), qc2(LIMSDatabase::getCentralDb());
    qc.setSQL("select rj_box_cid from c_box_retrieval where retrieval_cid = :rtid");
    qc.setParam("rtid", job->getID());
    qc.open();
    while (!qc.eof()) {
        int rj_box_cid = qc.readInt("rj_box_cid");
        qc2.setSQL("delete from l_cryovial_retrieval where rj_box_cid = :rjbid");
        qc2.setParam("rjbid", rj_box_cid);
        qc2.execSQL();
        qc.next();
    }
    qc.setSQL("delete from c_box_retrieval where retrieval_cid = :rtid");
    qc.setParam("rtid", job->getID());
	qc.execSQL();
	qc.setSQL("update c_retrieval_job set status = :new where retrieval_cid = :rtid");
    qc.setParam("rtid", job->getID());
	qc.setParam("new", LCDbCryoJob::NEW_JOB); // NEW_JOB, INPROGRESS, DONE
    qc.execSQL();
    loadJobs();
}

void TfrmRetrievalJobList::clearStorageCache() {
    storageCache.clear();
}

void TfrmRetrievalJobList::getStorage(SampleRow * sample) {
/** fill in SampleRow structure with storage details of sample */
    ROSETTA result; StoreDAO dao;
    map<int, const SampleRow *>::iterator found = storageCache.find(sample->store_record->getBoxID());
    if (found != storageCache.end() && NULL != (found->second)) { // fill in box location from cache map
        sample->copyLocation(*(found->second));
    } else {
        string tmStatusString = "";
        if (dao.findBox(sample->store_record->getBoxID(), result)) { // status stored in ROSETTA result
            sample->copyLocation(result);
            // but, status could be OFFLINE or ARCHIVED - should display to user
            // that something may be amiss (e.g. in process of being moved)
            switch (result.getIntDefault("status", -1)) { // LDbValid::NEW_RECORD = 0, RECORD_IN_USE = 1, ARCHIVED = 2, DELETED = 99 LCDbTankMap::OFFLINE = 5
                case LCDbTankMap::NEW_RECORD:
                case LCDbTankMap::RECORD_IN_USE:
                    break;  // ok, do nothing
                case LCDbTankMap::ARCHIVED:
                    sample->vessel_name += " (ARCHIVED)"; break;
                case LCDbTankMap::OFFLINE:
                    sample->vessel_name += " (OFFLINE)"; break;
                default:    //case -1:
                    sample->vessel_name += " (UNKNOWN STATUS)";
            }
        } else {
            sample->setLocation("Unconfirmed", 0, "", 0, 0, "", 0);
        }
        storageCache[sample->store_record->getBoxID()] = sample; // cache result
    }
}


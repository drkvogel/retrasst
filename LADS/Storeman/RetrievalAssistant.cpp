#include <vcl.h>
#pragma hdrstop
#include "RetrievalAssistant.h"
#include "StoreUtil.h"
#include "LCDbAuditTrail.h"
#include "LCDbObject.h"
#include "StringUtil.h"
#include "LCDbProject.h"
#include "RetrievalManager.h"
#include "ReferredBoxes.h"
#include "RetrievalProcess.h"
#pragma package(smart_init)
#pragma resource "*.dfm"

TfrmRetrievalAssistant *frmRetrievalAssistant;

void TfrmRetrievalAssistant::debugLog(String s) { memoDebug->Lines->Add(s); }

__fastcall TfrmRetrievalAssistant::TfrmRetrievalAssistant(TComponent* Owner) : TForm(Owner) { }

#define SGJOBS_COL_DESCRIP      0
#define SGJOBS_COL_JOBTYPE      1
#define SGJOBS_COL_STATUS       2
#define SGJOBS_COL_PRIMARY      3
#define SGJOBS_COL_PROJECT      4
#define SGJOBS_COL_REASON       5
#define SGJOBS_COL_TIMESTAMP    6

void TfrmRetrievalAssistant::init() {
    cbLog->Visible = MYDEBUG;
    memoDebug->Visible = MYDEBUG;
    radgrpMode->ItemIndex = 1;
    sgJobs->Cells[SGJOBS_COL_DESCRIP]   [0] = "Description";
    sgJobs->Cells[SGJOBS_COL_JOBTYPE]   [0] = "Job type";
    sgJobs->Cells[SGJOBS_COL_STATUS]    [0] = "Status";
    sgJobs->Cells[SGJOBS_COL_PRIMARY]   [0] = "Primary Aliquot";
    sgJobs->Cells[SGJOBS_COL_PROJECT]   [0] = "Project";
    sgJobs->Cells[SGJOBS_COL_REASON]    [0] = "Reason";
    sgJobs->Cells[6][0] = "TimeStamp";
    //sgJobs->Cells[5][0] = "UserID";
    sgJobs->ColWidths[SGJOBS_COL_DESCRIP]   = 200;
    sgJobs->ColWidths[SGJOBS_COL_JOBTYPE]   = 120;
    sgJobs->ColWidths[SGJOBS_COL_STATUS]    = 100;
    sgJobs->ColWidths[SGJOBS_COL_PRIMARY]   = 200;
    sgJobs->ColWidths[SGJOBS_COL_PROJECT]   = 100;
    sgJobs->ColWidths[SGJOBS_COL_REASON]    = 200;
    sgJobs->ColWidths[SGJOBS_COL_TIMESTAMP] = 100;
    // c_retrieval_job
    // exercise_cid -> c_object_name
    // external_name
    // description
    // reason          - obsolete
    // job_type
    // project_cid
    // primary_aliquo
    // process_cid
    // status
    // time_stamp       - obsolete
    // start_date
    // claimed_until
    // finish_date
    loadJobs();
}

void __fastcall TfrmRetrievalAssistant::sgJobsDrawCell(TObject *Sender, int ACol, int ARow, TRect &Rect, TGridDrawState State) {
/* clSilver clMoneyGreen  clGray  clSkyBlue
#define RETRIEVAL_ASSISTANT_HIGHLIGHT_COLOUR    clActiveCaption
#define RETRIEVAL_ASSISTANT_NEW_JOB_COLOUR      clMoneyGreen
#define RETRIEVAL_ASSISTANT_IN_PROGRESS_COLOUR  clGreen
#define RETRIEVAL_ASSISTANT_DONE_COLOUR         clSkyBlue
#define RETRIEVAL_ASSISTANT_ERROR_COLOUR        clRed
#define RETRIEVAL_ASSISTANT_DELETED_COLOUR      clGray
 */
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
//	enum Status { NEW_JOB, INPROGRESS, DONE, DELETED = 99 };
//	enum JobKind { UNKNOWN, BOX_MOVE, BOX_RETRIEVAL, BOX_DISCARD, SAMPLE_RETRIEVAL, SAMPLE_DISCARD, NUM_TYPES };
        switch (job->getStatus()) {
        case LCDbCryoJob::Status::NEW_JOB:
            background = RETRIEVAL_ASSISTANT_NEW_JOB_COLOUR;
            break;
        case LCDbCryoJob::Status::INPROGRESS:
            background = RETRIEVAL_ASSISTANT_IN_PROGRESS_COLOUR;
            break;
        case LCDbCryoJob::Status::DONE:
            background = RETRIEVAL_ASSISTANT_DONE_COLOUR;
            break;
        case LCDbCryoJob::Status::DELETED:
            background = RETRIEVAL_ASSISTANT_DELETED_COLOUR;
            break;
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

void __fastcall TfrmRetrievalAssistant::cbLogClick(TObject *Sender) { memoDebug->Visible = cbLog->Checked; }
void __fastcall TfrmRetrievalAssistant::btnExitClick(TObject *Sender) { Close(); }
void __fastcall TfrmRetrievalAssistant::cbNewJobClick(TObject *Sender) { loadJobs(); }
void __fastcall TfrmRetrievalAssistant::cbInProgressClick(TObject *Sender) { loadJobs(); }
void __fastcall TfrmRetrievalAssistant::cbDoneClick(TObject *Sender) { loadJobs(); }
void __fastcall TfrmRetrievalAssistant::cbDeletedClick(TObject *Sender) { loadJobs(); }
void __fastcall TfrmRetrievalAssistant::cbBoxRetrievalClick(TObject *Sender) { loadJobs(); }
void __fastcall TfrmRetrievalAssistant::cbSampleRetrievalClick(TObject *Sender) { loadJobs(); }
void __fastcall TfrmRetrievalAssistant::cbBoxMoveClick(TObject *Sender) { loadJobs(); }
void __fastcall TfrmRetrievalAssistant::cbBoxDiscardClick(TObject *Sender) { loadJobs(); }
void __fastcall TfrmRetrievalAssistant::cbSampleDiscardClick(TObject *Sender) { loadJobs(); }

void TfrmRetrievalAssistant::loadJobs() {
    Screen->Cursor = crSQLWait;
    LQuery qc(LIMSDatabase::getCentralDb());
    LCDbCryoJobs &jobs = LCDbCryoJobs::records();
    jobs.read(LCDbCryoJob::JobKind::UNKNOWN, true); // $2 true: readall

    delete_referenced<tdvecpJob>(vecJobs);
    for (Range< LCDbCryoJob > jr = jobs; jr.isValid(); ++jr) {
        if (!jr->isAvailable()) continue;
        std::ostringstream oss;
        oss<<__FUNC__<<", type: "<<jr->getJobType()<<": desc: "<<jr->getDescription().c_str();
        debugLog(oss.str().c_str());
        if (jr->getStatus() == LCDbCryoJob::Status::NEW_JOB             && !cbNewJob->Checked) continue;
        if (jr->getStatus() == LCDbCryoJob::Status::INPROGRESS          && !cbInProgress->Checked) continue;
        if (jr->getStatus() == LCDbCryoJob::Status::DONE                && !cbDone->Checked) continue;
        if (jr->getStatus() == LCDbCryoJob::Status::DELETED             && !cbDeleted->Checked) continue;
        if (jr->getJobType() == LCDbCryoJob::JobKind::BOX_RETRIEVAL     && !cbBoxRetrieval->Checked) continue;
        if (jr->getJobType() == LCDbCryoJob::JobKind::SAMPLE_RETRIEVAL  && !cbSampleRetrieval->Checked) continue;
        if (jr->getJobType() == LCDbCryoJob::JobKind::BOX_MOVE          && !cbBoxMove->Checked) continue;
        if (jr->getJobType() == LCDbCryoJob::JobKind::BOX_DISCARD       && !cbBoxDiscard->Checked) continue;
        if (jr->getJobType() == LCDbCryoJob::JobKind::SAMPLE_DISCARD    && !cbSampleDiscard->Checked) continue;
        LCDbCryoJob * job = new LCDbCryoJob(); *job = *jr;
        vecJobs.push_back(job);
    }
    showJobs();
    Screen->Cursor = crDefault;
}

void TfrmRetrievalAssistant::showJobs() {
    sgJobs->RowCount = vecJobs.size() + 1;
    //sgJobs->FixedRows = 1;
    tdvecpJob::const_iterator it;
    int row = 1;
    for (it = vecJobs.begin(); it != vecJobs.end(); it++, row++) {
        LCDbCryoJob * job = *it;
//        std::ostringstream oss;
//        oss<<__FUNC__<<", type: "<<jr->getJobType()<<": desc: "<<job->getDescription().c_str();
//        debugLog(oss.str().c_str());
        // jobs have already been filtered; need to be to know size of array
//        if (job->getStatus() == LCDbCryoJob::Status::NEW_JOB            && !cbNewJob->Checked) continue;
//        if (job->getStatus() == LCDbCryoJob::INPROGRESS                 && !cbInProgress->Checked) continue;
//        if (job->getStatus() == LCDbCryoJob::DONE                       && !cbDone->Checked) continue;
//        if (job->getStatus() == LCDbCryoJob::DELETED                    && !cbDeleted->Checked) continue;
//        if (job->getJobType() == LCDbCryoJob::JobKind::BOX_RETRIEVAL    && !cbBox->Checked) continue;
//        if (job->getJobType() == LCDbCryoJob::JobKind::SAMPLE_RETRIEVAL && !cbBox->Checked) continue;

        sgJobs->Cells[SGJOBS_COL_DESCRIP]   [row] = job->getDescription().c_str();
        sgJobs->Cells[SGJOBS_COL_JOBTYPE]   [row] = jobTypeString(job->getJobType()); // UNKNOWN, BOX_MOVE, BOX_RETRIEVAL, BOX_DISCARD, SAMPLE_RETRIEVAL, SAMPLE_DISCARD, NUM_TYPES
        sgJobs->Cells[SGJOBS_COL_STATUS]    [row] = jobStatusString(job->getStatus()); // NEW_JOB, INPROGRESS, DONE, DELETED = 99
        sgJobs->Cells[SGJOBS_COL_PRIMARY]   [row] = getAliquotDescription(job->getPrimaryAliquot()).c_str(); // int
        sgJobs->Cells[SGJOBS_COL_PROJECT]   [row] = getProjectDescription(job->getProjectID()).c_str();
        sgJobs->Cells[SGJOBS_COL_REASON]    [row] = job->getReason().c_str();
        sgJobs->Cells[SGJOBS_COL_TIMESTAMP] [row] = job->getTimeStamp().DateTimeString();
        sgJobs->Objects[0][row] = (TObject *)job;
    }
}

std::string TfrmRetrievalAssistant::getExerciseDescription(int exercise_cid) {
    // c_object_name: 20: storage exercise
    std::ostringstream oss;
    const LCDbObject * exercise = LCDbObjects::records().findByID(exercise_cid);
    oss << exercise->getName().c_str();
    return oss.str();
}

std::string TfrmRetrievalAssistant::getProjectDescription(int project_cid) {
    if (0 == project_cid) return "Project not specified";
    try {
        return LCDbProjects::records().get(project_cid).getName().c_str();
    } catch (...) {
        std::ostringstream oss; oss<<"Project ID "<<project_cid<<" not found"; return oss.str();
    }
}

std::string TfrmRetrievalAssistant::getAliquotDescription(int primary_aliquot_cid) {
    // c_object_name 6: aliquot type?
    std::ostringstream oss;
    if (0 == primary_aliquot_cid) return "Aliquot not specified";
    try {
        const LCDbObject * primary_aliquot = LCDbObjects::records().findByID(primary_aliquot_cid);
        oss << primary_aliquot->getName().c_str();
    } catch (...) {
        oss << "Aliquot ID "<<primary_aliquot_cid<<" not found";
    }
    return oss.str();
}

std::string TfrmRetrievalAssistant::getAuditInfo(int process_cid) {
    // c_audit_trail
    //LCDbCryoJob::getUserID()
    return "";
}

void TfrmRetrievalAssistant::loadBoxes() {
    LQuery qp = Util::projectQuery(0 /*box->project_cid*/, true); // true 2nd param gets ddb

// Find where the boxes are supposed to be:
//    qp.setSQL("select … from box_name n, box_store bs, c_rack_number r, c_tank_map m"
//        " where n.box_cid=bs.box_cid and bs.rack_cid=r.rack_cid and r.tank_cid=m.tank_cid"
//        " and bs.retrieval_cid = jobID");
//
//    sgBoxes->RowCount = 1; sgJobs->FixedRows = 1;
    for (int i; i < 10; i++) {
        //sgBoxes->Cells[][] = "";
    }
}

void __fastcall TfrmRetrievalAssistant::sgJobsDblClick(TObject *Sender) {
    LCDbCryoJob * job = ((LCDbCryoJob *)(sgJobs->Objects[0][sgJobs->Row]));
    switch (job->getJobType()) {
    case LCDbCryoJob::JobKind::SAMPLE_RETRIEVAL:
    case LCDbCryoJob::JobKind::BOX_RETRIEVAL:
        // OK
        break;
    default:
        return;
    }

    switch (job->getStatus()) {
    case LCDbCryoJob::Status::NEW_JOB:
    case LCDbCryoJob::Status::INPROGRESS:
        // OK
        break;
    default:
        return;
    }

    // make INPROGRESS?

    switch (radgrpMode->ItemIndex) {
    case 0: // manage
        if (    job->getJobType() == LCDbCryoJob::JobKind::SAMPLE_RETRIEVAL
            &&  job->getStatus() == LCDbCryoJob::Status::NEW_JOB) {
            frmRetrievalManager->autochunk = (IDYES == Application->MessageBox(L"Do you want to automatically create chunks for this list?", L"Question", MB_YESNO));
        } else {
            frmRetrievalManager->autochunk = false; // and form type = ?
        }
        frmRetrievalManager->setJob(job);
        frmRetrievalManager->ShowModal();
        break;
    case 1: // process
        frmRetrievalProcess->setJob(job);
        frmRetrievalProcess->ShowModal();
        break;
    default:
        throw Exception("Unknown mode");
    }
}




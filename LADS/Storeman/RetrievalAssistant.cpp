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
#include "RetrievalAssistantSamples.h"
#pragma package(smart_init)
#pragma resource "*.dfm"

/*
// template
    ostringstream oss; oss<<__FUNC__; debugLog(oss.str().c_str());
    LQuery q(LIMSDatabase::getCentralDb());
    //LQuery q(Util::projectQuery(project), true); // get ddb with central and project dbs
    q.setSQL("SELECT * FROM  WHERE status != 99");
    Screen->Cursor = crSQLWait;
    q.open();
    delete_referenced<vecp>(s);
    while (!q.eof()) {
        RetrievalPlan * plan = new RetrievalPlan(q.readString("name"));
        //ob-> = q.readInt("");
        //ob-> = q.readString("");
        s.push_back();
        q.next();
    }
    Screen->Cursor = crDefault;
*/

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

/*
StoreMan allows the user to create lists of boxes or cryovials to be retrieved for analysis (8.2) or disposal (7.2).
Sample retrieval for analysis may specify two aliquots: the secondary can be used if the primary is not available.
The retrieval assistant (8.3) guides a user through the retrieval process.
The first stage, when the retrieval assistant is given a new list to work on,
is to create a retrieval plan divided up into manageable chunks.

Box retrieval

    c_retrieval_job.status = new job (0); job type = box retrieval (2) or disposal (3)

    Find where the boxes are currently stored:

    Select 
        b.external_name as box, 
        s.external_name as site, 
        m.position, 
        v.external_full as vessel, 
        m.shelf_number,
        r.external_name as rack, 
        bs.slot_position
    from 
        box_name b, 
        box_store bs, 
        c_rack_number r, 
        c_tank_map m, 
        c_object_name s, 
        c_object_name v
    where 
        b.box_cid = bs.box_cid and 
        bs.rack_cid = r.rack_cid and 
        r.tank_cid = m.tank_cid and 
        s.object_cid = location_cid and 
        v.object_cid = storage_cid and 
        bs.retrieval_cid = :jobID; // e.g. -636363

    *List the name, current structure and expected location of each box.
    The location should include site+position+name+layout, as it does in StoreMan’s storage browser.
    Allow the user to divide up the list (if necessary; see below for more details).
    Show each chunk in turn and allow the user to sort by location,
    otherwise show the whole list and allow them to sort that.
    Ask the user to save changes with the option of going back to re-order if necessary.
    Insert a record into c_box_retrieval for each box in turn and update c_retrieval_job: set status=in progress (1)

Sample retrieval

    c_retrieval_job.status = new job (0); job type = sample retrieval (4) or disposal (5)

    Find the samples to be retrieved:

    Select 
        cryovial_barcode, 
        t.external_name as aliquot, 
        b.external_name as box,
        cryovial_position, 
        s.external_name as site, 
        m.position, 
        v.external_full as vessel, 
        shelf_number,
        r.external_name as rack, 
        bs.slot_position 
    from 
        cryovial c, 
        cryovial_store cs, 
        box_name b, 
        box_store bs,
        c_rack_number r, 
        c_tank_map m, 
        c_object_name s,    -- site
        c_object_name v,    -- vessel 
        c_object_name t     -- aliquot?
    where 
        c.cryovial_id = cs.cryovial_id and 
        b.box_cid = cs.box_cid and 
        b.box_cid = bs.box_cid and 
        bs.status = 6 and   -- 6?
        t.object_cid = aliquot_type_cid and 
        bs.rack_cid = r.rack_cid and 
        r.tank_cid = m.tank_cid and 
        s.object_cid = location_cid and 
        v.object_cid = storage_cid and 
        cs.retrieval_cid = :jobID;

    At least half of these will be for the primary aliquot
    (i.e. cryovial.aliquot_type_cid = c_retrieval_job.primary_aliquot).

    The others may be for the secondary aliquot.

    Primary and secondary tubes may have the same barcode but should always come from specimen entries
    with the same source name.

    Entries for the primary aliquot may have a destination box defined.
    You could find these using a left join:

    select
        ...
    from
        cryovial_store s1
    left join
        cryovial c on c.cryovial_id = s1.cryovial_id
    left join
        box_name n1 on n1.box_cid = s1.box_cid
    left join
        cryovial_store s2 on s1.cryovial_id = s2.cryovial_id and
        s2.status = 0
    left join
        box_name n2 on n2.box_cid = s2.box_cid
    where
        s1.retrieval_cid = :jobID

    If no destination boxes have been defined, ask for the box type and create suitable entries in box_name.

    Piece this information together to create a list giving the destination box+position,
    cryovial barcode and current box+position+structure+location of the primary and secondary aliquots.

    The user may want to export and/or import the list to specify the retrieval plan – this needs further thought.

    Display the size of the job and ask user if they want to divide up the list.  If they do:
    1.	Ask them the maximum chunk size (default = 500 cryovials)
    2.	Calculate slots/box (where c_box_size.box_size_cid = box_content.box_size_cid)
    3.	Ask them to select the size of first chunk from a list – it must be a multiple of the box size (from 2)
        and no more than the maximum (from 1)
    4.	Allocate the appropriate number of destination boxes to the first chunk
    5.	Repeat steps (2) and (3) until every entry has been allocated to a chunk

    After division, show each chunk in turn and allow the user to sort by location,
    otherwise show the whole list and allow them to sort that.

    Ask the user to save changes with the option of going back to re-order if necessary.

    Insert an entry into c_box_retrieval for each destination box, recording the chunk it is in,
    and a record into l_cryovial_retrieval for each cryovial, recording its position in the list.

    Update c_retrieval_job: set status=in progress (1)

    Other jobs

    c_retrieval_job.status = in progress (1) and job.type in (2,3,4,5): act on list [tbd]
    c_retrieval_job.status not in (0,1) or job.type not in (2,3,4,5): complain

*/

void TfrmRetrievalAssistant::init() {
    cbLog->Visible = RETRASSTDEBUG;
    memoDebug->Visible = RETRASSTDEBUG;
    //radgrpMode->ItemIndex = 0;
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
enum Status { NEW_JOB, INPROGRESS, DONE, DELETED = 99 };
enum JobKind { UNKNOWN, BOX_MOVE, BOX_RETRIEVAL, BOX_DISCARD, SAMPLE_RETRIEVAL, SAMPLE_DISCARD, NUM_TYPES };
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
    tdvecpJob::const_iterator it;
    int row = 1;
    for (it = vecJobs.begin(); it != vecJobs.end(); it++, row++) {
        LCDbCryoJob * job = *it;
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
    switch (job->getStatus()) {
    case LCDbCryoJob::Status::NEW_JOB: // manage
        switch (job->getJobType()) {
        case LCDbCryoJob::JobKind::SAMPLE_RETRIEVAL:
            frmSamples->setJob(job);
            if (IDOK == frmSamples->ShowModal()) {
                // then make INPROGRESS?
            }
            break;
        case LCDbCryoJob::JobKind::BOX_RETRIEVAL:
            frmBoxes->setJob(job);
            if (IDOK == frmBoxes->ShowModal()) {
                // then make INPROGRESS?
            }
            break;
        default:
            throw Exception("Unknown job type");
        }
        break;
    case LCDbCryoJob::INPROGRESS: // process
        switch (job->getJobType()) {
        case LCDbCryoJob::JobKind::SAMPLE_RETRIEVAL:
            frmProcess->setJob(job);
            frmProcess->ShowModal();
            break;
        case LCDbCryoJob::JobKind::BOX_RETRIEVAL:
            frmProcess->setJob(job);
            frmProcess->ShowModal();
            break;
        default:
            throw Exception("Unknown job type");
        }
        break;
    default:
        throw Exception("Unknown status");
    }
}

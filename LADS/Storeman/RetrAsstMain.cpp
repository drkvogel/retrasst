#include <vcl.h>
#pragma hdrstop
#include "RetrAsstMain.h"
#include "LCDbAuditTrail.h"
#include "LCDbObject.h"
#include "LCDbProject.h"
#include "RetrAsstPlanSamples.h"
#include "RetrAsstPlanBoxes.h"
#include "RetrAsstCollectSamples.h"
#include "RetrAsstCollectBoxes.h"
#include "StoreUtil.h"
#include "StoreDAO.h"
#include "LCDbTankMap.h"

#pragma package(smart_init)
#pragma resource "*.dfm"

TfrmRetrievalAssistant *frmRetrievalAssistant;
__fastcall TfrmRetrievalAssistant::TfrmRetrievalAssistant(TComponent* Owner) : TForm(Owner) { }

// moved from header file: linker error under 64-bit XE4
//void TfrmRetrievalAssistant::msgbox(const char * main, const char * title ) {
//	Application->MessageBoxW(String(main).w_str(), String(title).w_str(), MB_OK);
//}

void TfrmRetrievalAssistant::msgbox(string main, string title) {
	Application->MessageBoxW(String(main.c_str()).c_str(), String(title.c_str()).c_str(), MB_OK);
}

//void TfrmRetrievalAssistant::msgbox(String main, string title ) {
//	Application->MessageBoxW(main.w_str(), String(title.c_str()).c_str(), MB_OK);
//}

void __fastcall TfrmRetrievalAssistant::FormResize(TObject *Sender) { sgwJobs->resize(); }

void __fastcall TfrmRetrievalAssistant::cbLogClick(TObject *Sender) { panelDebug->Visible = cbLog->Checked; }

void __fastcall TfrmRetrievalAssistant::btnExitClick(TObject *Sender) { Close(); }

void __fastcall TfrmRetrievalAssistant::cbNewJobClick(TObject *Sender) { loadJobs(); }

void __fastcall TfrmRetrievalAssistant::cbInProgressClick(TObject *Sender) { loadJobs(); }

void __fastcall TfrmRetrievalAssistant::cbDoneClick(TObject *Sender) { loadJobs(); }

void __fastcall TfrmRetrievalAssistant::cbDeletedClick(TObject *Sender) { loadJobs(); }

void __fastcall TfrmRetrievalAssistant::cbRejectedClick(TObject *Sender) { loadJobs(); }

void __fastcall TfrmRetrievalAssistant::cbBoxRetrievalClick(TObject *Sender) { loadJobs(); }

void __fastcall TfrmRetrievalAssistant::cbSampleRetrievalClick(TObject *Sender) { loadJobs(); }

void __fastcall TfrmRetrievalAssistant::cbBoxMoveClick(TObject *Sender) { loadJobs(); }

void __fastcall TfrmRetrievalAssistant::cbBoxDiscardClick(TObject *Sender) { loadJobs(); }

void __fastcall TfrmRetrievalAssistant::cbSampleDiscardClick(TObject *Sender) { loadJobs(); }

void __fastcall TfrmRetrievalAssistant::sgJobsDrawCell(TObject *Sender, int ACol, int ARow, TRect &Rect, TGridDrawState State) {
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

void __fastcall TfrmRetrievalAssistant::sgJobsDblClick(TObject *Sender) {
    LCDbCryoJob * job = ((LCDbCryoJob *)(sgJobs->Objects[0][sgJobs->Row]));
    if (NULL == job) return;
    switch (job->getStatus()) {
    case LCDbCryoJob::Status::NEW_JOB: // create plan
        switch (job->getJobType()) {
            case LCDbCryoJob::JobKind::SAMPLE_RETRIEVAL:
                frmRetrAsstPlanSamples->setJob(job);
                if (mrOk == frmRetrAsstPlanSamples->ShowModal()) {
                    job->setStatus(LCDbCryoJob::INPROGRESS);
                    job->saveRecord(LIMSDatabase::getCentralDb());
                }
                break;
            case LCDbCryoJob::JobKind::BOX_RETRIEVAL:
                frmRetrAsstPlanBoxes->setJob(job);
                if (mrOk == frmRetrAsstPlanBoxes->ShowModal()) {
                    job->setStatus(LCDbCryoJob::INPROGRESS);
                    job->saveRecord(LIMSDatabase::getCentralDb());
                }
                break;
            default:
                throw runtime_error("Unknown job type");
        }
        break;
    case LCDbCryoJob::INPROGRESS: // process
        switch (job->getJobType()) {
        case LCDbCryoJob::JobKind::SAMPLE_RETRIEVAL:
            frmRetrAsstCollectSamples->setJob(job);
            if (mrOk == frmRetrAsstCollectSamples->ShowModal()) {
                job->setStatus(LCDbCryoJob::Status::DONE);
                job->saveRecord(LIMSDatabase::getCentralDb());
            }
            break;
        case LCDbCryoJob::JobKind::BOX_RETRIEVAL:
            frmRetrAsstCollectBoxes->setJob(job);
            if (mrOk == frmRetrAsstCollectBoxes->ShowModal()) {
                job->setStatus(LCDbCryoJob::Status::DONE);
                job->saveRecord(LIMSDatabase::getCentralDb());
            }
            break;
        default:
            throw runtime_error("Unknown job type");
        }
        break;
    default:
        msgbox("Cannot process a job with this status"); return;
    }
}

void __fastcall TfrmRetrievalAssistant::sgJobsClick(TObject *Sender) {
    ostringstream oss; //oss;// << __FUNC__;
    LCDbCryoJob * job = ((LCDbCryoJob *)(sgJobs->Objects[0][sgJobs->Row]));
    if (NULL == job) return;
    oss << endl << "job: "<<job->getID()<<", projectid: "<<job->getProjectID()<<", status: "<<(job->getStatus());//oss<<sgwJobs->printColWidths();
    debugLog(oss.str().c_str());
}

void __fastcall TfrmRetrievalAssistant::FormClose(TObject *Sender, TCloseAction &Action) {
    delete_referenced<tdvecpJob>(vecJobs);
    delete sgwJobs;
}

string TfrmRetrievalAssistant::getProjectDescription(int project_cid) {
    if (0 == project_cid) return "Project not specified";
    try {
        return LCDbProjects::records().get(project_cid).getName().c_str();
    } catch (...) {
        ostringstream oss; oss<<"Project ID "<<project_cid<<" not found"; return oss.str();
    }
}

string TfrmRetrievalAssistant::getAuditInfo(int process_cid) {
    // c_audit_trail
    //fixmeLCDbCryoJob::getUserID();
    return "";
}

string TfrmRetrievalAssistant::getExerciseDescription(int exercise_cid) { // c_object_name: 20: storage exercise
    ostringstream oss;
    const LCDbObject * exercise = LCDbObjects::records().findByID(exercise_cid);
    oss << exercise->getName().c_str(); return oss.str();
}

void TfrmRetrievalAssistant::debugLog(String s) { memoDebug->Lines->Add(s); }

void TfrmRetrievalAssistant::init() {
    cbLog->Checked      = RETRASSTDEBUG;
    cbLog->Visible      = true;//= RETRASSTDEBUG;
	panelDebug->Visible = cbLog->Checked;

    sgwJobs = new StringGridWrapper<LCDbCryoJob>(sgJobs, &vecJobs);
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
    loadJobs();
}

void TfrmRetrievalAssistant::loadJobs() {
    Screen->Cursor = crSQLWait;
    LQuery qc(LIMSDatabase::getCentralDb());
    jobs = LCDbCryoJobs::records();
	jobs.read(LIMSDatabase::getCentralDb(), LCDbCryoJob::UNKNOWN, true); // $2 true: readall
	delete_referenced<tdvecpJob>(vecJobs);
    for (Range< LCDbCryoJob > jr = jobs; jr.isValid(); ++jr) {
        if (!jr->isAvailable()) continue;
        ostringstream oss;
        oss<<__FUNC__<<", type: "<<jr->getJobType()<<": desc: "<<jr->getDescription().c_str();
        debugLog(oss.str().c_str());
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

void __fastcall TfrmRetrievalAssistant::btnResetJobsClick(TObject *Sender) {
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

void TfrmRetrievalAssistant::clearStorageCache() {
    storageCache.clear();
}

void TfrmRetrievalAssistant::getStorage(SampleRow * sample) {
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
            sample->setLocation("No records found", 0, "", 0, 0, "", 0);
        }
        storageCache[sample->store_record->getBoxID()] = sample; // cache result
    }
}

void TfrmRetrievalAssistant::combineAliquots(const vecpSampleRow & primaries, const vecpSampleRow & secondaries, vecpSampleRow & combined) {

    struct PosKey {
    /** compound of box and position used for index into map of box + pos -> sample */
        PosKey(int b, int p) : box(b), pos(p) { }
        //PosKey(SampleRow * s) : box(s->dest_cryo_pos), pos(s->dest_box_id) { }
        PosKey(SampleRow * s) : box(s->dest_box_id), pos(s->dest_cryo_pos) { }
        int box, pos;
        bool operator <(const PosKey &other) const {
            if (box < other.box) {
                return true;
            } else if (box == other.box) {
                return pos < other.pos;
            } else {
                return false;
            }
        }
    };

    typedef std::map< PosKey, SampleRow * > posCache;
    posCache cache;

    // store primaries and cache
    combined.clear();
    for (vecpSampleRow::const_iterator it = primaries.begin(); it != primaries.end(); it++) {
        SampleRow * row = *it;
        PosKey key(row);
        cache[key] = row; // cache combination of dest box and pos
        combined.push_back(row);
    }

    // try to match secondaries based on same box/pos key
    for (vecpSampleRow::const_iterator it = secondaries.begin(); it != secondaries.end(); it++) {
        SampleRow * row = *it;
        PosKey key(row);
        posCache::iterator found = cache.find(key);
        if (found != cache.end()) { // destination box and position already used (by primary)
            if (NULL == row)
                throw runtime_error("null in cache");
            found->second->backup = row; // add as backup to primary
        } else {
            combined.push_back(row);     // add to list in its own right
        }
    }
}

//template <class T>
//int Chunk<T>::getStatus() {
//    bool complete = true;
//    bool not_started = true;
//    for (int i=0; i<getSize(); i++) {
//        int status = rowAt(i)->retrieval_record->getStatus();
//        switch (status) {
//            case LCDbCryovialRetrieval::EXPECTED:
//                complete = false; break;
//            case LCDbCryovialRetrieval::IGNORED:
//            case LCDbCryovialRetrieval::COLLECTED:
//            case LCDbCryovialRetrieval::NOT_FOUND:
//                not_started = false; break;
//            default:
//                throw "unexpected LCDbCryovialRetrieval status";
//        }
//    }
//    if (complete) {
//        return DONE;
//    } else if (not_started) {
//        return NOT_STARTED;
//    } else {
//        return INPROGRESS;
//    }
//}
//    using namespace boost::local_time;
//    //local_date_time
//
//    time_t t(time(NULL)); // current time
//    tm tm(*localtime(&t));
//    std::locale loc("");
//    const std::time_put<char> &tput = std::use_facet< std::time_put< char > >(loc);
//    tput.put(oss.rdbuf(), oss, _T('\0'), &tm, _T('x'));

// LCDbBoxRetrieval::Status::NEW|PART_FILLED|COLLECTED|NOT_FOUND|DELETED
// LCDbCryovialRetrieval::Status::EXPECTED|IGNORED|COLLECTED|NOT_FOUND

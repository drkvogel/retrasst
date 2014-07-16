#include <vcl.h>
#pragma hdrstop

#include "RetrAsstMain.h"
#include "RetrAsstCollectBoxes.h"
#include "RetrAsstCollectSamples.h"
#include "RetrAsstPlanBoxes.h"
#include "RetrAsstPlanSamples.h"

#pragma package(smart_init)
#pragma link "RetrievalListBase"
#pragma resource "*.dfm"

TfrmRetrievalAssistant *frmRetrievalAssistant;

__fastcall TfrmRetrievalAssistant::TfrmRetrievalAssistant(TComponent* Owner) : TfrmRetrievalJobList(Owner) { }

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
    loadJobs();
}


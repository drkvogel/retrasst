[DAR not working](dar-not-working-on-vlabdev.md) was a red herring.

SavePlanThread::Execute()

    try {
        save();
        frmRetrAsstPlanSamples->ModalResult = mrOk; // save and close here rather than OnTerminate in case of exception
            // why ??? surely not good behaviour?
    } catch (std::exception & e) {
        debugMessage = e.what(); Synchronize((TThreadMethod)&debugLog);
    }    

crashes on 

    ModalResult = mrCancel;

wherever called (in worker thread or not)

here:

    void __fastcall TfrmRetrAsstPlanSamples::FormClose(...) {
        combined.clear();
        delete_referenced< vector <SampleRow * > >(secondaries);
        delete_referenced< vector <SampleRow * > >(primaries); <-- crash

delete_references(primaries) already done in LoadVialsJobThread::load()

deletes secondaries, then primaries - but after combineAliquots, secondaries might be linked to primaries - does this cause a problem?

delete_references(primaries) seems to go through deleting things merrily until a delete cause the freeze

commented out all delete_references calls in FormClose(). Freeze avoided. 
But will that cause problems when form is loaded next time?

Try opening form again:
Loading samples, please wait...
and gets stuck there.

BUT IT WAS WORKING BEFORE!!

No changes (SourceTree: File status, log selected) that look like they could have caused this since 03/06 (today is 27/06).

But plans have been saved (started, INPROGRESS) on 

    4/6: id: 979187, name: "Retrieval_2", desc: "Reveal cyovials - use EDTA4 if 3 not found", type: 4, status: 1, project: -582959, primary: -31783, secondary: -833348, reason: "order 66"

    25/6 (yesterday): id: 979372, name: "Retrieval_4", desc: "A simple Friday afternoon retrieval", type: 4, status: 1, project: -1212564, primary: -31783, secondary: -833348, reason: "order 66"

The one that is causing problems today is 

    id: 979369, name: "Retrieval_3", desc: "Primary and secondary, no location", type: 4, status: 0, project: -1212564, primary: -31781, secondary: -31782, reason: "order 66"

Is there something different about it? 
Reset 979187 "Reveal cyovials - use EDTA4 if 3 not found"
After uncommenting delete_referenced from FormClose() (back to previous behaviour), this job seems to save OK (apart from that I'd removed ModalResult = mrOk, which is needed to cause the main form to save the status of the job as INPROGRESS).

## conclusion

So perhaps it is something different about this job - 

check size of sec and prim on close
130 and 130 
???



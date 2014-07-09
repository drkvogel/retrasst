[DAR not working](dar-not-working-on-vlabdev.md) was a red herring.

SavePlanThread::Execute()

    try {
        save();
        frmRetrAsstPlanSamples->ModalResult = mrOk; // save and close here rather than OnTerminate in case of exception
            // why ??? surely not good behaviour?
    } catch (std::exception & e) {
        debugMessage = e.what(); Synchronize((TThreadMethod)&debugLog);
    }    

crashes on `ModalResult = mrCancel;` wherever called (in worker thread or not), here:

    void __fastcall TfrmRetrAsstPlanSamples::FormClose(...) {
        combined.clear();
        delete_referenced< vector <SampleRow * > >(secondaries);
        delete_referenced< vector <SampleRow * > >(primaries); <-- crash

`delete_references(primaries)` already done in `LoadVialsJobThread::load()`

deletes secondaries, then primaries - but after combineAliquots, secondaries might be linked to primaries - does this cause a problem?

`delete_references(primaries)` seems to go through deleting things merrily until a delete cause the freeze

commented out all `delete_references` calls in `FormClose()`. Freeze avoided. 
But will that cause problems when form is loaded next time?

Try opening form again:

> Loading samples, please wait...

and gets stuck there. BUT IT WAS WORKING BEFORE!!

No changes (SourceTree: File status, log selected) that look like they could have caused this since 03/06 (today is 27/06).

But plans have been saved (started, INPROGRESS) on 

    4/6: id: 979187, name: "Retrieval_2", desc: "Reveal cyovials - use EDTA4 if 3 not found", type: 4, status: 1, project: -582959, primary: -31783, secondary: -833348, reason: "order 66"

    25/6 (yesterday): id: 979372, name: "Retrieval_4", desc: "A simple Friday afternoon retrieval", type: 4, status: 1, project: -1212564, primary: -31783, secondary: -833348, reason: "order 66"

The one that is causing problems today is 

    id: 979369, name: "Retrieval_3", desc: "Primary and secondary, no location", type: 4, status: 0, project: -1212564, primary: -31781, secondary: -31782, reason: "order 66"

Is there something different about it? 
Reset 979187 "Reveal cyovials - use EDTA4 if 3 not found"
After uncommenting delete_referenced from FormClose() (back to previous behaviour), this job seems to save OK (apart from that I'd removed ModalResult = mrOk, which is needed to cause the main form to save the status of the job as INPROGRESS).

I don't know why I haven't seen this problem before or why it doesn't happen to other jobs - it should happen to any job with combined aliquots. 
because secondaries are deleted first - and the ->backup ptr in the primaries will still point to the old memory
perhaps only primaries or loose other aliquots should be deleted, and if they have a backup, the dtor should delete that as well

## conclusion

So perhaps it is something different about this job - 

check size of sec and prim on close: 130 and 130 ???
perhaps the clue is in the name: "Primary and secondary, no location"
does destroying a sample with no location cause a crash?

    ~SampleRow() {
        if (store_record) delete store_record;
        if (cryo_record) delete cryo_record;
        if (backup) delete backup;
        if (lcr_record) delete lcr_record;
    }

can't inspect any of these, e.g. "Error inspecting 'store_record':" - why?
but can inspect 'c' in delete_referenced(Container& c). no variable names makes it pretty difficult...
a lot delete ok before the error so difficult to find and no backtrace.
put OutputDebugStrings on delete_referenced... (try/catch?)
is an exception type being ignored? disabled ignore for all exceptions I could find, still no info. 
time to go home.

use auto_ptr or something similar (C++11?) or...
dtor checks for not null, but who said ptr is set null in the first place?

2014-07-02

freeze only happens on desktop??? freezing version I did yesterday and put onto laptop for demo didn't freeze in demo... oh yes it did

2014-07-09

appears to happen on last sample of last chunk... nothing looks unusual....

use unique_ptr?

secondaries.size(): 130
primaries.size():   130

put OutputDebugString in with convoluted syntax:

    wstringstream oss; oss<<"deleting: "<<(sample->debug_str().c_str());
    OutputDebugString(oss.str().c_str());

last output:

Debug Output: deleting: id: 1713, proj: -1212564, status: 2, barc: "120000132", aliq: -31781 "EDTA_1", cryo_status: 2, src: {-751, "BEST-D EDTA_1_retrieval 53_751", pos: 45}, dst: {-20189 "EDTA1_2 3_20189", pos: 28, type: 978201 "QClot_new"}, loc: {Unconfirmed[0]: :0[0]/[0]} Process Storeman.exe (12544)

and crashes at

    ~SampleRow() {
        if (store_record) delete store_record; <-- here

but `store_record` is accessed to print out `debug_str()` just before, and contains data (`store_record->getBoxID()`, `store_record->getPosition()`), even though `storage_str()` prints previously saved information ("Unconfirmed").

It's not the last sample, anyway - it's the 9th in chunk 3, barcode 120000132

Is it always that one? No. 

Is it store_record? Not necessarily...
Would thought it more likely to be `backup` as that's the one we just deleted... 
Easy way to find out: take out `delete backup;`, see if it fails - no, it doesn't.

Could make `backup` a `unique_ptr` or `shared_ptr`, or...

As we know that we just deleted all the backups, make `backup` NULL and/or don't try to delete it.

In plan, we know they're deleted, but not so in collect...
So keep `delete backup` in dtor, and NULL them before deleting.




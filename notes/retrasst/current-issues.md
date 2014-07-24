
## new buttons

Add Note
 no existing form (existing code?)
 multiple notes can be added
 existing notes can't be altered
Already Retrieved
Reality Schism

Glyphs for buttons are in `C:\Program Files (x86)\Embarcadero\RAD Studio\11.0\Images\Buttons`

## border icons

BorderStyle

## exit doesn't exit

exit doesn't exit!! fixed with ModalResult = mrCancel, but

## saveProgressThreadTerminated called twice

on exit

1st:
TfrmStoremain::BtnRetrieveClick
TfrmRetrievalAssistant::sgJobsDblClick

2nd:
TfrmStoremain::BtnRetrieveClick
(no sgJobsDblClick)

## detach from db switch?

for dev w/o db?

## 

## updateStorage() updates everything

instead of current chunk


if (chunk.status == DONE) # no EXPECTED or IGNORED vials)
    chunkCompleted()

chunkCompleted()
    signOff()
    saveProgress()

SaveProgressThread::Execute()
    updateStorage() 
    findEmpties() # updates emptyBoxes

updateStorage()
    # needs work

saveProgressThreadTerminated()
    if (!emptyBoxes.empty()):
        collectEmpties()    
    if (isJobComplete()):
        closeJob()
        ModalResult = mrOk
    else:
        msgbox("unactioned samples; not marked as finished")

closeJob()
    foreach `c_box_retrieval`:
        LCDbBoxRetrieval cbr.setStatus(LCDbBoxRetrieval::COLLECTED)
    LCDbCryoJob.setStatus(DONE)




## chunk should be complete

SaveProgressThread::updateStorage()
    updateStorage(SampleRow * aliquot)
        LPDbCryovialStore * current = aliquot->store_record
        if (NULL != current)
            switch (aliquot->lcr_record->getStatus()) {
                case LCDbCryovialRetrieval::EXPECTED:
                case LCDbCryovialRetrieval::IGNORED:
                    throw runtime_error("chunk should be complete")

No it shouldn't, necessarily. Use cases:

### 1. chunk finished

The user got to the end of a chunk - it may not be complete because this includes ignored vials. But current state should be signed off.

    save progress
    if job complete # all vials processed, not ignored
        close job
        exit

### 2. user exits

User quits voluntarily. Progress must be signed off and saved.

need to differentiate between end of chunk reached and chunk completed

    save progress
    if !current chunk completed # including ignored
        exit?

i.e. if the current chunk is not finished, the user must have pressed Exit in order for saveProgress to be initiated. But if the current chunk is finished, saveProgress was initiated by reaching the end of the chunk? 

## aside

I don't work spectacularly fast, I must admit.

But given that it's taken until recently to get some feedback from Martin, which led to some quite major changes, could it have been done any quicker? Martin being too busy was blocker, but I've used that time to iron out bugs which may save a lot of time after testing.

## move RetrAsstThread into RetrAsstMain.h

.

## finish last chunk

should `chunk->nextUnresolvedAbs()` be called in `showCurrentRow()`?

    chunk->setRowAbs(chunk->nextUnresolvedAbs());

but nextUnresolvedAbs() now returns NONE_FOUND (-1?) if chunk complete

## confusing methods?

currentSample(), 
currentAliquot()
chunk->currentObject()
fillRow()

---

nextRow()
    if DONE == chunk->getStatus()
        chunkCompleted()
     if (isJobComplete())
        if (IDYES != Application->MessageBox(L"Save job? Are all chunks completed?", L"Info", MB_YESNO)) return;
        ModalResult = mrOk;    

chunkCompleted()

isJobComplete() 
    true if all chunks complete

exit()
calls:

SaveProgressThread::Execute()

    foreach sample
        find source box and cache
        if status not in EXPECTED, IGNORED
            storeSample
        else
            unactionedSamples = true
        if backup
            storeSample(sample->backup)

SaveProgressThread::storeSample()

SaveProgressThread::jobFinished()

saveProgressThreadTerminated
-> collectEmpties()


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

The user got to the end of a chunk - it may not be complete because this includes ignored vials. 

    save progress
    if job complete # all vials processed, not ignored
        close job
        exit

### 2. user exits

    save progress
    if !current chunk finished
        exit?

i.e. if the current chunk is finished 

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

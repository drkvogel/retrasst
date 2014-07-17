What is now `TfrmRetrievalJobList::combineAliquots` can be moved (back) into `LoadVialsJobThread`

Utility methods (ones that are used by more than one class) should be put into StoreUtil.cpp

e.g. getStorage()?

Thing about `getStorage()` is that it's specific to Retrieval Assistant at the moment and references SampleRow


## chunk should be complete

SaveProgressThread::updateStorage()
    updateStorage(SampleRow * aliquot)
        LPDbCryovialStore * current = aliquot->store_record
        if (NULL != current)
            switch (aliquot->lcr_record->getStatus()) {
                case LCDbCryovialRetrieval::EXPECTED:
                case LCDbCryovialRetrieval::IGNORED:
                    throw runtime_error("chunk should be complete")

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

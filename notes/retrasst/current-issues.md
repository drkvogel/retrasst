
## chunk should be complete

SaveProgressThread::updateStorage()
    updateStorage(SampleRow * aliquot)
        LPDbCryovialStore * current = aliquot->store_record
        if (NULL != current)
            switch (aliquot->lcr_record->getStatus()) {
                case LCDbCryovialRetrieval::EXPECTED:
                case LCDbCryovialRetrieval::IGNORED:
                    throw runtime_error("chunk should be complete")

## Process ID not yet allocated

LCDbAuditTrail::getProcessID()
    if (hasStarted())
        return current->getID()
    throw Exception("Process ID not yet allocated")

Had made a checkbox ("Done") true on overriding ManageLists form, wondered if that commit [853c1cf] had caused it. Reverted using SourceTree File Status -> Log Selected -> Reset to Commit.

It does seem to cause it. 

Changing checkboxes programmatically in TfrmManageLists::FormCreate seems to cause it as well. How strange. No-one's looking at Manage Lists at the moment anyway, so forget for now.




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

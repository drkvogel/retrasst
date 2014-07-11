## repeating myself?

nextRow()
    chunkCompleted(chunk)

chunkCompleted()
    frmConfirm->ShowModal()
    foreach row
        updateStorage()
    collectEmpties()
    saveProgress()
    if (isJobComplete())
        //jobFinished();//??
        ModalResult = mrOk

updateStorage()
    LPDbCryovialStore * current->saveRecord()
    LPDbBoxName boxName = *(pBoxName) ?? # BoxName method to update cryovials?

FormClose()
    //exit()

menuItemExitClick()
    checkExit()

btnExitClick() 
    checkExit()    

checkExit()
    if IDYES == "Are you sure?
        exit()

exit()
    frmConfirm->ShowModal()
    saveProgress()

saveProgress()
    saveProgressThread = new SaveProgressThread()

SaveProgressThread::Execute()
    # lots
    unactionedSamples = false
    foreach sample in combined
        sourceBox = sample.store_record.BoxID
        boxes.cache(sourceBox)
        status = sample.lcr_record.status
        switch status
            case LCDbCryovialRetrieval::EXPECTED:
            case LCDbCryovialRetrieval::IGNORED:
                    if sample.backup not null
                        switch sample.backup.lcr_record.status
                            case LCDbCryovialRetrieval::EXPECTED
                            case LCDbCryovialRetrieval::IGNORED
                                unactionedSamples = true
                    else
                        unactionedSamples = true
        storeSample(sample) # always 'store' even if ignored
    emptyBoxes.clear()
    foreach box in boxes
        vialRemains = false
        SetOfVials & setOfVials = found.second
        SetOfVials::const_iterator it;
        foreach sample in setOfVials # for each vial in the box
            switch sample.lcr_record.status
                case LCDbCryovialRetrieval::EXPECTED
                    vialRemains = true
        if !vialRemains # empty/completed box, mark for discard
            emptyBoxes.insert(box) # set of int used in discardBoxes()


SaveProgressThread::storeSample()
    ? nyi

SaveProgressThread::jobFinished()
    foreach in `c_box_retrieval WHERE retrieval_cid = job->getID()`
    LCDbBoxRetrieval cbr.setStatus(LCDbBoxRetrieval::COLLECTED)
    cbr.saveRecord()

saveProgressThreadTerminated()
    collectEmpties()

collectEmpties()
    foreach box in std::set<int>emptyBoxes    
        LPDbBoxName BoxName = boxNames.findByID(box_cid)
        boxes.push_back(boxName) # get box objects from ids
    ?
    frmCollectEmpties->ShowModal()
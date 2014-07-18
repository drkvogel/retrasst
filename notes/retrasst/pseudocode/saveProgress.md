void TfrmRetrAsstCollectSamples::nextRow() {
/** save primary (and backup) in l_cryovial_retrieval, but not cryovial/_store at this point, move to next */
    Chunk< SampleRow > * chunk = currentChunk();
    SampleRow * sample = chunk->currentObject(); // which may be the backup aliquot

    if (!sample->lcr_record->saveRecord(LIMSDatabase::getCentralDb())) { throw runtime_error("saveRecord() failed"); }
    if (sample->backup) { // fixme what is being saved if backup was not needed?
        if (!sample->backup->lcr_record->saveRecord(LIMSDatabase::getCentralDb())) { throw runtime_error("saveRecord() failed for backup"); }
    } // deferred (IGNORED) vials are not actually saved to the database, they remain EXPECTED

    int nextAbs = chunk->nextUnresolvedAbs(); // fast-forward to first non-dealt-with row
    if (nextAbs != Chunk< SampleRow >::NextUnresolvedStatus::NONE_FOUND) {
        chunk->setRowAbs(nextAbs);
    else // past last row
        if (chunk->getSection() < (int)chunks.size()) {
            sgChunks->Row = sgChunks->Row+1; // next chunk
        } else {
            sgChunks->Row = 1; //  could cause a loop but should be picked up by isJobComplete() //TfrmRetrievalAssistant::msgbox("End of last chunk");

    showChunks(); // don't need to save chunk - completedness or otherwise of 'chunk' should be implicit from box/cryo plan
    labelPrimary->Enabled = true; labelSecondary->Enabled = false; editBarcode->Clear(); ActiveControl = editBarcode; // focus for next barcode

    if (Chunk< SampleRow >::Status::DONE == chunk->getStatus()) { // check if chunk is complete (no EXPECTED or IGNORED vials)
        chunkCompleted(chunk);

void TfrmRetrAsstCollectSamples::chunkCompleted(Chunk< SampleRow > * chunk) {
/* * Require user to sign off
    * update cryo store records
    * calculate if there are any empty boxes
        * create tick list or switch list of boxes, empty/otherwise
        * ask user to comfirm that empty boxes are in fact empty
    * if error, create referred box (INVALID/EXTRA/MISSING CONTENT?) in `c_box_name` and/or `c_slot_allocation` */

    // Require user to sign off, skip in debug
    frmConfirm->initialise(TfrmSMLogin::RETRIEVE, "Ready to sign off boxes"); // std::set<int> projects; projects.insert(job->getProjectID()); frmConfirm->initialise(TfrmSMLogin::RETRIEVE, "Ready to sign off boxes", projects);
    if (!RETRASSTDEBUG && mrOk != frmConfirm->ShowModal()) {
        Application->MessageBox(L"Signoff cancelled", L"Info", MB_OK);
        return; // fixme what now?
    }
    saveProgress();

void TfrmRetrAsstCollectSamples::saveProgress() {
    info.clear(); warnings.clear(); errors.clear();
    saveProgressThread = new SaveProgressThread();
    saveProgressThread->OnTerminate = &saveProgressThreadTerminated;

/** check cryo/store old/new params correct for `LCDbCryovialRetrieval`
`c_box_retrieval`: set `time_stamp`, `status` = 1 (PART_FILLED)
`box_name` (if record): update `time_stamp`, `box_capacity`, `status=1` (IN_USE)
`c_box_name` (if record): update `time_stamp`, `box_capacity`, `status=1` (IN_USE)
check at save and exit if all vials in a box have been saved and if so update box */
void __fastcall SaveProgressThread::Execute() {

    try {
        updateStorage();
    } catch (...) {

    }

    typedef std::set< SampleRow * > SetOfVials;
    typedef std::map< int, SetOfVials > VialsInBoxesMap;
    VialsInBoxesMap boxes;
    VialsInBoxesMap::iterator found;

    for (auto& sample : collect->combined) {
        int sourceBox = sample->store_record->getBoxID(); // should get id of secondary box as well and add it to map, we are checking for all empty boxes

        // build a map of source box ID to a set of vials contained in that box
        found = boxes.find(sourceBox);
        if (found == boxes.end()) { // not added yet
            SetOfVials setOfVials;
            setOfVials.insert(sample);
            boxes[sourceBox] = setOfVials;
        } else { // already in map
            found->second.insert(sample);
        } ostringstream oss; oss<<sample->cryovial_barcode<<" "<<sample->aliquotName<<" in box with id "<<sourceBox; updateStatus(oss.str());
    }

    collect->emptyBoxes.clear();

    // check for completed boxes
    for (auto &found : boxes) { // for each source box
        bool vialRemains = false;
        SetOfVials & setOfVials = found.second;
        SetOfVials::const_iterator it;
        for (auto &sample : setOfVials) { // for each vial in the box
            switch (sample->lcr_record->getStatus()) { // what about secondaries?
            case LCDbCryovialRetrieval::EXPECTED:
                vialRemains = true;
            default:
                ;
            }
        }
        if (!vialRemains) { // empty/completed box, mark for discard
            collect->emptyBoxes.insert(found.first); // set of int box_cids, used in collectEmpties()
        }
    }
}

void SaveProgressThread::updateStorage() {
    for (auto& sample : collect->combined) {
    //for (int row=0; row < chunk->getSize(); row++) { // update cryo store records
        //SampleRow * sample = chunk->objectAtRel(row);
        LQuery pq(LIMSDatabase::getProjectDb(sample->project_cid));
        updateStorage(sample, pq);
        if (sample->backup) {
            updateStorage(sample->backup, pq); //??? could be different for backup?
        }
    }
}

void SaveProgressThread::updateStorage(SampleRow * aliquot, LQuery & q) {

/* * should be 4 cryovial_store recs/sample: src + dest * primary + secondary
    * new `NOT_FOUND` status (ALLOCATED, CONFIRMED, MOVE_EXPECTED, DESTROYED, ANALYSED, TRANSFERRED, NOT_FOUND, DELETED = 99) (no IGNORED status?)
    * NOT_FOUND will be 6, not 7 - unless there is supposed to be another new status?

    // handle backups
    // deal with ignored, expected

    * if primary aliquot found:
        - primary src TRANSFERRED?
        - primary dest CONFIRMED?
        - secondary src CONFIRMED?
        - secondary dest DELETED?
        - l_cryo
    * if secondary aliquot found:
        - primary src NOT_FOUND?
        - primary dest DELETED?
        - secondary src TRANSFERRED?
        - secondary dest CONFIRMED?
    * `cryovial_store` (source and dest, primary and secondary)
        * Primary, source:
            * if found: update `removed`, `status=5` (TRANSFERRED)
            * else `status=7` (NOT_FOUND??)
        * Primary, dest:
            * if found: update `time_stamp`, `status=1` (STORED)
            * else `status=99`
        * Secondary, src:
            * if primary found: clear `retrieval_cid`, `status=1`
            * else if secondary found update `removed`, `status=5`,
            * else `status=7` (NOT_FOUND)
        * Secondary, dest:
            * if primary found: `status=99`
            * else if secondary found: update `time_stamp`, `status=1`,
            * else `status=99` */

/** update cryovial_store (old and new, primary and secondary) when they enter their password to confirm */
/* LPDbCryovialStore
enum Status { ALLOCATED, CONFIRMED, MOVE_EXPECTED, DESTROYED, ANALYSED, ALIQUOTS_TAKEN, NOT_FOUND, TRANSFERRED, DELETED = 99 };
    void setStatus( Status update ) { status = update; }
    Status getStatus() const { return Status( status ); }
    int getPosition() const { return position; }
    int getBoxID() const { return boxID; }
    bool saveRecord( LQuery query );
    bool setJobRef( LQuery project, int jobRef );    */

    LPDbCryovialStore * current = aliquot->store_record;
    if (NULL != current) {

        switch (aliquot->lcr_record->getStatus()) {
            case LCDbCryovialRetrieval::EXPECTED:
            case LCDbCryovialRetrieval::IGNORED:
                throw runtime_error("chunk should be complete");
            case LCDbCryovialRetrieval::COLLECTED:
                current->setStatus(LPDbCryovialStore::TRANSFERRED); break; //???
            case LCDbCryovialRetrieval::NOT_FOUND:
                current->setStatus(LPDbCryovialStore::NOT_FOUND); break;
            default:
                throw runtime_error("unexpected status");
        }
        current->saveRecord(q);
    } // else //throw runtime_error("NULL store_record");

    /* LPDbCryovialStore(int id, int cryovial, int box, short pos) */
    // add to box and use LPDbBoxName methods?
    LPDbBoxNames boxNames;
    boxNames.readFilled(q); // reads CONFIRMED [2], ANALYSED [3]. boxes.readCurrent(qp) reads EMPTY [0], IN_USE [1]
    const LPDbBoxName * pBoxName;

    //int box_cid = *idIt;
    pBoxName = boxNames.findByID(aliquot->dest_box_id);
    if (NULL == pBoxName) { // IN_TANK [4] records not read and findByID for those ids returns a null
        throw runtime_error("box not found"); //???
    else
        LPDbBoxName boxName = *(pBoxName); // make mutable object
        //boxes.push_back(boxName)

    //LPDbCryovialStore retrieved(id, cryovial, box, pos); //???
//    LPDbCryovialStore retrieved(0,0,0,0); //???
//    switch (aliquot->lcr_record->getStatus()) {
//        case LCDbCryovialRetrieval::EXPECTED:
//        case LCDbCryovialRetrieval::IGNORED:
//            throw runtime_error("chunk should be complete");
//        case LCDbCryovialRetrieval::COLLECTED:
//            retrieved.setStatus(LPDbCryovialStore::TRANSFERRED); break; //???
//        case LCDbCryovialRetrieval::NOT_FOUND:
//            retrieved->setStatus(LPDbCryovialStore::NOT_FOUND); break;
//        default:
//            throw runtime_error("unexpected status");
//    }
//    retrieved.saveRecord();
    /*
    LQuery qp(Util::projectQuery(job->getProjectID(), false));
    LPDbBoxNames boxNames;
    boxNames.readFilled(qp); // reads CONFIRMED [2], ANALYSED [3]. boxes.readCurrent(qp) reads EMPTY [0], IN_USE [1]
    const LPDbBoxName * pBoxName;

    int box_cid = *idIt;
    pBoxName = boxNames.findByID(box_cid);
    if (NULL == pBoxName) { // IN_TANK [4] records not read and findByID for those ids returns a null
        throw runtime_error("box not found"); //???
    } else {
        LPDbBoxName boxName = *(pBoxName); // make mutable object
        boxes.push_back(boxName);
    }*/
}

// `cryovial_store`: as above (dealt with already?)

void __fastcall TfrmRetrAsstCollectSamples::saveProgressThreadTerminated(TObject *Sender) {
    DEBUGSTREAM(__FUNC__<<"save plan for job "<<job->getID()<<" finished")
    hideProgressMessage(); //progressBottom->Style = pbstNormal; progressBottom->Visible = false; panelLoading->Visible = false;
    Screen->Cursor = crDefault; Enabled = true;

    collectEmpties(); // were we exiting or just finished a chunk? does it matter?
                      // yes, because you don't want to close the form if just finishing a chunk

    if (isJobComplete()) { // job is complete
        if (IDYES != Application->MessageBox(L"Save job? Are all chunks completed?", L"Info", MB_YESNO)) return;
        jobFinished();
        ModalResult = mrOk;
    else
        TfrmRetrievalAssistant::msgbox("There are unactioned samples in this retrieval job; will not be marked as finished"); // fixme

bool TfrmRetrAsstCollectSamples::isJobComplete() { /** are all chunks complete? */
    for (auto &chunk : chunks) 
        if (Chunk< SampleRow >::Status::DONE != chunk->getStatus()) return false; 
    return true

void TfrmRetrAsstCollectSamples::jobFinished() 
    LQuery qp(Util::projectQuery(job->getProjectID(), false))
    LQuery qc(LIMSDatabase::getCentralDb());

    // all boxes must be finished if here (ie. all samples are finished) // is this necessary???
    qc.setSQL("SELECT * FROM c_box_retrieval WHERE retrieval_cid = :rtid");
    qc.setParam("rtid", job->getID());
    qc.open();
    while (!qc.eof()) {
        // `c_box_retrieval`: set `time_stamp`, `status=2` (collected)
        LCDbBoxRetrieval cbr(qc);
        cbr.setStatus(LCDbBoxRetrieval::COLLECTED); //???
        cbr.saveRecord(LIMSDatabase::getCentralDb()); // time_stamp set by default - should be in 2.7.2

        // `box_name`: (if record): update `time_stamp`, `box_capacity`, `status=2` (CONFIRMED)
        // `c_box_name`: (if record): update `time_stamp`, `box_capacity`, `status=2` (CONFIRMED)
    }

    // `c_retrieval_job`: update `finish_date`, `status` = 2
    job->setStatus(LCDbCryoJob::DONE);
    job->saveRecord(qc); // finish date is updated by this method

void TfrmRetrAsstCollectSamples::collectEmpties()

    // create tick list or switch list of boxes, empty/otherwise
    // ask user to comfirm that empty boxes are in fact empty
    // if error, create referred box (INVALID/EXTRA/MISSING CONTENT?) in `c_box_name` and/or `c_slot_allocation`
    // `c_box_name` and `c_slot_allocation` fields together == `l_box_arrival`
    // calculate if there are any empty boxes

/** if, at the end of processing a chunk, there are any source boxes which have become empty,
    the user may want to discard them instead of replacing them.
    if so, provide an option to discard these empty boxes, recording it in the database */

/*  * collect empties (all vials "accepted" or "not found") for discard
    * at the end of processing each chunk, if source boxes are now empty
    * unlikely for test tasks but rat tanks may throw old boxes away
    * all source boxes from a reorganisation task should end up empty
    * ask user to confirm that vessel/structure/slot is now empty
    * otherwise box should be referred */
    Application->MessageBox(L"Handle disposal of empty boxes", L"Info", MB_OK);

    LQuery qp(Util::projectQuery(job->getProjectID(), false)); // fixme project-specific
    LPDbBoxNames boxNames;
    boxNames.readFilled(qp); // reads CONFIRMED [2], ANALYSED [3]. boxes.readCurrent(qp) reads EMPTY [0], IN_USE [1]
    const LPDbBoxName * pBoxName;
    typedef std::vector< LPDbBoxName > VecBoxes;
    VecBoxes boxes;

    if (!emptyBoxes.empty())
        if (IDYES != Application->MessageBox(L"There are empty boxes. Would you like to mark these as discarded?", L"Info", MB_YESNO)) {
            throw runtime_error("user did not want to deal with empty boxes");
            // then mark them REFERRED
    else 
        return

    std::set< int >::const_iterator idIt;
    for (idIt = emptyBoxes.begin(); idIt != emptyBoxes.end(); idIt++) {
        int box_cid = *idIt;
        pBoxName = boxNames.findByID(box_cid);
        if (NULL == pBoxName) { // IN_TANK [4] records not read and findByID for those ids returns a null
            throw runtime_error("box not found"); //???
        } else {
            LPDbBoxName boxName = *(pBoxName); // make mutable object
            boxes.push_back(boxName);
        }
    }

    VecBoxes::const_iterator boxIt;
    //for (boxIt = boxes.begin(); boxIt != boxes.end(); boxIt++)
        //LPDbBoxName box = *boxIt;
        // add to tick/switch list

        //boxName.setStatus(LPDbBoxName::Status::IN_TANK);
        //LQuery cq(LIMSDatabase::getCentralDb()); // for extra param to LPDbBoxName::saveRecord()
        //boxName.saveRecord(qp, cq);

    //}
    if (!(boxes.empty()))
        frmCollectEmpties->ShowModal()

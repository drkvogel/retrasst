class SampleRow : public RetrievalRow
    LPDbCryovial            * cryo_record; // auto_ptr for these?
    LPDbCryovialStore       * store_record;
    LCDbCryovialRetrieval   * retrieval_record;

class LCDbBoxRetrieval : public LCDbID
    int rj_box_cid, retrieval_cid, box_id, project_cid, section, status;
    enum Status { NEW, PART_FILLED, COLLECTED, NOT_FOUND, DELETED = 99, NUM_STATUSES };

why are rows not changing status/colour any more?

timerLoadPlanTimer()
    loadChunk()

loadChunk()
    start LoadPlanWorkerThread

LoadPlanWorkerThread
    create SampleRow objects, put them into Chunk objects
    if secondary aliquots, store those within primaries
    get storage details for all aliquots using getStorage()

getStorage(sample)
    fill in storage details for sample

loadPlanWorkerThreadTerminated()
    showChunks()
    currentChunk()->setCurrentRow(0)
    showCurrentRow()

rowAt()    
    return sgw->rows->at((start)+(pos))

showChunks()
    foreach chunk
        put details into sg
    showChunk()        

showChunk(chunk)
    if (NULL == chunk) chunk = currentChunk()
    foreach row
        sampleRow = chunk->rowAt(row)
        fillRow(sampleRow, row+1)
    showCurrentRow()

currentChunk() 
    if (sgChunks->Row < 1) sgChunks->Row = 1 # force selection of 1st row
    chunk = sgChunks->Objects[0][sgChunks->Row]
    return chunk

btnSkipClick()
    currentSample()->retrieval_record->setStatus(LCDbCryovialRetrieval::IGNORED)
    showCurrentRow()
    nextRow()

currentSample()
    chunk = currentChunk()
    current = chunk->getCurrentRow()
    sample = chunk->rowAt(current)
    return  (NULL != sample->secondary) ? sample->secondary : sample

showCurrentRow()
    rowIdx = currentChunk()->getCurrentRow()
    if (rowIdx == currentChunk()->getSize()) # past the end, chunk completed
        sample = NULL;              # no details to show
        sgVials->Row = rowIdx       # just show the last row
    else
        sample = currentChunk()->rowAt(rowIdx)
        sgVials->Row = rowIdx+1     # allow for header row
    showDetails(sample)

showDetails(sample)
    display details of sample in status bar

nextRow()
    chunk = currentChunk()
    current = chunk->getCurrentRow()
    sample = currentSample() # which may be the secondary aliquot # SampleRow * sample = chunk->rowAt(current);
    # save both primary and secondary
    # sample->retrieval_record->saveRecord(LIMSDatabase::getProjectDb())
    # sample->secondary->retrieval_record->saveRecord(LIMSDatabase::getProjectDb())
    if (current < chunk->getSize()-1)
        lookAhead = sgVials->VisibleRowCount/2
        if (current+lookAhead < chunk->getSize()-1)
            sgVials->Row = current+lookAhead+1 # bodge to scroll next few samples into view; ScrollBy doesn't seem to work
        else
            sgVials->Row = sgVials->RowCount-1
        chunk->setCurrentRow(current+1)
        showCurrentRow()
    else # skipped last row
        chunk->setCurrentRow(current+1) # past end to show complete?
        # don't save - completedness or otherwise of 'chunk' should be implicit from box/cryo plan
        if (chunk->getSection() < (int)chunks.size())
            sgChunks->Row = sgChunks->Row+1 # next chunk
        else
            if (IDYES != Application->MessageBox(L"Save job? Are all chunks completed?", L"Info", MB_YESNO)) return
            # todo : Handle disposal of empty boxes
    # labelPrimary->Enabled = true labelSecondary->Enabled = false;
    showChunks()
    # editBarcode->Clear()
    # ActiveControl = editBarcode # focus for next barcode

accept(barcode)
    # check correct vial # could be missing, swapped etc
    sample = currentSample()
    switch (sample->retrieval_record->getStatus())
        case LCDbCryovialRetrieval::EXPECTED:
        case LCDbCryovialRetrieval::IGNORED:
            # ok, carry on
        case LCDbCryovialRetrieval::COLLECTED:
            msgbox("Already collected")
        case LCDbCryovialRetrieval::NOT_FOUND:
            if (IDOK != Application->MessageBox(L"Confirm sample has now been found", L"Question", MB_OKCANCEL)) 
                return
    if (barcode == sample->cryovial_barcode)
        sample->retrieval_record->setStatus(LCDbCryovialRetrieval::COLLECTED)
        nextRow()
    else
        Application->MessageBox(L"Barcode not matched", L"Info", MB_OK)

skip()
    currentSample()->retrieval_record->setStatus(LCDbCryovialRetrieval::IGNORED)
    showCurrentRow()
    nextRow()

notFound()
    rowIdx = currentChunk()->getCurrentRow()
    sample = currentChunk()->rowAt(rowIdx)
    if (sample->secondary)
        fillRow(sample->secondary, rowIdx+1)
        showCurrentRow() # got storage already
        showDetails(sample->secondary)
    else
        currentSample()->retrieval_record->setStatus(LCDbCryovialRetrieval::NOT_FOUND)
        nextRow()

addChunk(row)
    if (chunks.size() == 0)  # first chunk, make default chunk from entire listrows
        newchunk = new Chunk(sgwVials, chunks.size()+1, 0, row) # empty chunk, don't know how big it will be yet
    else
        chunks[chunks.size()-1]->setEnd(row-1)
        newchunk = new Chunk(sgwVials, chunks.size()+1, row, row)
    chunks.push_back(newchunk)

fillRow(sampleRow, row)
    put details into sg

## todo

 * currentSample() returns the secondary aliquot if present; now that they're all loaded if available, any changes made to the status of the 'currentSample()' are made to the secondary whether selected or not; result: it looks like nothing has happened (to the primary). This is wrong.
 * 

 * changes to status not apparent
    * because currentSample() returns secondary if loaded and secondary is now loaded by default, see above
    * return secondary only if primary is NOT_FOUND?
 * save both primary and secondary to database

## done

OnClick
    show details

process() 
    # not used, remove

## c++ into python-like pseudocode:
    strip out 
        {}, ;
        .c_str()
        type declarations
        return types (void etc)
    -> into .
    // comments into #
    /* */ into """

### language options

 * python (st2 plugin)
 * javascript (in browser)
 * PHP
 * regex
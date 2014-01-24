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


fillRow()
    vial    = sampleRow->cryo_record
    store   = sampleRow->store_record

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


accept()
    nextRow();

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

 * python (st2 macro?)
 * javascript (in browser)
 * PHP
 * regex

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

addChunk(row)
    if (chunks.size() == 0)  # first chunk, make default chunk from entire listrows
        newchunk = new Chunk(sgwVials, chunks.size()+1, 0, row) # empty chunk, don't know how big it will be yet
    else
        chunks[chunks.size()-1]->setEnd(row-1)
        newchunk = new Chunk(sgwVials, chunks.size()+1, row, row)
    chunks.push_back(newchunk)

fillRow(sampleRow, row)
    put details into sg

## Suggestions

OnClick
    show details


process() # not used
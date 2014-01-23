class SampleRow : public RetrievalRow
    LPDbCryovial            * cryo_record; // auto_ptr for these?
    LPDbCryovialStore       * store_record;
    LCDbCryovialRetrieval   * retrieval_record;

class LCDbBoxRetrieval : public LCDbID
    int rj_box_cid, retrieval_cid, box_id, project_cid, section, status;
    enum Status { NEW, PART_FILLED, COLLECTED, NOT_FOUND, DELETED = 99, NUM_STATUSES };

why are rows not changing status/colour any more?

btnSkipClick()
    currentSample()->retrieval_record->setStatus(LCDbCryovialRetrieval::IGNORED);
    showCurrentRow();
    nextRow();


currentSample()
    Chunk< SampleRow > * chunk = currentChunk();
    int current = chunk->getCurrentRow();
    SampleRow * sample = chunk->rowAt(current);
    return  (NULL != sample->secondary) ? sample->secondary : sample;

Chunk< SampleRow > * currentChunk() 
    if (sgChunks->Row < 1) sgChunks->Row = 1; // force selection of 1st row
    Chunk< SampleRow > * chunk = (Chunk< SampleRow > *)sgChunks->Objects[0][sgChunks->Row];
    if (NULL == chunk) throw Exception("null chunk");
    return chunk;

showCurrentRow();
nextRow();
accept()
    nextRow();


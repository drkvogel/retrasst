void TfrmSamples::showChunk(Chunk< SampleRow > * chunk) {
    Screen->Cursor = crSQLWait; Enabled = false;
    debugLog("showChunk");

    if (NULL == chunk) { chunk = currentChunk(); }

    if (chunk->getSize() <= 0) { //?? error surely
        sgwVials->clear();
    } else {
        sgVials->RowCount = chunk->getSize()+1;
        sgVials->FixedRows = 1;
    }

    for (int row=0; row < chunk->getSize(); row++) {
        SampleRow *         sampleRow = chunk->rowAt(row);
        LPDbCryovial *      vial    = sampleRow->cryo_record;
        LPDbCryovialStore * store   = sampleRow->store_record;
        int rw = row+1; // for stringgrid
        sgVials->Cells[sgwVials->colNameToInt("barcode")]  [rw] = sampleRow->cryovial_barcode.c_str();
        sgVials->Cells[sgwVials->colNameToInt("aliquot")]  [rw] = sampleRow->aliquot_type_name.c_str();
        sgVials->Cells[sgwVials->colNameToInt("currbox")]  [rw] = sampleRow->src_box_name.c_str();
        sgVials->Cells[sgwVials->colNameToInt("currpos")]  [rw] = sampleRow->store_record->getPosition();
        sgVials->Cells[sgwVials->colNameToInt("site"   )]  [rw] = sampleRow->site_name.c_str();
        sgVials->Cells[sgwVials->colNameToInt("vesspos")]  [rw] = sampleRow->vessel_pos;
        sgVials->Cells[sgwVials->colNameToInt("vessel" )]  [rw] = sampleRow->vessel_name.c_str();
        sgVials->Cells[sgwVials->colNameToInt("shelf"  )]  [rw] = sampleRow->shelf_number;
        sgVials->Cells[sgwVials->colNameToInt("structpos")][rw] = sampleRow->structure_pos;
        sgVials->Cells[sgwVials->colNameToInt("struct" )]  [rw] = sampleRow->structure_name.c_str();
        sgVials->Cells[sgwVials->colNameToInt("boxpos" )]  [rw] = sampleRow->box_pos;
        sgVials->Cells[sgwVials->colNameToInt("destbox")]  [rw] = sampleRow->dest_box_name.c_str();
        sgVials->Cells[sgwVials->colNameToInt("destpos")]  [rw] = sampleRow->dest_cryo_pos;
        sgVials->Objects[0][rw] = (TObject *)sampleRow;
    }
    sgVials->Row = 1;

    Screen->Cursor = crDefault; Enabled = true;
}

bool TfrmSamples::addChunk(unsigned int offset) {
/** Add chunk starting at specified row [of the specified size?]
    offset: number of rows after beginning of previous chunk at which to cut off new chunk
    return: is there any space for more? */
    if (vials.size() == 0) return false; //throw "vials.size() == 0"; // not an error strictly; not by my program anyway!
    int numvials  = vials.size(); int numchunks = chunks.size();

    Chunk< SampleRow > * curchunk, * newchunk;
    if (chunks.size() == 0) { // first chunk, make default chunk from entire listrows
        newchunk = new Chunk< SampleRow >(sgwVials, chunks.size()+1, 0, vials.size()-1); // 0-indexed // size is calculated
    } else {
        if (offset <= 0 || offset > vials.size()) throw "invalid offset"; // ok only for first chunk
        curchunk = currentChunk();
        int currentchunksize = curchunk->getSize(); // no chunks until first added
        if (curchunk->getStart()+offset > vials.size()) { // current last chunk is too small to be split at this offset
            return false; // e.g. for auto-chunk to stop chunking
        }
        curchunk->setEnd(curchunk->getStart()+offset-1); // row above start of new chunk
        newchunk = new Chunk< SampleRow >(sgwVials, chunks.size()+1, curchunk->getStart()+offset, vials.size()-1);
    }
    chunks.push_back(newchunk);
    if (frmAutoChunk->Visible) {
        frmAutoChunk->BringToFront();
        FocusControl(frmAutoChunk);
    }
    return true;
}
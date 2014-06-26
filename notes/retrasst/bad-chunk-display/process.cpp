void TfrmProcess::showChunk(Chunk< SampleRow > * chunk) {
    Screen->Cursor = crSQLWait; Enabled = false;

    if (NULL == chunk) { chunk = currentChunk(); } // default

    if (chunk->getSize() <= 0) {
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
        sgVials->Objects[0][row] = (TObject *)sampleRow;
    }
    sgVials->Row = 1;

    Screen->Cursor = crDefault; Enabled = true;
}

void TfrmProcess::addChunk(int row) {
    Chunk< SampleRow > * chunk;// = new Chunk< SampleRow >;
    chunk = new Chunk< SampleRow >(sgwVials, chunks.size()+1, 1, row);
    chunks.push_back(chunk);
}
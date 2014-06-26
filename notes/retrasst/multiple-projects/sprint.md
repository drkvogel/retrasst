# mad dash

get rid of primaries, secondaries.

## nextRow()

I changed it recently and buggered it up: why did I change it?

`.rej` files contains the rejected 'hunk' from SourceTree:

    $ catDS/Storeman/RetrAsstCollectSamples.cpp.rej
    diff a/LADS/Storeman/RetrAsstCollectSamples.cpp b/LADS/Storeman/RetrAsstCollectSamples.cpp      (rejected hunks)
    @@ -669,16 +669,18 @@
     * save secondary as `IGNORED` if not required? primary was */
         Chunk< SampleRow > * chunk = currentChunk();
         //int current = chunk->getRowRel(); //SampleRow * sample = currentAliquot();  //
         SampleRow * sample = chunk->currentObject(); // which may be the secondary aliquot
         
         // save changes both primary and secondary in l_cryovial_retrieval (not cryovial/_store at this point)
    +    // deferred (IGNORED) vials are not actually saved written to db...
         if (!sample->lcr_record->saveRecord(LIMSDatabase::getCentralDb())) { throw runtime_error("saveRecord() failed"); }
    -    if (sample->backup) {
    +    if (sample->backup) { // backup not needed
    +        sample->backup->lcr_record->setStatus(LCDbCryovialRetrieval::IGNORED); // ...unless not needed
             if (!sample->backup->lcr_record->saveRecord(LIMSDatabase::getCentralDb())) { throw runtime_error("saveRecord() failed for secondary"); }
    -    } // deferred (IGNORED) vials are not actually saved to the database, they remain EXPECTED
    +    }
          
         // don't need to save chunk - completedness or otherwise of 'chunk' should be implicit from box/cryo plan
         if (chunk->getRowRel() < chunk->getSize()-1) {
             chunk->setRowAbs(chunk->nextUnresolvedAbs()); // fast-forward to first non-dealt-with row
         } else { // last row
             if (chunk->getSection() < (int)chunks.size()) {


isJobComplete()

chunkCompleted()

SaveProgressThread::Execute()

discardBoxes() 

xe4 on laptop-
reply to thread
solve the problems
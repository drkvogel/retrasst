## accept() 

does NOT_FOUND instead of COLLECTED!
SourceTree:
 log selected
 open selected (opens tmp file in builder)

happened here:

    Commit: 6ae6ad1eb98156c19812cc05581ed001a4cd4609 [6ae6ad1]
    Parents: 3be80aed5e
    Author: Chris Bird <chris.bird@ctsu.ox.ac.uk>
    Date: 06 June 2014 17:15:43

It was just plain wrong. 
Thing is, there are several points where an action has to be carried out on a row, which might be: 

 * primary with no backup
 * primary with backup
 * secondary backup

and no common code to distinguish which with. currentAliquot() sort of does this, but how to send it different actions to apply? Sounds like a closure or a lambda type of thing (what's [the difference](http://scottmeyers.blogspot.co.uk/2013/05/lambdas-vs-closures.html)? "Closures are to lambdas as objects are to classes").

C++11 has [closures named lambdas](http://stackoverflow.com/questions/12635184/do-we-have-closures-in-c).

A [lambda function](http://www.cprogramming.com/c++11/c++11-lambda-closures.html) is a function that you can write inline in your source code, usually to pass in to another function, similar to the idea of a functor or function pointer.

(should be allowed time to learn about this - what's that about a commitment to training again? Ask for it).

But for now, handling primaries and secondaries can be done separately at each point.



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


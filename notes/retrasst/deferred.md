## handling of primaries and secondaries/backups

Thing is, there are several points where an action has to be carried out on a row, which might be: 

 * primary with no backup
 * primary with backup
 * secondary backup

and no common code to distinguish which with. currentAliquot() sort of does this, but how to send it different actions to apply? Sounds like a closure or a lambda type of thing (what's [the difference](http://scottmeyers.blogspot.co.uk/2013/05/lambdas-vs-closures.html)? "Closures are to lambdas as objects are to classes").

C++11 has [closures named lambdas](http://stackoverflow.com/questions/12635184/do-we-have-closures-in-c).

A [lambda function](http://www.cprogramming.com/c++11/c++11-lambda-closures.html) is a function that you can write inline in your source code, usually to pass in to another function, similar to the idea of a functor or function pointer.

(should be allowed time to learn about this - what's that about a commitment to training again? Ask for it).

But for now, handling primaries and secondaries can be done separately at each point.

## cache/hash column names

not important

## more loading messages

to show something's happening

## move Util::getAliquotDescription() to LCDbCryoJob?

So `job.str()` can print meaningful aliquot types

## debugLog() should take std::string?

    debugLog()
and should be refactored into one function

### row colours

secondary (expected) should perhaps be green - ie. stronger version of primary colour

colours should be consistent between plan and retrieve

## box types are all the same?

    e.g. 978238 QClot_new 978237
    are they meant to be? yes, no jobs with multiple box types yet
    take out of main loop

#### Error inspecting <symbol>

In `LCDbCryoJob::saveRecord()`, couldn't inspect `central`.

Similar to [this](http://qc.embarcadero.com/wc/qcmain.aspx?d=68452)?

Project settings ("Debug configuration - 64-bit Windows platform") say "Disable all optimizations" is true, though.

## Misc

 * select for process samples in testing/q2.sql
 * use DEBUGSTREAM
 * Usual sort order before chunking: dest box, pos
 * Usual sort order/chunk: vess pos, shelf, structure, slot, curr pos

## Deferred

 * crashes on release build?
 * boxes form doesn't work properly - turn the handle (but not for demo)
 * put lookAhead into chunk class
 * factor out chunk logic?
 * Note: a sample retrieval can include boxes that do not have their current locations recorded in the database.
why can't I inspect chunk?
look for ??? /newrow
 * port [Plan|Process]Samples to [Plan|Process]Boxes
 * something to bear in mind for processing when it eventually goes live is that the database might not reflect reality perfectly - vials might be missing or swapped etc.
   won't come up till live testing, but worth thinking about at this stage of development
 * demo possible; email martin. make sure there are sufficient example jobs - e.g. "1743 THRIVE boxes - few (EDTA1 & EDTA2)"- 
 * Retrieval Assistant
    * save changes with the option of going back to re-order if necessary.
    * Cryogenics staff can reject a retrieval list if it does not have a retrieval plan
 * up/down arrows to show column sort
 * Only read currently selected chunk/aliquot from db when selected?
 * canned searches - save, delete e.g. site name, vessel pos, structure pos, slot, source box pos
 * define behaviour for manual chunk split in landlocked chunk-
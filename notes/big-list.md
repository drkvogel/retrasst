## todo

 * update cryovial_store (old and new, primary and secondary) when they enter their password to confirm
 * save secondary as 'IGNORED' if not required? primary was 
    * secondary aliquots should always be saved if present
 * All source boxes from a reorganisation task should end up empty
 * collect empties (all vials "accepted" or "not found") for discard
    * ask user to confirm that vessel/structure/slot is now empty
    * otherwise box should be referred
 * put lookAhead into chunk class

showCurrentRow gets triggered twice
    sgChunksClick
        showChunk
            #showCurrentRow # just once
            # now not at all

    skip --> nextRow --> showCurrentRow --> 
    skip --> nextRow --> showChunks --> showChunk --> showCurrentRow

 * Insert a record into c_box_retrieval for each box in turn and update c_retrieval_job: set status=in progress (1)
 * Ask the relevant question(s) from the URS when they’re ready to finish and 
 * check cryo/store old/new params are correct for LCDbCryovialRetrieval
 * slot number? (what about it?)
 * C_box_retrieval: set time_stamp, status = 1 
 * If the user exits without finishing the job, once they've signed off: 
    * Cryovial_store: 
        * Primary, source: if found update removed, status = 5, otherwise status = 7 
        * Primary, destination: if found update time_stamp, status = 1, otherwise status = 99 
        * Secondary, source: if primary found clear retrieval_cid, status = 1, otherwise if secondary found update removed, status = 5, otherwise set status = 7 
        * Secondary, destination: if primary found set status = 99, otherwise if secondary found update time_stamp, status = 1, otherwise status = 99 
    * Box_name (if record found): 
        * update time_stamp, box_capacity, status = 1 
    * C_box_name (if record found): 
        * update time_stamp, box_capacity, status = 1 
 * At the end of a job, once they've signed off: 
     * C_box_retrieval: set time_stamp, status = 2 
     * Cryovial_store: as above 
     * Box_name (if record found): update time_stamp, box_capacity, status = 2 
     * C_box_name (if record found): update time_stamp, box_capacity, status = 2 
     * C_retrieval_job: update finish_date, status = 2 
 * "source" and "destination" (boxes) > "old"/"new" or "current"/"future"
  * c_box_name is copied from box_name by a cron job
    * do I now use c_box_name in one of my queries? yes, because it speeds things up
    * in which case, box changes should be made to both c_box_name, and box_name (if a box_name record is present - not always the case). The cron job should be able to cope with this (up to date c_box_name entries already existing)
    * setting the timestamps correctly (to 'now') is crucial 
 * changes to c_box_retrieval, cryovial_store should be done on signoff and exit
 * there should be 4 cryovial_store records for each requested sample: source and destination for primary and secondary aliquots
    * NOT_FOUND will be a new status
    * currently ALLOCATED, CONFIRMED, MOVE_EXPECTED, DESTROYED, ANALYSED, TRANSFERRED, DELETED = 99
    * no IGNORED status?
    * if primary aliquot is found:
        - set primary src TRANSFERRED?
        - set primary dest CONFIRMED?
        - set secondary src CONFIRMED?
        - set secondary dest DELETED?
        - set l_cryo 
    * if secondary aliquot is found:
        - set primary src NOT_FOUND?
        - set primary dest DELETED?
        - set secondary src TRANSFERRED?
        - set secondary dest CONFIRMED?
    * should probably do a truth table of primary/secondary src/dest cryovial_store/l_cryovial_retrieval
    * make sure both aliquots are dealt with whatever happens
 * save both primary and secondary to database
 * accept(): if primary aliquot !collected   # expected, ignored (previously skipped), not found (now found?)


chunk: should add/change store records?

 * Emails from Nick/Martin
    * Another minor feature to add to the retrieval assistant when you’re implementing the final part: when you’ve finished retrieving samples from a box, check if it’s empty.  That’s unlikely to be necessary for tasks I’ve created but anyone using Jonathan’s utility to rationalise tanks may end up throwing the old boxes away
    This means we’ll be demonstrating next week using the mirror system, just in case it makes any difference Nick (I'm happy for it to be split I f that makes more sense. Martin)
    * As I mentioned earlier, there's a bit of a problem including boxes in a retrieval if we don't know where they are.  Our current thinking is that we'll ask the user about such boxes when they create a retrieval list and add dummy/incomplete box_store records.  I'm about to add such records for existing jobs.  Please ignore any integrity errors that result
    * This doesn't seem to have caused any problems.  The dummy box_store records have a status of 1 (unconfirmed), rack_cid=0 and slot_position=0.  StoreDAO::findBox() will still return false because it looks for the rack_cid in c_rack_number
 
 * factor out chunk logic?
 * consolidate notes (move to separate windows and compare)
 * select for process samples - 'no chunks' - in testing/q2.sql
 * boxes form doesn't work properly - turn the handle (but not for demo)
 * new requirement is to check at the end of processing each chunk (pt II) if any source boxes are now empty, in which case they might want to discard them rather than put them back

## Misc

 * use DEBUGSTREAM
 * Usual sort order before chunking: destination box and position
 * Usual sort order for each chunk: vessel position, shelf, structure, slot and current position
 * 'move forward'

## Deferred

 * Note: a sample retrieval can include boxes that do not have their current locations recorded in the database.
 * deferred boxes are not saved as such...
why can't I inspect chunk?
look for ??? /newrow
 * port [Plan|Process]Samples to [Plan|Process]Boxes
 * something to bear in mind for processing when it eventually goes live is that the database might not reflect reality perfectly - vials might be missing or swapped etc.
   won't come up till live testing, but worth thinking about at this stage of development
 * demo possible; email martin. make sure there are sufficient example jobs - e.g. "1743 THRIVE boxes - few (EDTA1 & EDTA2)"- 

 * Retrieval Assistant
    * save changes with the option of going back to re-order if necessary.
        * TODO
    * Cryogenics staff can reject a retrieval list if it does not have a retrieval plan

 
 * speed up queries
   * profile?
   * Plan: 
        * load primary aliquot only?
        * save is quite slow
   * Process:  
 * example cryovial retrieval - no chunks
    * there is a plan in the temp table
    * create a new plan, open - ok - plan possible made before db rebuild
 * Session tables: http://community.actian.com/forum/questions-feedback-suggestions/11359-temporary-table.html
DECLARE GLOBAL TEMPORARY TABLE session.temptable AS select * from myview ON COMMIT PRESERVE ROWS WITH NORECOVERY;
 * save changes thread
 * thread "save changes" in plan-/ 
 * sort "aliquot aliquot ascending" is sorting by ID, so primary (-31781) comes after secondary (-31782) shouldn't show aliquot anyway
 * up/down arrows to show column sort
 * Only read the currently selected chunk/aliquot from the database when it is selected ?
 * canned searches - save, delete e.g. site name, vessel pos, structure pos, slot, source box pos
 * define behaviour for manual chunk split in landlocked chunk-

## ???

* Create List
     * Any aliquot type used by a project can be selected as the primary; the secondary cannot match the primary
     * Creating a new sample retrieval list will always create new box name records and (provisionally) allocate cryovial positions
     * A source file may be pre-sorted and may include box numbers and positions as well as the barcode
     * The retrieval list can be re-sorted on screen but this will only affect the destination positions if the source was not pre-sorted
     * Rejected tasks can be modified (re-using existing box name records) or deleted
 * a sample retrieval can include boxes that do not have their current locations recorded
 * As retrieval lists will always specify destination boxes, chunk size can be based on the number of cryovials allocated to each box

## done

in <done.md>

## misc

 when loading chunks, boxes, vials, pay attention to their state (
    // Chunk? Must be calculated? NEW|PART_PROCESSED|COMPLETED
        // NOT_STARTED, INPROGRESS, DONE, REJECTED, DELETED
    // LCDbBoxRetrieval::Status::NEW|PART_FILLED|COLLECTED|NOT_FOUND|DELETED
    // LCDbCryovialRetrieval::Status::EXPECTED|IGNORED|COLLECTED|NOT_FOUND
 ) and colour accordingly

* Source/Current box - standardise name - source better
* how to save stuff? to which tables? when (ie. per row or on exit/save)?
    * save *_retrieval per row, others on exit/signoff
 * Allow user to fill gaps in boxes from secondary aliquot after primary aliquot (partly?) completed
 * though for last chunk, there is no row after
 chunk->getStatus() called in showChunks and sgChunksDrawCell - necessary?
 * notFound() crash
 * show row numbers (e.g. 1,2,3... for each in chunk)
 * chunk progress not shown at first
 * showChunks should fast forward to first unresolved
 * closing window with x exits without save
 * signoff
 * allow going back over skipped
 * changes to status not apparent
    * because currentSample() returns secondary if loaded and secondary is now loaded by default, see above
    * return secondary only if primary is NOT_FOUND?
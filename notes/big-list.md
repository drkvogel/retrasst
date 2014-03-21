## todo

 * make sure both aliquots are dealt with whatever happens
    * save both primary and secondary - secondary aliquots should always be saved if present
    * accept(): 
        * if primary aliquot !collected # expected, ignored, not found (now found?)
    * save secondary as `IGNORED` if not required? primary was 
 * collect empties (all vials "accepted" or "not found") for discard
    * at the end of processing each chunk, if source boxes are now empty
    * unlikely for test tasks but rat tanks may throw old boxes away
    * all source boxes from a reorganisation task should end up empty
    * ask user to confirm that vessel/structure/slot is now empty
    * otherwise box should be referred
 * sign off on exit (complete or not): 
    * Ask the relevant question(s) from the URS
    * check cryo/store old/new params correct for `LCDbCryovialRetrieval`
    * `c_box_retrieval`: set `time_stamp`, `status` = 1 (part-filled)
    * changes to `c_box_retrieval`, `cryovial_store` 
    * update `cryovial_store` (old and new, primary and secondary)
        * Primary, source: 
            * if found: update `removed`, `status=5`
            * else `status=7` 
        * Primary, dest: 
            * if found update `time_stamp`, `status=1`
            * else `status=99` 
        * Secondary, src: 
            * if primary found: clear `retrieval_cid`, `status=1` 
            * else if secondary found update `removed`, `status=5`, 
            * else `status=7`
        * Secondary, dest: 
            * if primary found: `status=99`
            * else if secondary found: update `time_stamp`, `status=1`, 
            * else `status=99`
    * `box_name` (if record): update `time_stamp`, `box_capacity`, `status=1`
    * `c_box_name` (if record): update `time_stamp`, `box_capacity`, `status=1`
 * job finished: 
     * `c_box_retrieval`: set `time_stamp`, `status=2` (collected)
     * `cryovial_store`: as above 
     * `box_name`
        * (if record): update `time_stamp`, `box_capacity`, `status=2`
     * `c_box_name`
        * (if record): update `time_stamp`, `box_capacity`, `status=2` 
     * `c_retrieval_job`
        * update `finish_date`, `status` = 2 
 * use `c_box_name` in some queries to speeds things up (`c_box_name` is copied from `box_name` by a cron job)
    * box changes should be made to both `c_box_name`, and `box_name` (if a `box_name` record is present - not always the case). The cron job should be able to cope with this (up to date `c_box_name` entries already existing)
    * setting the timestamps correctly (to 'now') is crucial 
 * should be 4 cryovial_store recs/sample: src + dest * primary + secondary
    * new `NOT_FOUND` status (ALLOCATED, CONFIRMED, MOVE_EXPECTED, DESTROYED, ANALYSED, TRANSFERRED, DELETED = 99) (no IGNORED status?)
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
    * should probably do a truth table of primary/secondary src/dest `cryovial_store`/`l_cryovial_retrieval`
 * chunk: should add/change store records? (???)
    * there's a bit of a problem including boxes in a retrieval if we don't know where they are. current thinking is ask the user about such boxes when they create a retrieval list and add dummy/incomplete box_store records. I'm about to add such records for existing jobs.
    * dummy `box_store` records
        * `status=1` (unconfirmed), `rack_cid=0`, `slot_position=0`
        * StoreDAO::findBox() will still return false because it looks for the `rack_cid` in `c_rack_number`

## Misc

 * select for process samples in testing/q2.sql
 * use DEBUGSTREAM
 * Usual sort order before chunking: dest box, pos
 * Usual sort order/chunk: vess pos, shelf, structure, slot, curr pos

## Deferred

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
 * showCurrentRow gets triggered twice, no, three times!
        sgChunksClick --> showChunk --> showCurrentRow # just once
                # now not at all
        //btnSkipclick --> skip --> showCurrentRow // not necessary
        btnSkipclick --> skip --> nextRow --> showCurrentRow
        btnSkipclick --> skip --> nextRow --> showChunks --> showChunk --> showCurrentRow
 * consolidate notes (move to separate windows and compare)
 * IGNORED status shouldn't be saved?
 * Insert a record into c_box_retrieval for each box in turn and update c_retrieval_job: set status=in progress (1)
 * "source" and "destination" (boxes) > "old"/"new" or "current"/"future"
 * deferred boxes are not saved as such...
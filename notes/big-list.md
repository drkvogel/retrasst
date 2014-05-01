1086765 ("Use primaries, keep secondaries in reserve")
1086654 ("Primary and secondary in alternate slots")

SaveProgressThread::storeSample()?

### row colours

secondary (expected) should perhaps be green - ie. stronger version of primary colour

colours should be consistent between plan and retrieve

### completing last row doesn't finish chunk

When a non-last (i.e. previously-deferred) row is completed and this is the last row to be completed in the chunk, instead of the chunk finished routine executing, the cursor jumps to the last (already actioned) row. Attempting to action this throws an error, which is correct.

`checkChunkComplete()` is not itself complete, maybe why.

Check completeness or otherwise of chunk with `Chunk::getStatus()`
    
    nextRow()    
    completeChunk()



### "is chunk finished" routine not correct

Doesn't seem to ignore deferred rows, i.e. they are counted as being completed when they should not be.

Deferring a marooned row that is the only unactioned row results in the chunk being "Completed" and turned to the completed colour, whilst showing (e.g.) 11/12 (92%).

`float Chunk::getProgress()` looks OK, i.e. returns 92%, and 
`string Chunk::progressString()` uses that, is OK as well
`int Chunk::getStatus()` was wrong - was counting IGNORED rows as being completed

### deal with jobs containing vials/boxes from multiple projects

    select * from c_retrieval_job where retrieval_cid = 1086786

    project_cid = -659 (HPS, t_ldb3)

`project_cid`: _"project boxes/vials are from (-1: not known; 0: more than one project)"_

has each SampleRow got project_cid stored somewhere? no to all of these...:

    LPDbCryovial
    LPDbCryovialStore
    LCDbCryovialRetrieval
    .

`c_box_name` has it (used in collect samples) - plan samples should use it and record project_cid in SampleRow...

`Retrieval/Box/SampleRow` objects didn't record `project_cid` - now they do. 

The correct star database (`*_ddb`) should be selected to collect details from the correct

    cryovial
    cryovial_store

And `StoreDAO::findBox()`? No, this gets data from `c_slot_allocation`, `c_rack_number`, `c_tank_map`, `c_object_name`, all central databases.

Should use `TfrmRetrievalAssistant::getStorage()` which caches storage locations application-wide (what about when storage locations change? cache should be invalidated e.g. on return to main Retrieval Assistant screen).

### chunk finished

At the end of chunk, check if the chunk is actually finished (no REFERRED vials). If finished:
    * Require user to sign off-
    * update cryo store records
    * calculate if there are any empty boxes
    * create tick list or switch list of boxes, empty/otherwise
    * ask user to comfirm that empty boxes are in fact empty
    * if error, create referred box (INVALID/EXTRA/MISSING CONTENT?) in `c_box_name` and/or `c_slot_allocation`

### allow marking of boxes as invalid (REFERRED) in Collect Empties

When collecting empty boxes as determined by the database, boxes may be found that are not actually empty, or there may boxes that are supposed to have vials in that are actually empty. We proposed that such boxes would be marked with some 'invalid' status and, as this is likely an unusual and tricky situation to deal with, rather than try to make Retrieval Assistant able to deal with such situations, or indeed Referred Boxes, mark them as REFERRED and allow Referred Boxes to pick them up, and flag as invalid in some way so that they can be dealt with manually - and then probably discarded from Referred Boxes. 

As such erroneous boxes have already 'arrived' by this point, `l_box_arrival` is probably not the best place to add them - though Referred Boxes currently only looks at `l_box_arrival` for referred boxes, and that table is added to only by Box Reception via Storage Sync. 

So we proposed adding invalid boxes from Retrieval Assistant to `c_box_name` and/or `c_slot_allocation`, and have Referred Boxes pick up boxes from those tables _as well_.

#### pick up invalid boxes in Referred Boxes

`c_box_name` and `c_slot_allocation` fields together == `l_box_arrival`

> Markdown Extra implements definition lists. Definition lists are made of terms and definitions of these terms, much like in a dictionary. A simple definition list in Markdown Extra is made of a single-line term followed by a colon and the definition for that term.

##### `C_BOX_NAME`

> Box names are copied into this table from each of the projects each night but it also lists boxes shared between projects. Programs modifying this table should apply the same changes to the relevant box_name entry if there is one.

`box_cid`
:   A unique ID for this record
`project_cid` (`c_project`)
:   The project that uses this box (0 = shared by several projects)
`barcode`
:   The ID to be printed on the container (usually a number, often `project code` + `box_cid`)
`box_type_cid` (`c_box_content`)
:   The formation of aliquot types stored in this container
`box_capacity`
:   The number of cryovials that can still be added (0 = full box; -1 = not known)
`external_name`
:   A unique label for this box. Usually `project name` `box type` `box number`
`status`
:   0 = new/empty, 1 = cryovials added, 2 = content confirmed, 3 = ready to store, 4 = stored, 8 = destroyed, 99 = invalid
`time_stamp`
:   When this record was last updated
`process_cid` (`c_audit_trail`)
:   The program run that created or updated this record

##### `C_SLOT_ALLOCATION`

> A record of where each box is likely to be stored. See box_store in the relevant project database for further information if the box contains cryovials from a single project.

`slot_cid`
:   The ID for this record
`rack_cid` (`c_rack_number`)
:   The rack this slot is part of
`slot_position`
:   The position of this slot in that rack
`project_cid` (`c_project`)
:   The project this slot currently belongs to (obsolete?)
`box_cid`
:   The internal ID of the box in this slot
`status`
:   0: slot reserved (by StoreMan), 1: unconfirmed (imported data), 2: removal expected, 3: removed, 5: slot allocated (by Box Reception), 6: confirmed, 7: referred 99: deleted
`retrieval_cid` (`c_retrieval_job`)
:   `status=2`: box will be moved as part of this task; `status=3`: box was moved by that task
`time_stamp` 
:   When this record was updated, i.e. when the box was stored or removed


 * overallStatus() or sampleStatus() method for a sample
    * e.g. status of the primary, or if the primary was not found, and there is a secondary, the status of the secondary?

 * sample, vial, aliquot - settle on maybe two of these words 
    * (sample and aliquot; or sample and vial?) - sample and vial

 * make sure both aliquots are dealt with whatever happens
    * save both primary and secondary (if present)
    * save secondary as `IGNORED` if not required? primary was 

 * collect empties (all vials "accepted" or "not found") for discard
    * at the end of processing each chunk, if source boxes are now empty
    * unlikely for test tasks but rat tanks may throw old boxes away
    * all source boxes from a reorganisation task should end up empty
    * ask user to confirm that vessel/structure/slot is now empty
        * so need to check if v/st/sl is empty as well?
    * otherwise box should be referred

 * sign off on exit (complete or not): 
    * check cryo/store old/new params correct for `LCDbCryovialRetrieval`?
    * `c_box_retrieval`: set `time_stamp`, `status` = 1 (part-filled)
    * `cryovial_store` (old and new, primary and secondary):
        * Primary, source: 
            * if found: update `removed`, `status=5` (TRANSFERRED)
            * else `status=7` (NOT_FOUND)
        * Primary, dest: 
            * if found update `time_stamp`, `status=1` (CONFIRMED)
            * else `status=99` (DELETED)
        * Secondary, src: 
            * if primary found: clear `retrieval_cid`, `status=1` (CONFIRMED) 
            * else if secondary found update `removed`, `status=5`, (TRANSFERRED)
            * else `status=7` (NOT_FOUND)
        * Secondary, dest: 
            * if primary found: `status=99` (DELETED)
            * else if secondary found: update `time_stamp`, `status=1`, (CONFIRMED)
            * else `status=99` (DELETED)
    * `box_name` (if record): update `time_stamp`, `box_capacity`, `status=1` (IN_USE)
    * `c_box_name` (if record): update `time_stamp`, `box_capacity`, `status=1` (IN_USE)

 * job finished: 
     * `cryovial_store`: as above 
     * `c_box_retrieval`: set `time_stamp`, `status=2` (COLLECTED)     
     * `box_name`
        * (if record): update `time_stamp`, `box_capacity`, `status=2` (CONFIRMED)
     * `c_box_name`
        * (if record): update `time_stamp`, `box_capacity`, `status=2` (CONFIRMED)
     * `c_retrieval_job`
        * update `finish_date`, `status=2` (DONE)

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

#### Error inspecting <symbol>

In `LCDbCryoJob::saveRecord()`, couldn't inspect `central`.

Similar to [this](http://qc.embarcadero.com/wc/qcmain.aspx?d=68452)?

Project settings ("Debug configuration - 64-bit Windows platform") say "Disable all optimizations" is true, though.

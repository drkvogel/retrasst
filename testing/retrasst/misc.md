# Retrieval Assistant

## todo

 * signoff
 * allow going back over skipped
 * Insert a record into c_box_retrieval for each box in turn and update c_retrieval_job: set status=in progress (1)
 * Note: a sample retrieval can include boxes that do not have their current locations recorded in the database.

 * Ask the relevant question(s) from the URS when they’re ready to finish and 
 * update cryovial_store (old and new, primary and secondary) when they enter their password to confirm

 * check cryo/store old/new params are correct for LCDbCryovialRetrieval

 * signoff form
 * secondary aliquots should always be saved if present
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
 * currentSample() returns the secondary aliquot if present; now that they're all loaded if available, any changes made to the status of the 'currentSample()' are made to the secondary whether selected or not; result: it looks like nothing has happened (to the primary). This is wrong.
 * changes to status not apparent
    * because currentSample() returns secondary if loaded and secondary is now loaded by default, see above
    * return secondary only if primary is NOT_FOUND?
 * save both primary and secondary to database

## misc

 * Usual sort order before chunking: destination box and position
 * Usual sort order for each chunk: vessel position, shelf, structure, slot and current position

## done

 * reported chunk sizes look wrong (not 200)
 * changes to l_cryovial_retrieval should be saved on the fly
 * Tick each cryovial off in the retrieval plan as you go along.
 * does it load previously saved/altered rows?
 * should use saveRecord for LCDbBoxRetrieval
 * fast forward to first unactioned sample...
    * only works for 1st chunk
    * what about deferred samples?
 * save progress
 * load in collected/not found/ignored samples as well to return to in progress chunk-
* accept button should be next to text box
* "Defer" would be a better name than "Skip" for passing over a sample
* does LCDbCryovialRetrieval::saveRecord() insert work? - it does now
not saving barcode, so collect query doesn't join
saveRecord()
LCDbCryovialRetrieval::saveRecord()
1st chunk red
could find out there's no samples really quite quickly without doing whole query... 
 * The option to exit the process saving progress should be offered, with an “are you sure?” message in case of accidental selection (REQ 8.3.12).
 * Allow selection of retrieval list that are either in progress or new (REQ 8.3.1). 
 * For new lists give the option to divide the list (REQ 8.3.2), 
 * if “Yes” is selected allow user to define division points in list (REQ 8.3.3) and present each section for retrieval in turn with the option to re-order each sub-section by sample ID, primary aliquot location, secondary location or destination (REQ 8.3.4). 
 * If no is selected allow whole list to be ordered by sample ID, primary aliquot location, secondary location or destination (REQ 8.3.5). 
 * Following this division and re-ordering the user should be asked to save the changes prior to commencing retrieval with the option going back to re-order if necessary (REQ 8.3.6). 
 * For in progress lists these steps should be skipped (REQ 8.3.7).
 * Work through list or sub-section by giving the storage location and sample ID of each sample on the list in the order saved above (REQ 8.3.8) 
 * as each sample is retrieved it’s barcode should be scanned, if the scanned barcode matches that on the list the destination location should be displayed and the next ID/location should be displayed (REQ 8.3.9) 
 * if the IDs do not match a warning should be displayed and re-entry of the barcode required (REQ 8.3.10). 
 * When working through the list the previous five successfully entered ID’s should always be visible (REQ 8.3.11). 
  * OnClick: show details
 * process() # not used, remove



## misc

questions for meeting
 observation - still going on?
 slot number?
 bugzilla bugs (gencomms, 
 nova
 
 * accept(): if primary aliquot !collected   # expected, ignored (previously skipped), not found (now found?)
 * http://www.thefreedictionary.com/aliquot: a sample can be divided into aliquot parts
http://www.soulseekqt.net/SoulseekQT/Linux/SoulseekQt-2013-11-6.tgz 

    int getPosition() const { return position; }
    int getBoxID() const { return boxID; }

LCDbBoxRetrieval
LCDbBoxRetrievals
LCDbCryovialRetrieval
LCDbCryovialRetrievals

    -12896345|SHARP urine Nov 2013          |SHARP urine retrieval for Richard Haynes            No samples found
      -636363|Example box retrieval         |A THRIVE box retrieval for Chris                    None found but no message, sgvials is all blue
      -363636|Example cryovial retrieval    |A THRIVE cryovial retrieval for Chris               
       -14059|THRIVE Lp(a) Sep 2013         |THRIVE samples for a combined HPS/THRIVE retrieval  No samples found
       -14058|HPS Lp(a) Sep 2013            |HPS samples for a combined HPS/THRIVE retrieval     No samples found
       -14056|STICS troponin analysis       |14 boxes for a STICS serum_1 troponin retrieval     No samples found
        -4008|SHARP base&FU Oct 2013        |SHARP EDTA_2 retrieval - baseline and follow-up     No samples found
        -4004|SHARP baseline Oct 2013       |SHARP EDTA_2 retrieval - baseline only              No samples found
        -1015|THRIVE Lp(a) baseline         |Initial THRIVE Lp(a) analysis                       Loads ~4000 samples OK

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

 * python (st2 plugin)
 * javascript (in browser)
 * PHP
 * regex
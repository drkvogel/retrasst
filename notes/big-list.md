
---new stuff from CVS:---
LIMSDatabase::getProjectDb()
LCDbProject::isInCurrentSystem()
not new, but...
Tank::getVessel()? // Inventory.h
TfrmNewJob::createJob
NewTank methods
SampleEntry
StoreDAO::findBox, addToRetrieval
---
a sample retrieval can include boxes that do not have their current locations recorded
misc
    find destination boxes 1st
    restructure DataRow etc to be more like/same as nick's GridEntry
sorting
    Nick's sorter in SampleEntry.cpp/h
    check sorting works properly 
    sort racks using rack position instead of name?
    "P.P.S. I ended up using the c_rack_number.position to sort the structures.  Different layouts follow different naming conventions"
find number of boxes
    current box
    destination box?
    what do we mean by 'size' of a chunk - number of boxes
chunks
    chunks fill in situ if manual
    select row - 'chunk here'?
    auto chunk - populate chunks only when ok is pressed
    chunks "start, end" - use real values
    section - Cannot Be Changed 
    start
        1st chunk: CBC
        Others:
    end:
        1st chunk: 
    size:
        calculate end (max if not valid)

    specify in samples or boxes?
        
    highlight current sample/box
    double-click sample/box
        current chunks upper boundary is here (check valid)
        next chunk (if present) lower boundary is here
find destination boxes - faster method using sequence?
    nick's method - 
        build map of boxes first, groups of many samples will map to each box
            TfrmRetrieveMain::btnLocateClick
                if(dao.findBox())    
  
## Retrieval Assistant

### Background

#### Create List

 * Any aliquot type used by a project can be selected as the primary; the secondary cannot match the primary
 * Creating a new sample retrieval list will always create new box name records and (provisionally) allocate cryovial positions
 * A source file may be pre-sorted and may include box numbers and positions as well as the barcode
 * The retrieval list can be re-sorted on screen but this will only affect the destination positions if the source was not pre-sorted
 * Columns should be displayed in Russian doll order, left to right: site, location, vessel, shelf, structure, slot, box, position
 * Rejected tasks can be modified (re-using existing box name records) or deleted
 * Finding where all the cryovials are can take a long time; users must wait for the information to be available 

### Retrieval Assistant

 * General
    * Box Retrieval
        * Select * from c_box_retrieval b order by b.section, b.rj_box_cid
        
    * Sample (cryovial) Retrieval
        * Select * from c_box_retrieval b, l_cryovial_retrieval c where b.rj_box_cid = c.rj_box_cid order by b.section, c.position  

 * As retrieval lists will always specify destination boxes, chunk size can be based on the number of cryovials allocated to each box
 * Columns should be displayed in Russian doll order, left to right: site, location, vessel, shelf, structure, slot, box, position
 * If required, the secondary aliquots appear at the end of the retrieval plan for each chunk – they may never be needed
    * TODO
 * a panel displaying sort order for both Create List and Retrieval Assistant
    * Kind of
 * sort by location
    * Via column clicks and/or sorter panel
 * location should include site+position+name+layout, as it does in StoreMan’s storage browser.-
    * OK
 * chunking -  Allow the user to divide up the list
    * TODO
 * save changes with the option of going back to re-order if necessary.
    * TODO
 * Cryogenics staff can reject a retrieval list if it does not have a retrieval plan
    * TODO

---to sort---

---done---
invalid pointer operation on click retrieval assistant open 1st time only- nick has sorted
  diff btwn specs in notes/spec-comparison
    sample query-
        gets src and dest boxes in one projectdb-only query - but not storage details, these can be looked up after and cached in a map for efficiency
    a sample retrieval can include boxes that do not have their current locations recorded
    export/edit/import? maybe
    option of rejecting whole task
ukes reminder-
xe2-
sits:vision-
StoreUtil class -> namespace? hmm
sorter combos - populate/implement-/
populating LCDbBoxStore * store_record by LQuery contructor is inefficient?:
    LCDbBoxStore::copyFields()
        LQuery::readInt() // etc
            ROSETTA &LQuery::getRecord( )
            // actually seems to just pull the correct fields out of the current cursor, without running a new query, so ok
retrasst objectives for next meeting >> workblog >> stf eml
get rid of maxrows maximum-
show rows - show all by default
retrasst emails -> folder
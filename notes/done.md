## Done

 * auto-chunk prompt - silly because list might not be sorted at correctly at this point. remind to hit auto-chunk or manually chunk by double-clicking on rows
 * get rid of add, del, +/- - auto chunk and dbl-click is enough flexibility
 * fixed row count error from clicking pos too much- Enabled = false
 * longer descriptions for sorters (e.g. several called pos)- 
 * auto-chunk: get dest box size from job- is it getting the dest size or src size?- dest now
 * crash on "do you want to chunk" now, as list not loaded yet? list is loaded, but no chunks yet. fixed.
 * is sorting chunks working properly?
    sorting chunk sorts everything, not the chunk. this is wrong.
    not quite, sorting a chunk without affecting rest sometimes, but not others...
    because didn't change sort_dsc in the same way
    but that produces an access violation... can you do rend()+x? no, of course not, it's rbegin()+x you want
    but that doesn't seem to do anything!
    nope, they're still messin' with one another.. sorter still assigned to previously selected chunk? currentChunk() not right?
    the column toggle value would still be in the Col object no matter what chunk is selected
    it is, I think, sorting the last shown chunk on first click
    and the sort was off by one - including one from previous chunk and missing last of current chunk, so I added 1 to start and end - should I have?
 * delete current chunk (e.g. from middle of chunk list)
 * existing chunk +/-
 * storage sync not worth attempting to rebuild-
 * bodge should be ok and not something I have to worry about-
 * other than search/replacing my code to use tube_position rather than cryovial position-
 
// dot notation: vec.begin() also seems to work - how? it wasn't being compiled because dead code in a template// http://stackoverflow.com/questions/456713/why-do-i-get-unresolved-external-symbol-errors-when-using-templates
//export
//inline
// attempts to move definition of Chunk() to .cpp in order to try to set breakpoints which disappear at runtime when
// I set them in the header. none worked. it seems templated methods must be defined in the header.
// why my breakpoints, which I need to find out why the program is crashing, are disappearing, I do not know.
// I'm going home.
    // setEnd(end); // setStart(st); // moved out to caller cause can't set breakpoints in template
    // http://stackoverflow.com/questions/1568091/why-use-getters-and-setters

storage sync build

ss:
vcl not found!
[BCC32 Warning] W8123 Path 'c:\program files (x86)\embarcadero\rad studio\9.0\include\vcl' not found - path ignored in option '-I'
$(BDSINCLUDE)\windows\vcl;..\LabsForms\;..\CoreUtilities\;..\LabsLeasing\;..\LabsDAO\;..\LabsObjects\;..\LabsDAO;..\CoreUtilities;..\LabsObjects;..\LabsLeasing;..\LabsForms
$(BDS)\lib\debug;$(BDS)\lib\psdk;..\LabsForms\;..\CoreUtilities\;..\LabsLeasing\;..\LabsDAO\;..\LabsObjects\;$(BDS)\lib\obj;$(BDS)\lib;..\LabsDAO;..\CoreUtilities;..\LabsObjects;..\LabsLeasing;..\LabsForms
lcdbaudittrail.cpp both at head 1.46, but SS one fails on got wchar_t
_TCHAR maps to <blank> - fixed
pulled from cvs
[BCC32 Error] LSDbBox.cpp(18): E2283 Use . or -> to call 'LCDbAuditTrail::getProcessID()'
LCDbTest.h 1.46 ("Convertion (sic) to XE4 code")

what did I pull out of CVS?

retrasst:
$(BDSINCLUDE)\windows\vcl;..\..\jonathanpr\RetrievalLists\;..\..\paulst\;..\LabsLeasing\;..\CoreUtilities\;..\LabsDAO\;..\LabsObjects\;..\LabsForms\;$(II_SYSTEM)\ingres\files\;$(BDSINCLUDE)

_TCHAR maps to wchar_t
LCDbTest.h 1.47
find destination boxes - faster method using sequence?
    nick's method - 
        build map of boxes first, groups of many samples will map to each box
            TfrmRetrieveMain::btnLocateClick
                if(dao.findBox())  
 * location should include site+position+name+layout, as it does in StoreMan’s storage browser.-
invalid pointer operation on click retrieval assistant open 1st time only- nick has sorted
diff btwn specs in notes/spec-comparison
sample query- gets src and dest boxes in one projectdb-only query - but not storage details, these can be looked up after and cached in a map for efficiency
a sample retrieval can include boxes that do not have their current locations recorded
export/edit/import? maybe
option of rejecting whole task
ukes reminder-
xe2-
sits:vision-
StoreUtil class -> namespace? hmm
sorter combos - populate/implement-/
populating LCDbBoxStore * store_record by LQuery contructor is inefficient?: no
LCDbBoxStore::copyFields(); LQuery::readInt() // etc ROSETTA &LQuery::getRecord( ) // pulls the fields out of the current cursor, without new query, so ok
retrasst objectives for next meeting >> workblog >> stf eml
get rid of maxrows maximum-
show rows - show all by default
retrasst emails -> folder
find destination boxes 1st-
restructure DataRow etc to be more like/same as nick's GridEntry-
Columns should be displayed in Russian doll order, left to right: site, location, vessel, shelf, structure, slot, box, position
---todo---
whsmiths food
contact ucu about ay anyway
---dealt with---
splitter for memodebug-
job title/description on chunking form-
are we finding the destination boxes correctly? looks like it 
---
---to sort---
Looking at your Sorter again, I still couldn’t get it to do what I wanted it to, e.g. sorting by Vessel then rack left it in rack order, rather than Vessel+rack.  I have therefore created my own Sorter (in SampleEntry.cpp) but I’d be happy to merge it with yours – it would make more sense if all the screens worked in the same way
sorting
    sorters
    Nick's sorter in SampleEntry.cpp/h
    check sorting works properly 
    sort racks using rack position instead of name?
    P.P.S. I ended up using the c_rack_number.position to sort the structures.  Different layouts follow different naming conventions
find number of boxes
    current box
    destination box?
    what do we mean by 'size' of a chunk - number of boxes
chunks
    chunk/section number should begin at 1, not 0, as 0 means 'no chunk'
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
    specify in samples or boxes depending on job
    highlight current sample/box
    double-click sample/box
        current chunks upper boundary is here (check valid)
        next chunk (if present) lower boundary is here
* Columns should be displayed in Russian doll order, left to right: site, location, vessel, shelf, structure, slot, box, position
 * Finding where all the cryovials are can take a long time; users must wait for the information to be available 
 * If required, the secondary aliquots appear at the end of the retrieval plan for each chunk – they may never be needed
    * TODO
 * a panel displaying sort order for both Create List and Retrieval Assistant
    * Kind of
 * sort by location
    * Via column clicks and/or sorter panel
 * chunking -  Allow the user to divide up the list
    * TODO
* Box Retrieval
    * Select * from c_box_retrieval b order by b.section, b.rj_box_cid
* Sample (cryovial) Retrieval
    * Select * from c_box_retrieval b, l_cryovial_retrieval c where b.rj_box_cid = c.rj_box_cid order by b.section, c.position  

sorter.txt


sql to delete plan
still the mystery of the missing list:
 list I made with create list has no rows
 select * from c_retrieval_job
 retrieval_cid 1223

select * from cryovial_store where retrieval_cid = 1223
gives one row and
select * box_store where retrieval_cid = 1223
gives none on t_ldb20
then, if I run the query on the ddb, I get
    E_LQ002E The 'rollback' query has been issued outside of a DBMS session (t_ldb20_ddb)
and then when I go back to the project database:
    E_LQ002E The 'prepare' query has been issued outside of a DBMS session (t_ldb20)
selecting from -1015 (select * from cryovial_store where retrieval_cid = -1015) gives lots of rows.

click on chunk - show wait cursor while loading-
sample grid - shelf and vessel wrong way round-dn
in fact, storage details should come after src box - not done, was going to get my coldefs and sorters working first and re-arrange columns like that.-done
copyLocation
    position vs rack_pos vs tank_pos?
        src_box_pos is dupe of store_record->getPosition()
"An empty retrieval list" - null chunk!
double-clicking a sample/box should mark the end of a chunk and create a new one
i started the coldef thing because I put the aliquot column in and it threw all the sorters out
sorters are not on correct columns; ColDef class is supposed to prevent this
'sorting' a column where values are largely the same (e.g. dest box "Example retrieval box 9") results in almost random sorting
things are 
            
# Sorting out the Sorter

   Test(std::vector<T *> & v) : vec(v) {}   
   std::vector<SampleRow *>    vials;   
   Test<SampleRow *> test(vials);
E2285 Could not find a match for 'Test<SampleRow *>::Test(vector<SampleRow *,allocator<SampleRow *> >)'
use typedef? use pointer to vector instead of reference
now just passing function pointer to ColDef constructor is not right:
E2034 Cannot convert 'bool(const SampleRow *,const SampleRow *)' to 'bool (*)(SampleRow * const *,SampleRow * const *)'
notice ** double pointers - ah, because Sorter was specialized with SampleRow, and ColDef is being specialized with SampleRow *    
    
# Problems
 * asc and desc funcs both need to be declared - couldn't one be a reverse of the other?
   * already removed from class spec - implementations in SampleRow now redundant-taken out
 * why are the sort funcs implemented in SampleRow anyway?
 * using enum as parameter to Sorter::sort rather than calling sort_asc/desc/toggle directly seems unnecessary
 * separate Sorter array means it can get out of sync with column definitions - group together in (e.g.) ColDef?
   * does Sorter being a template make this a problem? the type T is e.g. SampleRow for all columns
must be thread-related? e.g. sample->store_record->getBoxID()?
non-threaded version seems ok... something in worker thread must be doing something gui-related... one of the classes descended from a VCL class?
void __fastcall LCDbCryoJobs::Renewal(TObject *)? timer in main thread though so should be ok
what if I take the synchronized updateStatus calls out?
  looked promising, but still crashing
be pragmatic, it seems to work non-threaded.
still much faster when locations are found, shouldn't be - is the query messing things up?
no, still crashes on notAThread
    on second load of same job <-- code smell
    vials not being reset/deleted?
    some other change I've made recently - follow it through from the start   
    unitialized map? clear?
    it was because the map was static <---------!!!

take out dupes:
int src_box_pos             s1.cryovial_position as source_pos // dupe
int src_box_id,             cryovial_store.box_cid // dupe
    
sort out SampleRow:
Member                      Constructor     Database

                            SampleRow(
                             LPDbCryovialStore * store_rec[
                             LPDbID         cryovial_store.cryovial_id
                             LDbNoteCount   cryovial_store.Note_Exists
                             retrievalID    cryovial_store.retrieval_cid
                             boxID          cryovial_store.box_cid
                             status         cryovial_store.status
                             position       cryovial_store.cryovial_position
                             cryovialID],   cryovial_store.cryovial_id
                             
string cryovial_barcode     barcode,        cryovial_barcode
string aliquot_type_name    aliquot,        aliquot_type_cid -> name
string src_box_name         src_name,       box_name.external_name
int dest_box_id             dest_id,        s2.box_cid
string dest_box_name        dest_name,      b2.external_name
int dest_box_pos            dest_pos        cryovial_position as dest_pos  # pos of cryovial in box/sample rack

string site_name            site,           c_tank_map.location_cid -> name
int vessel_pos              tank_pos,       c_tank_map.position as tank_pos? # current pos of vessel at site
string vessel_name          vessel,         c_tank_map.storage_cid -> name
int shelf_number            shelf,          c_tank_map.shelf_number # current pos of population within vessel (0: single population)                                
int structure_pos           rack_pos,       c_rack_number.position as rack_pos # pos of structure within population (in a vessel?) 1st structure in 1st section is at pos 1
int structure_name          rack_name,      c_rack_number.rack_cid -> name
int box_position           slot)           box_store.slot_position # pos of box/rack in structure    
    
is it initializing the ColDefs in frmSample() that's corrupting memory? -no
I have your new card so please pop in with the old one when you’re next passing.-

            // why can't derived members be initialised in constructor's initialization list?
            // because they can't - but you can delegate to the base class' constructor
            // http://stackoverflow.com/questions/17196495/initialise-protected-data-members-from-derived-class-constructor
1st toggle sort seems to be descending-not any more
sort by shelf not working? mismatch, sorted
debuglog which col is being sorted-
drawcell border round selected chunk-
chunks: display 1st and last sample id/boxname-
dblclick: highlighted chunk is second-to last, not last-
addchunk: new start and end are wrong (-ve) - were uninitialised
sorting works again! OMG! sorting by shelf doesn't seem to do anything it is quite slow though...
show log if in debug-
job -1015 THRIVE Lp(a) baseline (496 boxes, 3972 cryovials)
 select count(*) from c_box_retrieval where retrieval_cid = -1015: 496 boxes
 select count(*) from l_cryovial_retrieval: 3972 cryovials
autochunk: thread/wait pointer
process: doesn't load chunks/rows
autochunk: goes over last vial
chunks: put in db-
vials: select 1st row/scroll to top when new chunk created
chunks: sizes are wrong - 1 too high
chunks: can go over end of vials - check
can only double click to split chunk on last chunk?
sorting slow
496 boxes
3972 cryovials
 * Chunks: 
    * what is a chunk? it's a bit of the whole list. If The Whole List is an object, we shouldn't duplicate it in any way. either:
        * move all the contents of the initial list into chunks, or 
        * keep the list, but chunks context indexes into it, and do partial_sorts of its contents.
            * then again, the StringGridWrapper should be shy - just display and sort sth, without knowing whether that list is a part or a whole
 * StringGridWrapper
    * Have implemented the StringGridWrapper, but sorting is no longer working. Must rembember that at all times, even if the chunk is a default chunk containing all rows, we should be sorting a chunk and not the underlying vector of rows.
    * That means the 'rows' of the StringGridWrapper should be set to be the rows of whichever chunk is currently selected?
    * At which point (which routine) should we do this? Something smells wrong about the current way chunks are handled.
    * Pragmatic - forget sorting for now, fire some tracer bullets - get some visual stuff up and database inserts going
sample - can be split into cryovials (aliquots)

### Cultural learnings of 2013-09-28

 * Templated code is not compiled until it is instantiated - i.e. if there is part of your template that is not used, it will not throw a compile error even if bad syntax
 * Debug symbols do not seem to be generated for templated code, meaning breakpoints cannot be set. I don't know if this can be changed in the options.
 * Windows' API debug function is `OutputDebugString()` and is captured by the Event Log tab in C++ Builder
 * Use it like this: `OutputDebugString(L"string")` or like this: `wstringstream oss; oss<<__FUNC__<<"text"<<var; OutputDebugString(oss.str().c_str());`

 * Chunk list should include a second row for each chunk if there is a secondary aliquot (second part of each chunk?)
 * allow selection of secondary aliquots 
 * still not showing 2ndary aliquot    
 * use 2ndary button - remove
 * factor out storage query - getStorage?
    * cache should be wiped at certain points to avoid errors?
 * rename files: Retrasst[Plan|Process][Samples|Boxes]
 * query is v slow - e.g. 1m30s for aliquot_type_cid != Primary; -- load primary and secondary together
 * load 2ndaries seems to work ok
 * save 2ndaries in plan
 * why is the map of box_id to rj_box_id per chunk?
    within each chunk, all boxes containing the vials in that chunk will have a plan entry (rj_box_cid)
    each cryovial is linked to __a box in a particular chunk__ via rj_box_cid, which tells us which chunk the vial is in.
    thus the map being per chunk rather than global is what defines chunks - ie, definitely keep it that way.
    the problem now is how to keep this behaviour and factor out code in order to save both primary and secondary aliquots.
    by the same logic as above, boxes of secondary aliquots must be given unique-per chunk entries also.
    ok, so the same map can be used for both.
  struct Saver to re-use code for 2ndary ali - seems to be saving both aliquots.
  are they read in properly in process? they are read in (hurrah!) but both are displayed.
   * 2nd ali gets overwritten on accept
      accept()
        sample = currentSample() # = currentChunk()->getCurrentRow()->rowAt(current); return (NULL != sample->secondary) ? sample->secondary:sample;
 * Is there a class/method for looking up aliquot type names from an aliquot ID? there is now. My big query for loading the retrieval plan crashes out on real-world size plans (e.g. 4000 samples rather than 28). Joining on c_object_name as well was too much
 * Don’t allow people to save a plan with chunks of more than 1001 samples for sample retrieval/disposal/reorganisation
 * Only display primary aliquots.  
 * add 1 chunk - must add to last current chunk, not in middle
 * factor out select_stats into Util-
 * click on chunk doesn't work now - stays on current chunk-
 * auto-select last chunk-
 * Show aliquot type(s) but not as a separate column
 * Chunk list should show destination boxes rather than source-
 * Columns (left to right): cryovial barcode, site, vessel position, vessel name, shelf (if necessary), structure name, slot, current box, current position, destination box, destination position
 * sgvialsfixedclick debuglog: show sorter name
 * sortChunk(currentChunk(), ACol, Sorter<SampleRow *>::TOGGLE);
 * curr pos sorting doing something funny, dest pos sorting not doing anything
     because adding 'aliquot' column skewed everything - as there is no combined column definition (ColDef?) class,
     sorter funcs can get out of sync with column names
     does it need an enum at all? if an array of objects was initialized, numcols would be the size of the array
 * storage (site, pos, shelf, vessel, structure, slot) of which box? current or destination? both?
     does it matter for setting up the chunks, or which matters more?
     in the retrieval, will want to find storage of both?
 * fix comboSorter - broken window
  * OnClick: show details
 * process() # not used, remove
 * currentSample() returns the secondary aliquot if present; now that they're all loaded if available, any changes made to the status of the 'currentSample()' are made to the secondary whether selected or not; result: it looks like nothing has happened (to the primary). This is wrong.
 * changes to status not apparent
    * because currentSample() returns secondary if loaded and secondary is now loaded by default, see above
    * return secondary only if primary is NOT_FOUND?
 * save both primary and secondary to database
 * scroll to show at least 5 next samples - ScrollBy doesn't seem to work! but worked out a bodge
 * show storage location
 * show sample ID (barcode)?
 * if identical barcode entered into signoff box
    * mark signed off
    * refresh - different colour for signed off    
* testing would be a lot less painful if the plans loaded quicker
 * currentSample() returns the secondary aliquot if present; now that they're all loaded if available, any changes made to the status of the 'currentSample()' are made to the secondary whether selected or not; result: it looks like nothing has happened (to the primary). This is wrong.
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
last chunk crash on statusString
has to iter thru all samples to get status - off the end?
currentRowIdx is now the absolute row index rather than relative
at the start of the second chunk, progress is shown as 91%, 216/237 - 216 is the real position of the first row
reverse changes: end is last row, not one after
in collect, chunk sizes are fubar: 215, 237, 229, 252, 37
    select count(*) from c_box_retrieval br, l_cryovial_retrieval cr where br.rj_box_cid = cr.rj_box_cid and br.section = 1/2/3/4/5
        shows correct(?) sizes per chunk: 400 (200 of both aliquots: -31782,1)
    select count(*) from c_box_retrieval br, l_cryovial_retrieval cr where br.rj_box_cid = cr.rj_box_cid and br.section = 3 and cr.aliquot_type_cid = -31781
        there is the correct (200) number of each aliquot in each chunk of the plan - something is going awry on loading the plan into memory
    the join is probably including extra store records created on previous saves - no
    the plan looks ok 1674 = 2 * 837 rows, chunks of 200
    run /testing/retrasst/is-query-correct/go.sh again -> out.txt -> out.xlsx
    count(*) of big query gives 1674! the query is ok
    rowCount = 217  when rowCountTemp = 400
    rowCount is wrong
    2ndary aliquot code not right
        assumes aliquots alternate, sometimes they are grouped together
    or sort order not right
        only two types currently (select distinct(aliquot_type_cid) from l_cryovial_retrieval)
        some secondaries must get counted as primaries
        yes, sort by lcr_position, not source_pos - lcr_position is the order they are put in the plan, source_pos *may* be the same order sometimes, but not always, depending on how you sort the plan - previously I had been saving the plan as-is without sorting, and source_pos worked, but recently have been sorting by destination box and position - hence recent funny behaviour. sort by lcr_position and now it's *almost* right - 1st chunk 1 too big, last 1 too small...

`TfrmRetrievalAssistant::getStorage()` calls `StoreDAO::findBox()` but maintains a cache map.

### not found source locations

`retrieval_cid` 1086786 "HPS samples into mixed boxes"

All source locations are 'not found'. 

`project_cid` is -659 (`dev_hps`, `t_ldb3`)

WHY???

 * 1st, `LoadVialsJobThread::load()` uses its own `StoreDAO dao`
     - `LoadPlanThread::Execute()` uses `frmRetrievalAssistant->getStorage()`
     - OK, now `LoadVialsJobThread::load()` uses `getStorage()` as well - at least debugging in same place (`StoreDAO::findBox()`)
 
 LoadVialsJobThread::load()
    frmRetrievalAssistant->getStorage(sample);
    TfrmRetrievalAssistant::getStorage(SampleRow * sample) (sample->project_cid -659, (sample->store_record)->boxID -53855)
        storageCache.find(sample->store_record->getBoxID())
        dao.findBox(sample->store_record->getBoxID(), LCDbProjects::getCurrentID(), result))
            `box_id`:  -53855 
            `proj_id`: -659
        doesn't find storage details.

It's in `c_box_name`: 

    select * from c_box_name where box_cid = -53855

and in `t_ldb3`: 

    select * from box_name where box_cid = -53855

but no storage details - is this correct?

(sample->store_record)->boxID -53857 found though, in same job - where did this come from?

##### job 1086789 "Add in the samples for SEARCH"

project -213

(sample->store_record)->boxID -625584 found in `c_box_name`, storage details found

(sample->store_record)->boxID -40191  found in `c_box_name`  storage details not found
    -40191 found in `c_slot_allocation`
    joins with `c_rack_number` on `rack_cid`
    and `c_tank_map` on `tank_cid`
    and `c_object_name` on `m.storage_cid`

it's (at least) `m.status = 0` that's doing it here - status is instead 1.

`c_tank_map.status` [0: expected; 1: in use; 5: off line; 99: mapping deleted]

`c_tank_map.status`  is 5 (off line) for the other one (-53855)

##### 2014-04-24 Nick changed findBox 

so that:

    for( const ROSETTA & record : cq ) {
        // fix Chris's bug: there should be one mapping and that should be in use
        // if there is one that's off-line, use it if there isn't anything better
        if( !found || result.getInt( "status" ) == LCDbTankMap::OFFLINE ) {
            result = record;
            found = true;
        } else if( record.getInt( "status" ) != LCDbTankMap::OFFLINE ) {
            return false;
        }
    }

which I don't quite understand atm, but it seems to work, i.e. storage locations are returned normally for locations for which `c_tank_map.status` is `RECORD_IN_USE`, but also if it is `OFFLINE` for whatever reason and the only one available - this status should be reported to the user though

Are secondaries missing? Source and destination positions go 1, 3, 5 etc.

### <ProjectExtensions> error

When hovering over a symbol with "Tooltip symbol insight" turned on (Tools -> Options -> Editor Options -> Code Insight), get an error:

> "The <ProjectExtensions> element occurs more than once"

Supposedly because of a [bug, not yet fixed by Embarcadero, in the IDE](http://codeverge.com/embarcadero.cppbuilder.ide/-xe2-update-2-fatal-error-the-proj/1072918), whereby the `<ProjectExtensions>` element does indeed occur more than once in the `.cbproj` project file. Except that it doesn't:

    $ grep ProjectExtensions *
    Storeman.cbproj:    <ProjectExtensions>
    Storeman.cbproj:    </ProjectExtensions>

Ah well, turn it off then.

### vlabdev

2014-04-08 Set `vnode_vlab` to point to `vlabdev`. `vlab` should be used for testing, as queries are slower on this VM than on `vlabdev`, probably because it has less memory.

### error saving plan

In `LCDbCryoJob::saveRecord()`, Nick had made changes so that query parameters `sdt`, `cdt`, `fdt`, `myid` were renamed `sd`, `cd`, `fd`, `id` - but hadn't updated the update string.
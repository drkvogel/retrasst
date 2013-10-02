update c_operator set status = 1, valid_to = date('now') + date('2 years'), password_md5 = '' where operator_cid = -31438

# Retrieval Assistant

LPDbCryovials::getAliquotTypes // useful?

# StoreMan Retrieval Assistant

###  8. Sample retrieval screen

#### 8.1 Previous lists

Allow previous retrieval lists to be viewed, first list names (REQ 8.1.1) then list of boxes samples retrieved into with current storage location (REQ 8.1.2).

#### 8.2 Create list _(Nick)_

Allow import of list of sample ID’s, cryovial ID’s or box names from either a text file (REQ 8.2.1) or from specimen table (REQ 8.2.2) and allow selection of primary aliquot choice and secondary aliquot choice (REQ 8.2.3). Generate list of sample ID’s with location of primary aliquot choice and secondary aliquot choice in separate columns (REQ 8.2.4), or where appropriate a list of box numbers (REQ 8.2.5) and allow list to be ordered alphanumerically by any column (REQ 8.2.6). Ask user if samples are to be retrieved to new boxes (REQ 8.2.7), if user specifies yes then allow a destination location to be mapped to each sample barcode (REQ 8.2.8), this would be a new box for the sample to go into once retrieved, if user responds no then use existing box numbers and locations and assume the samples will be kept in the original location (REQ 8.2.9). Where no destination is specified new boxes should be generated unless whole boxes are being retrieved (REQ 8.2.10) and they should be filled sequentially (REQ 8.2.11). After creating a list the following options should be given, Save or Discard (REQ 8.2.12). If Save is selected the user should be asked to name the list prior to it being saved (REQ 8.2.13), if Discard is selected an “Are you sure?” message should be displayed with the “Yes” response returning to the sample retrieval screen (REQ 8.2.14) and the “No” response returning to Save/Discard option (REQ 8.2.15). Created lists should be displayed on the status indicator on the main screen (REQ 8.2.16).

#### 8.3	Retrieval assistant

Allow selection of retrieval list that are either in progress or new (REQ 8.3.1). For new lists give the option to divide the list (REQ 8.3.2),
if “Yes” is selected allow user to define division points in list (REQ 8.3.3) and present each section for retrieval in turn with the option to re-order each sub-section by sample ID, primary aliquot location, secondary location or destination (REQ 8.3.4). If no is selected allow whole list to be ordered by sample ID, primary aliquot location, secondary location or destination (REQ 8.3.5). Following this division and re-ordering the user should be asked to save the changes prior to commencing retrieval with the option going back to re-order if necessary (REQ 8.3.6). For in progress lists these steps should be skipped (REQ 8.3.7). Work through list or sub-section by giving the storage location and sample ID of each sample on the list in the order saved above (REQ 8.3.8); as each sample is retrieved it’s barcode should be scanned, if the scanned barcode matches that on the list the destination location should be displayed and the next ID/location should be displayed (REQ 8.3.9) if the ID’s do not match a warning should be displayed and re-entry of the barcode required (REQ 8.3.10). When working through the list the previous five successfully entered ID’s should always be visible (REQ 8.3.11). The option to exit the process saving progress should be offered, with an “are you sure?” message in case of accidental selection (REQ 8.3.12).

## More detail

### Create retrieval work list, requirements 8.3.1 – 8.3.7

When the retrieval assistant is given a new job, it allows the user to divide the work into sections:

#### Box retrieval

    c_retrieval_job.status = new job (0); job type = box retrieval (2) or disposal (3)

Find where the boxes are supposed to be:

    Select … from box_name n, box_store bs, c_rack_number r, c_tank_map m
    where n.box_cid=bs.box_cid and bs.rack_cid=r.rack_cid and r.tank_cid=m.tank_cid
    and bs.retrieval_cid = jobID;

List the name, current structure and expected location of each box.  The location should include site+location+name+layout, as it does in StoreMan’s storage browser

Allow the user to divide up the list (this may not be necessary; see below for more details).  After division, show each section in turn and allow user to sort by location, otherwise show the whole list and allow them to sort that.

Ask user to save changes with the option of going back to re-order if necessary.

Insert an entry into `c_box_retrieval` for each box, recording the section it is in and its position within that section.  

	update c_retrieval_job set status=in progress (1)

#### Sample retrieval

    c_retrieval_job.status = new job (0); job type = sample retrieval (4) or disposal (5)
    
Find the cryovials to be retrieved:

    Select … from cryovial c, cryovial_store cs, box_name n, box_store bs, c_rack_number r, c_tank_map m
    where c.cryovial_id=cs.cryovial_id and n.box_cid=cs.box_cid and n.box_cid=bs.box_cid and bs.status=6 and bs.rack_cid=r.rack_cid and r.tank_cid=m.tank_cid and cs.retrieval_cid=jobID;

Roughly half of these will be for the primary aliquot (i.e. `cryovial.aliquot_type_cid = c_retrieval_job.primary_aliquot`).  The other half will be for the secondary aliquot.  Primary and secondary should have the same barcode.

Entries for the primary aliquot could have a destination defined.  You could find these using a left join (but not on a distributed database, hence no location):

    select … from cryovial_store s1 left join cryovial c on c.cryovial_id=s1.cryovial_id
    left join  box_name n1 on  n1.box_cid=s1.box_cid
    left join cryovial_store s2 on s1.cryovial_id=s2.cryovial_id and s2.status=0
    left join box_name n2 on n2.box_cid = s2.box_cid
    where s1.retrieval_cid = jobID

Create suitable `box_name` and `cryovial_store` entries if no destination boxes have been defined (but only for one cryovial in each pair)

Piece this information together to create a list giving the cryovial barcode, destination box, position, current box, position, structure and location of the primary and secondary

Display the size of the job and ask user if they want to divide up the list.  If they do:

1.	Ask them the maximum section size (default = 500 cryovials)
2.	Calculate slot/box (where `c_box_size.box_size_cid = box_content.box_size_cid`)
3.	Ask them to select the size of first section from a list – it must be a multiple of the box size (from 2) and no more than the maximum (from 1)
4.	Allocate the appropriate number of destination boxes to the first section
5.	Repeat steps (2) and (3) until every entry has been allocated to a section

After division, show each section in turn and allow the user to sort by location, otherwise show the whole list and allow them to sort that.

Ask user to save changes with the option of going back to re-order if necessary.

Insert an entry into `c_box_retrieval` for each destination box, recording the section it is in and its position within that section (you might use position = 0 to indicate the section should be sorted by current sample location).

    update c_retrieval_job set status=in progress (1)

## Other jobs

    C_retrieval_job.status = in progress (1) and job.type in (2,3,4,5): act on list [tbd]
    C_retrieval_job.status not in (0,1) or job.type not in (2,3,4,5): complain

## Meeting July 2013

split up 10,000 samples (boxes?)
5 boxes ~ 500 samples

mixed up boxes -> tank -> new boxes > `c_box_retrieval` (order of cryovials?)

chunk 1: boxes 1-5
chunk 2: boxes 5-10
...

"retrieval plan"

primary aliquot (preferred)
secondary aliquot (optional/backup)

deal with empty spaces?

## Discussion w/Nick 2013-07-24 ##

How to display box chunks and sample chunks. List of samples could be very big - don't want to display by default.

Instead, each of either box list or sample list is displayed as one big chunk to begin with, which contains all boxes or samples in the list. Chunks can be added and edited. Last chunk must contain everything and cannot be deleted.

When configuring a sample list, an option is given to 'auto-chunk' - divide sample list up automatically using algorithm supplied by Martin. If so, List Management screen begins with chunks already created (may be only one chunk if list of samples is few enough).

Run this by Martin when he gets back in 2nd week Aug.


## Impromptu discussion one very hot Friday afternoon 2013-07-19

exercise (e.g. 'LPA Boxes') consists of one or more jobs in `c_retrieval_job`, which are created by Create List (in development), e.g.

    job: retrieve 4000 boxes
        chunk 1
        chunk 2
        chunk 3
    job: get 500 of those
        chunk 1
    job: analyse 500
        chunk 1
    job: analyse 3500
        chunk 1
        chunk 2

as each job (e.g. 'retrieve 4000 boxes') may be too big to do in one go, jobs can be tackled in chunks, which are recorded implicitly in `c_box_retrieval` (currently empty, reserved for retrieval assistant (structure can be changed if necessary):

    box_id          # The box being retrieved (for box retrieval/disposal) or retrieved into (for sample retrieval/disposal)
    retrieval_cid   # The retrieval task this entry is part of
    retrieval_type  # obsolete
    section         # Which chunk of the retrieval plan this entry belongs to (0 = retrieve all boxes)
    position        # The position of this entry in that chunk (may be 0 for sample retrieval, i.e. use l_cryovial_retrieval position)
    box_name        # obsolete
    rj_box_cid      # Unique ID for this retrieval list entry (what does rj stand for?)
    status          # 0: new record; 1: part-filled, 2: collected; 3: not found; 99: record deleted

(from `file:///K:/webcore/LIMS/database/v2.7/CentralDb.htm#c_box_retrieval`)

`section` is the chunk - just a number. Each box has a record in here and is linked to the `retrieval_job`
    
`l_cryovial_retrieval` (empty and reserved as well) lists the individual cryovials in each box to be retrieved, thus is the actual retrieval list:
  
	rj_box_cid      # record id of c_box_retrieval entry for the box the sample should be placed into?
    position        # Where this sample appears in [the current chunk of] the retrieval plan
    cryovial_barcode# The barcode on the sample
    aliquot_type_cid# The aliquot type (cryovial_barcode + aliquot_type_cid should uniquely identify the cryovial within the project)
    slot_number     # The expected position of the cryovial in the destination box (if two records suggest the same position in the same box, the first should be the primary aliquot; the second will be ignored if the first is found)
    process_cid     # Who stored it, which program etc.
    status          # 0: expected; 1: ignored, 2: collected; 3: not found; 99: record deleted
      
## Sample History 

I thought it might be worth taking this as a case study of how the sample history is supposed to work.  Please let me know if either of you have any comments.

> we have the attached 4000 samples retrieved for THRIVE, 500 of these were selected for analysis before the others (also attached) we need to come up with a process that makes some sense that allows these samples to be return to storage in their existing boxes in new locations, and to reflect the unusual sample history these 500 samples will now display.

### Original boxes: ##

* Aliquot Allocation creates an entry in `box_name` when a new box is required to store received cryovials (boxes will probably wait in the cold room until they've been filled but there's no direct record of this)
* BoxReception creates an entry in `l_box_arrival` when the box is allocated a location (site+population+structure)
* StorageSync creates an entry in `box_store` once the box location has been confirmed [or BoxReception v3, or StoreMan for referred boxes]
* (boxes may be taken out of storage briefly to retrieve samples from but there's no direct record of this)

### Retrieval boxes: ##

* StoreMan::CreateList creates 40 entries in `box_name` to hold the required cryovials when it creates the retrieval job (boxes may just wait in the cold room until they're analysed but, if we need to keep a record:
  * **Retrieval Assistant deals with the jobs somehow**
  * BoxReception creates an entry in `l_box_arrival` when the box is allocated a location (site+population+structure)
  * StorageSync creates an entry in `box_store` once the box location has been confirmed
  * `StoreMan::?` marks the box_store record "removed for analysis" when it makes the specimen entries for the analysis)
* `BoxReception` creates [another] entry in `l_box_arrival` when the box is allocated a [new] location
* StorageSync creates an entry in `box_store` once the box location has been confirmed

### Example Exercise: LPA boxes: ###

* `StoreMan::CreateList` creates five more entries in `box_name` to hold the required cryovials
* (I'm assuming the boxes weren't kept in store for long so there's no need for a record)
* BoxReception creates an entry in `l_box_arrival` when the box is allocated a location
* StorageSync creates an entry in `box_store` once the box location has been confirmed

#### 3500 samples: ##

* Aliquot Allocation creates an entry in cryovial once the tube is scanned in (and aliquots taken, if UK samples)
* Aliquot Allocation creates an entry in `cryovial_store` once the cryovial has been assigned to a box
* StoreMan creates a second `cryovial_store` record giving its expected position in one of the retrieval boxes when it creates the retrieval job
* The retrieval assistant marks the first `cryovial_store` entry "transferred to new box" and updates the second once the sample has been retrieved [it may also 99 the second `cryovial_store` entry of the secondary aliquot if there is one]
* StoreMan marks the second `cryovial_store` record "removed for analysis" when it makes the specimen entry for the analysis [although it might be better is Specimen Reception did this]
* StoreMan creates a third `cryovial_store` record when the cryovial is put back in the box [Can this be confirmed?  When?  As things stand we create the third entry when we update the second; this doesn't give much idea of how long the sample was unfrozen for]

#### 500 samples: ##

* Aliquot Allocation creates an entry in cryovial once the tube is scanned in (and aliquots taken, if UK samples)
* Aliquot Allocation creates an entry in `cryovial_store` once the cryovial has been assigned to a box
* StoreMan creates a second `cryovial_store` record giving its expected position in one of the retrieval boxes when it creates the first retrieval job
* The retrieval assistant marks the first `cryovial_store` entry "transferred to new box" and updates the second once the sample has been retrieved [it may also 99 the second `cryovial_store` entry of the secondary aliquot if there is one].
* StoreMan creates a third `cryovial_store` record giving its expected position in one of the LPA boxes when it creates the second retrieval job
* The retrieval assistant marks the second `cryovial_store` entry "transferred to new box" and updates the third once the sample has been retrieved
* StoreMan marks the third `cryovial_store` record "removed for analysis" when it makes the specimen entry for the analysis
* StoreMan creates a final `cryovial_store` record when the cryovial is put back in the LPA box

The job history needs to be discussed further.  As it stands, we would have several records in `c_retrieval_job` with no obvious link between them with records in `c_box_retrieval` to keep track of the chunks being worked on.

I realise this is somewhat repetitive but I hope it clarifies the process
-- Nick

## Questions from me ##

> Should I use LCDbCryoJob/s for the jobs we are talking about?

Yes; read from `c_retrieval_job`

> "For in progress lists these steps should be skipped (REQ 8.3.7). " not clear which steps at the moment....
Check status: only look at status=0
There are four type of job: 
    cryovial retrieval for analysis is the most complex - that's what we talked about this morning; 
    cryovial retrieval for disposal is similar; 
    box retrieval for analysis and 
    box retrieval for disposal are far simpler - they bring whole boxes back to the lab

> Work through list or sub-section by giving the storage location and sample ID of each sample on the list in the order saved above (REQ 8.3.8)" 
> 'giving??' - does this mean displaying the details of each sample in turn? (ie one at a time - in big letters, I presume?)

Not for next week but yes, display current and destination location of each sample, one at a time (highlighting current sample in the context of five previous and a couple more still to be collected).

> Do I need to interface with the barcode scanner and if so how?

Treat it as reliable keyboard input

> "The option to exit the process saving progress should be offered, with an "are you sure?" message in case of accidental selection (REQ 8.3.12)."
> presume this means "The option to exit the process, saving progress" (note comma)

Agreed - they can either resume the process immediately if they say they don't want to exit or they can resume later if they say they're sure they want to close the window.  I think the same could apply to 8.3.6 but do whatever's simplest and safest

One more thing about retrieval lists.  There's potentially a two-level hierarchy: a retrieval task (in `c_retrieval_job`) can belong to a retrieval exercise (in `c_object_name`).  This is a fairly new idea so, although it's documented for 2.7.2, the data's not all there yet.  That may be why you couldn't find the fields you were looking for

### 28/06/2013 22:46

I doubt you'll get as far as thinking about how to store a retrieval plan this week.  If you do, it currently allows for four options:

section = 0, position = 0: fill all the boxes at once, in default order (i.e. by storage population, structure, slot and cryovial position, a.k.a. tank/rack/slot/position)
section = 0, position > 0: fill the boxes one by one, from start to finish (i.e. by destination position)
section > 0, position = 0: fill all the boxes in the first section in default order, then the second section, and so on.
section > 0, position > 0: fill the boxes in the first section one by one, then the second section, and so on.

You might have one section sorted one way and another section sorted the other but I've only allowed for these two possibilities.  From what Martin was saying about needing greater flexibility in future, we may need to change the table structure to list every cryovial, at least in some cases.  I'll look at that when I get back

## Misc SQL

    select * from c_retrieval_job rj, cryovial_store cs where rj.retrieval_cid = cs.retrieval_cid order by cs.box_cid
    select * from box_content bc, c_box_size bs where bc.box_size_cid = bs.box_size_cid
    select rj.description, ob.external_full from c_retrieval_job rj, c_object_name ob where rj.exercise_cid = ob.object_cid
    select * from c_retrieval_job where start_date > date('now') - date('40 days')


`c_retrieval_job`: record -1015, `job type` 4, `status` 0, `primary aliquot` EDTA_2, no secondary aliquot.  The retrieval assistant might split this into (say) eight chunks of five boxes in `c_box_retrieval` with cryovials in `l_cryovial_retrieval`.  It would set `c_retrieval_job.status` to 1 and the start date to `'now'`.  The operator would retrieve a chunk at a time, ticked off in in `c_box_retrieval` and `l_cryovial_retrieval`.  When that's finished, it would set `c_retrieval_job.status` to 2 and finish date to `'now'`.

`cryovial_store`: the 4000-odd records saying where the cryovials are at the moment have `retrieval_cid -1015, status = 2, removed = ''`.  Set `status = 5, removed = 'now'` when they're retrieved.  Each cryovial has a second `cryovial_store` record giving the expected destination, `status = 0`.  Set `status = 1` when the position's confirmed


## Example retrieval plans

-116: ten rows in `c_box_retrieval`.  
It's only a small job so it doesn't need chunks and they're retrieving whole boxes so it doesn't need to list individual cryovials.  
In this example, the retrieval plan is sorted by box location - the boxes were in the same segment in the same population so I ordered them by rack and slot giving `c_box_retrieval.section` from 1 to 10.

-1180118: twelve rows in `c_box_retrieval` 
for the destination boxes; 1140 in `l_cryovial_retrieval`.  It's a large enough job to need splitting into chunks (section 1 to 5).  The retrieval plan is sorted by cryovial location.  In this example, the boxes were filled one at a time but that's often not be the case.

BTW, `c_box_retrieval.position` is redundant - once you've created a retrieval plan there's no obvious difference between having no chunks and having one box per chunk.  The URS implies you can't edit a retrieval plan once it's been saved and the position in `l_cryovial_retrieval` takes priority anyway.

### stuff

bool LCDbCryoJob::isAvailable() const
{
	return status != INPROGRESS
		|| processID == LCDbAuditTrail::getCurrent().getProcessID()
		|| claimed_until < Now();

	enum Category { UNKNOWN, CALIBRANT, REAGENT, ANALYSER_EVENT, SIS_EVENT, CLUSTER = 5,
					ALIQUOT_TYPE, STORAGE_TYPE, STORAGE_SITE, STORAGE_POPULATION, TANK_LAYOUT = 10,
					PROGRAM_NAME, SAMPLE_CATEGORY, RESULT_ATTRIBUTE, LAB_NAME, ALIQUOT_CATEGORY = 15,
					STORAGE_VESSEL, QC_MATERIAL, TUBE_TYPE, CANNED_TEXT, STORAGE_EXERCISE = 20,
					NUM_TYPES };
                    
LCDbProjects::getCurrentID()

short LPDbBoxName::getSize() 


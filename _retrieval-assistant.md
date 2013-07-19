# Retrieval Assistant

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


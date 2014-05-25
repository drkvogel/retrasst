## long waits for queries slows down dev

can do 'other things' in meantime. 
make program send email to myself with details of how long each query took, for records.

## LCDbCryoJob::str()

put debug string in here, inc. name, desc, id, etc
primary, secondary aliquot

 loadRows for job

## move Util::getAliquotDescription() to LCDbCryoJob?

so job.str() can print meaningful aliquot types

## debugLog() should take std::string

    debugLog



## bad chunk

### 978238 several REVEAL boxes

>job: 978238, projectid: -582959, status: 1, type: 4: desc: Several REVEAL boxes for Chris

#### plan

699 total

Primary: `EDTA_1`, Secondary: `EDTA_2`

1.  EDTA_BC     199
2.  EDTA_2      100
3.  EDTA_1      400

Appears to save 100 vials in chunk 2 (says 'primary' though - incorrect text)

#### collect

    25/05/2014 18:19:26: loadPlan: job: id: 978238, name: "Retrieval_978238", desc: "Several REVEAL boxes for Chris", type: 4, status: 1, project: -582959, primary: -31781, secondary: -31782, reason: "Checking Retrieval Assistant"

appears to load 690+ vials

1.  EDTA_BC     199
2.  ?           0
3.  EDTA_1      400

chunk 2 is size 0.

i.e. secondaries (EDTA_2) just ain't there.

#### combineAliquots()

primaries.size():   599
secondaries.size(): 100
combined.size():    699

i.e. looks OK

because of recent changes?

e.g. in `LoadPlanThread::Execute()`?

        row->dest_type_name = Util::boxTubeTypeName(row->project_cid, row->dest_box_id).c_str();

        main->getStorage(row);

were put in main loop, take out

has the `order by` gone wrong?

now chunks _looks_ OK - 199, 100, 400 == 699 - not sure what I did to change this - but chunk 2 is all `EDTA_1` instead of `EDTA_2`, and chunk 3 has the `EDTA_2`s at the end - something has gone wrong with chunking

Ah, I moved `rowCount++`, and `rowCount` is a class variable.... I think that might have something to do with it...

## box types are all the same?
    e.g. 978238 QClot_new 978237
    are they meant to be? yes, no jobs with multiple box types yet
    take out of main loop

### job: 978253, projectid: -149662, status: 1

10 primaries with secondary backups
split into 3 chunks
looks OK

### job id: 978243 "Retrieval_978243", "Reveal boxes of three types"

3 different aliquot types but box types all same
sort and chunk by aliquot type
load plan
middle chunk (2) is 0 size and crashes
other chunks are 500 size each and seem to contain more than one aliquot


### ideas

try earlier commits? e.g.
84af27cefeeeaac499ce3e210db2be8cf9961756 [84af27c] before cvs pull



## fixme    

---

###  978243 "Retrieval_978243", "Reveal boxes of three types"

Contains secondaries, but not as backups (not same destination position) of primaries

### 978243 ("Reveal boxes of three types")

Primaries with secondaries (?) as backups




## confusing methods?

currentSample(), 
currentAliquot()
chunk->currentObject()
fillRow()


## chunkComplete()

## SaveProgressThread::Execute()

## SaveProgressThread::storeSample()

## SaveProgressThread::jobFinished()


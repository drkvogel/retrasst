

## long waits for queries slows down dev

can do 'other things' in meantime. 
make program send email to myself with details of how long each query took, for records.

## cache/hash column names

not important

## debugging rosetta/database errors

put breakpoints on 

    LIMSDatabase::xerrorCallback()
    LIMSDatabase::rosettaCallback()
    LIMSDatabase::xdbErrorCallback()

## more loading messages

to show something's happening

## LCDbCryoJob::str()

put debug string in here, inc. name, desc, id, primary, secondary aliquot

## move Util::getAliquotDescription() to LCDbCryoJob?

So `job.str()` can print meaningful aliquot types

## debugLog() should take std::string

    debugLog



## bad chunk

    FormShow()
        chunks.clear();
        sgwChunks->clear();

    LoadPlanThread::Execute() 
        if (aliquotType == secondary_aliquot)
            collect->secondaries.push_back(row)
        else // everything else, even if not explicitly primary
            collect->primaries.push_back(row)
        
        main->combineAliquots(collect->primaries, collect->secondaries, collect->combined);
        int combinedCount = collect->combined.size() // 699
        

combineAliquots(primaries, secondaries, combined) {
    posCache cache

    # store primaries and cache
    combined.clear();
    foreach row in primaries
        PosKey key(row)
        cache[key] = row
        combined.push_back(row)

    # try to match secondaries based on same box/pos key
    foreach row in secondaries
        PosKey key(row)
        found = cache.find(key)
        if found != cache.end()         # destination box and position already used (by primary)
            if NULL == row throw runtime_error
            found->second->backup = row # add as backup to primary
        else
            combined.push_back(row)     # add to list in its own right


### problem

Separating into primary and secondary, then doing `combineAliquot()`, messes up the order of the rows when secondaries are 'loose'

### solution

either:

sort combined by section, pos

or:

don't sort into primary and secondary - in `combineAliquots()`, just match ones where `aliquotType == secondary_aliquot`

will this mess up the combine in plan? think not, because the list is meant to be unsorted at that point.



Chunk::add method?

Don't need order by aliquot? plan should determine order in collect
Remember `rowCount` is a class variable - check it's been reset where needed.
if secondary doesn't match, put in combined-

Should plan store secondaries, or should they be implicit and displayed only by collect?

    showChunks() -> showChunk()

Are we reading all aliquots from db or just 'primaries'?

Now we combinealiquots *after* loading rows, not during the loop - but still create the chunks during the loop, using the rowCount - this is incorrect? Should create chunks after load - but need section (chunk), which is in LCDbBoxRetrieval but not LCDbCryovialRetrieval. Need to include `LCDbBoxRetrieval` in `SampleRow` or just `int section`?

### weird bomb-outs

calling

    row->dest_type_name = Util::boxTubeTypeName(row->cbr_record->getProjId(), row->dest_box_id).c_str();

inside or out of the main loops seems to cause `LoadPlanThread` to terminate early (leaving 0 chunks and therefore an exception). rebooted as weird things happening... Turns out Ingres on `vlabdev` was throwing wobblies.

### cbr_record NULL

`SampleRow` `row->cbr_record` appears to be initialised but is then null.. Ah, I was initialising superclass with uninitialised `cbr_record` member rather than `cbr_rec` constructor parameter.

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


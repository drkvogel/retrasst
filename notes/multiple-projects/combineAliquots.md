## fix/replace combineAliquots in collect

replace with code similar to old method, based on assumption that backups will be immediately after preferred vials in the plan (which they will)

## are secondaries stored by plan and how? (yes)

    978253 "Retrieving THRIVE samples (with secondary)"

reset job
run plan query - no results.
10 combined vials. order by pos 
chunks
1. dest pos 1, 2, 4
2. dest pos 5, 7, 11
3. dest pos 12, 13, 14, 16
save

secondaries are included in plan with separate lcr_pos to primaries
ie. if plan is *saved* correctly, secondaries should appear in list after the primaries they are a backup of
is this the case?
looks like it in SavePlanThread::save() and from plan query
then in collect, don't need to sort by aliquot - they should be in the right order - and each secondary should be able to be matched to a previous primary.
but because of combineAliquots()(?) the order is messed up.

what would combineAliquots() do to this list?
looks like it does it correctly
should work on combined one
but if there are loose secondaries, they would be the end of the combined list, possibly out of chunk order

can combineAliquots combine backups with primaries correctly as it stands? no
can it be made to do so? er...
do we need a new function to combine in collect? maybe do it the old way

they should not be sorted into prim/sec in the main loop, just in one list which will be ordered correctly
combine function should add backup to previous if they match, loose if not

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
        

    combineAliquots(primaries, secondaries, combined):
        posCache cache
     
        # store primaries and cache
        combined.clear();
        foreach row in primaries:
            PosKey key(row)
            cache[key] = row
            combined.push_back(row)
         
        # try to match secondaries based on same box/pos key
        foreach row in secondaries:
            PosKey key(row)
            found = cache.find(key)
            if found != cache.end():         # destination box and position already used (by primary)
                if NULL == row throw runtime_error
                found->second->backup = row # add as backup to primary
            else:
                combined.push_back(row)     # add to list in its own right

### problem

Separating into primary and secondary, then doing `combineAliquot()`, messes up the order of the rows when secondaries are 'loose'
i.e., it makes the loose secondaries go to the end of the list.

in collect, sorting into primary and secondary (which is ok in plan), ruins the order of the plan. combineAliquots() could perhaps do its thing in a different way on one vector

### questions

is plan properly sorted at point of being loaded into collect? yes
ie. should we disrupt that in any way? no

### solution

either: sort combined by section, pos

or: don't sort into primary and secondary - in `combineAliquots()`, just match ones where `aliquotType == secondary_aliquot`
will this mess up the combine in plan? think not, because the list is meant to be unsorted at that point.

Chunk::add method?

Don't need order by aliquot? plan should determine order in collect
Remember `rowCount` is a class variable - check it's been reset where needed.
if secondary doesn't match, put in combined-

Should plan store secondaries, or should they be implicit and displayed only by collect?

    showChunks() -> showChunk()

Are we reading all aliquots from db or just 'primaries'?

Now we combinealiquots *after* loading rows, not during the loop - but still create the chunks during the loop, using the rowCount - this is incorrect? Should create chunks after load - but need section (chunk), which is in LCDbBoxRetrieval but not LCDbCryovialRetrieval. Need to include `LCDbBoxRetrieval` in `SampleRow` or just `int section`?

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
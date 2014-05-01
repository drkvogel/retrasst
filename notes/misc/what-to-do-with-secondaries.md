should we save secondary aliquots in the the plan? I thought not

In PlanSamples:

    sgVials->Cells[sgwVials->colNameToInt("destpos")]  [rw] = sampleRow->dest_cryo_pos;

    dest_cryo_pos is 8th param of SampleRow::SampleRow()
    ie. dest_pos, 
    ie. s2.cryovial_position
    s2.status = 0 [EXPECTED] ie. dest box

Looked through emails mentioning 'aliquot', nothing conclusive.

`retrasst/secondary-aliquots/primary-secondary.txt` says (amongst other things):

  * if 2ndary specified, load them in plan retrieval, and save them in the plan.
  * thus we don't need to search for them again in collect
  * I think we agreed with martin/tim that displaying 2ndary aliquots at the planning stage would be confusing? 
     * option to show if required?
  * 2ndaries may not need to be displayed at first, but should be loaded

so yes, we should save them in the plan.

## RetrAsstPlanSamples

### LoadVialsWorkerThread::load()

* saves the secondaries in SampleRow->secondary where found
* storage not loaded for secondary - don't need to at this stage?
dest

### SavePlanThread::save() 

    saveSample(Chunk< SampleRow > * chunk, SampleRow * sampleRow, int rj_box_cid)

changed this last week to use pos++ for `l_cryovial_retrieval.` - should use `dest_pos` as originally

`dest_cryo_pos`  ie. `dest_pos` ie. `s2.cryovial_position` for job 1086654

## RetrAsstCollectSamples

### LoadPlanThread::Execute()

loads storage details for secondary this time

1086654 job means retrieve both aliquots always

* some samples may not have primaries!
* some primaries may not have secondaries!
* some secondaries may be intended as backup samples
* some secondaries may be required as well as the primary
* ie. can make no assumptions about which is present for each sample or which or whether both are required

should present both aliquots on plan screen to enable sorting
be able to toggle between showing both and showing only primary (sort by primary)
if both aliquots of a sample on (e.g.) the first row or most rows have the same destination position, assume primary preferred
if aliquots have different destination positions, assume both required

### combine or not?

present all aliquots all the time, or 

provide option to combine aliquots?
test if some or most or all(?) aliquots are paired ie. secondary is backup for primary
 
at plan save

primaries
secondaries
sortList

## cross-project jobs

also... samples might come from more than 1 project... which I haven't catered for so far. 

how to load them in at

temp table

### job load



### plan save

both aliquots should be included in save

### plan load

## Questions for Martin

How important is it/might it be to be able to deal with retrieval jobs that have vials from more than one project? i.e. would it be a significant hindrance to have to create different jobs for different projects?

As I see it there are (forgetting about projects for the minute) several types of retrieval job:

1. primary aliquots only ('primary only')
2. pairs of primary and secondary aliquots, each pair of which has only one destination box position - ie. the primary is preferred and is stored in the destination if found, else the secondary is stored if found ('primary preferred')
3. primary and secondary aliquots, all of which are to be retrieved into different destination box positions ('primary and secondary')
4. a mix of 2 and 3?

I have assumed only possibilities 1 and 2 so far, which was a mistake - certainly 3 now exists as a test job.

I suppose these could be broadly categorised into two types - those in which all aliquots 

Perhaps there is only one 'type' really - a possible mix. If there are secondaries and they have their own destinations, they should be shown for sorting, chunking, planning - and if there are any secondaries which share a destination with a primary, they should 'hide' behind the primary - they are only there for backup.

ie. you are only ever sorting/planning the destinations

Perhaps colour-code:

 * primary
 * secondary
 * combined

SampleRow or LCDbCryovialRetrieval should include dest box id?
  as well as `rj_box_cid`?
already does; inherited from RetrievalRow

    string              src_box_name;       // id and cryo pos are in store_record
    int                 dest_box_id;
    string              dest_box_name;
    string              site_name;
    int                 vessel_pos;
    string              vessel_name;
    int                 shelf_number;
    int                 structure_pos;      // c_rack_number.position as rack_pos
    string              structure_name;
    int                 box_pos;

    select * from c_retrieval_job 1086654
    select * from cryovial c, cryovial_store cs where c.cryovial_id = cs.cryovial_id and retrieval_cid = 1086654
    select * from cryovial_store where retrieval_cid = 1086654
    select * from c_box_retrieval where retrieval_cid = 1086654
    SELECT cbr.retrieval_cid, section AS chunk, cbr.rj_box_cid, cbr.box_id AS dest_id, lcr.position AS lcr_position, lcr.cryovial_barcode, lcr.aliquot_type_cid, lcr.process_cid AS lcr_procid, lcr.status AS lcr_status, lcr.slot_number AS lcr_slot, lcr.slot_number AS dest_pos FROM c_box_retrieval cbr, l_cryovial_retrieval lcr WHERE cbr.retrieval_cid = 1086650 AND cbr.rj_box_cid = lcr.rj_box_cid ORDER BY chunk, rj_box_cid, lcr_position, aliquot_type_cid
    select * from l_cryovial_retrieval where rj_box_cid = 1086697
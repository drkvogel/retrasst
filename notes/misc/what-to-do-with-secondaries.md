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

## RetrAsstCollectSamples

### LoadPlanThread::Execute()

loads storage details for secondary this time



notes:

big-list.md:11
discard-boxes/gj.md:2
discard-boxes/_discard-boxes.md:0
done.md:25
helper-funcs.txt:0
meetings/meeting-2014-02-13.md:4
misc/99-boxes.txt:0
misc/chunking.txt:0
misc/convos.docx:0
misc/c_retrieval_job.md:4
misc/db-tables-cheatsheet.docx:0
misc/easy-wins.md.bak:0
misc/excel-sorter.png:0
misc/impossible.md:0
misc/load-plan-samples-possible-1.svg:2
misc/load-samples.svg:2
misc/outer-join-not-allowed-ddb.png:0
misc/queryviz.txt:8
misc/screen-mockups.jpg:0
misc/terse-spec.md:6
misc/what-to-do-with-secondaries.md:2
old/misc-2.txt:6
old/more-notes-2.txt:16
old/orig-spec.txt:9
old/q.txt:7
old/sort-problems.txt:0
old/sorter.txt:3
spec-comparison/2013-07-31.txt:7
spec-comparison/2013-07-31.txt.bak:6
spec-comparison/2013-09-11.txt:5
spec-comparison/2013-09-11.txt.bak:4
truth-table.xlsx:0

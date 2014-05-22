## storeman log in

Hello both 
 
I have modified the log-in screen to select the appropriate database system for the new setup 
 
    Live = vnode_vlab_64::ldb* (the default for the release version) 
    Test = vnode_vlab_64::test_ldb* 
    Mirror = vnode_labdev_64::ldb* 
    Dev = vnode_labdev_64::t_ldb* (the default for the debug version) 
    Db Test = vnode_labdev_64::test_ldb* 
 
Please make sure you have your vnodes set up correctly. Don't use "mirror" if you're not sure. 
 
Jonathan: can you use the database connection from the log-in screen in your code? I'm happy to change my code to make that easier 
-- 
Nick 

## combineAliquots()

does combineAliquots work properly?

can we use it in collect?

in collect, the old method is used, relying on alternating order of primary/secondary. seems to do a reasonable job. code should be shared for consistency, though.

## combineAliquots() not working for collect

Vector sizes are as expected for job of 10 primaries with secondary backups: 10 primary, 10 secondary, 10 combined

But each combined sample->backup is NULL.

### look at combined during combineAliquots

...

### how is preparation different from in plan?

Declaration of
    
    private:
        ...
        vecpSampleRow                               primaries;
        vecpSampleRow                               secondaries;
        vecpSampleRow                               combined;

is the same in plan and collect.

### how is use of combine different from old way (commented out)?

...

## step through

### plan

10 prim, 10 sec, 0 combined

combineAliquots()
    combined.push_back(row);
    found->second->backup = row; // add as backup to primary

10 prim, 10 sec, 10 combined
MyFirst[5] is not NULL

clicking on loaded job shows backup aliquots in debug window

save plan

### collect

open plan

10, 10, 0

combineAliquots()
    combined.push_back(row);
    found->second->backup = row; // add as backup to primary

    combined.push_back(row) NOT called - 

10, 10, 10
((collect->combined)._MyFirst)[5] is NULL
sample->backup is NULL

### later

somehow ended up with 20 prim and sec for the same plan....sth funny going on

does combined.clear(); work?
combined[1] is NULL

    (((found._Ptr)->_Myval).first).box: 0
    (((found._Ptr)->_Myval).first).pos: -623407 //??? pos and box wrong way round?

    (((found._Ptr)->_Myval).second)->cryovial_barcode   "112004791"
    (((found._Ptr)->_Myval).second)->dest_cryo_pos:     0
    (((found._Ptr)->_Myval).second)->backup
    (((found._Ptr)->_Myval).second)->dest_box_id:       -623407
    (((found._Ptr)->_Myval).second)->dest_box_type:     978252          // ?
    (((found._Ptr)->_Myval).second)->
    (((found._Ptr)->_Myval).second)->

Constructor arguments are the wrong way around!:

    PosKey(SampleRow * s) : box(s->dest_cryo_pos), pos(s->dest_box_id) { }

somehow worked in plan...

but still no dice - no backups are shown in collect...
just noticed that dest pos is shown as being 0 for all boxes in collect

debug strings in `TfrmRetrAsstCollectSamples::sgVialsClick()`:

        <<" retrieval_status: "<<sample->retrieval_record->getStatus()
        <<" ("<<sample->retrieval_record->statusString(sample->retrieval_record->getStatus())<<")"
        <<", cryo_status: "<<sample->cryo_record->getStatus()
        <<", store_status: "<<sample->store_record->getStatus()
        <<", barcode: "<<sample->cryovial_barcode.c_str()
        <<", storage: "<<sample->storage_str().c_str()
        <<", dest: "<<sample->dest_str().c_str()
        <<", aliq: "<<sample->cryo_record->getAliquotType()

could be replaced with `SampleRow::str()`:

            <<"id: "<<(store_record->getID())<<", " 
                //  LPDbCryovialStore: cryovialID, boxID, retrievalID, status, position
                // <<"status: "<<(store_record->getStatus())<<", " 
                // LPDbCryovial: barcode, boxID, sampleID, typeID, storeID, retrievalID, status, position 
                //<<"barcode: "<<store_record->getBarcode() 
                //<<"sampleID"<<cryo_record->getSampleID() 
                //<<"aliquot type ID"<<cryo_record->getAliquotType()
            <<"proj: "<<(project_cid)<<", "
            <<"status: "<<store_record->getStatus()<<", "
            <<"barc: "<<cryovial_barcode<<", "<<"aliq: "<<aliquotName()<<", "
            <<"src: {"<<store_record->getBoxID()<<", "<<src_box_name<<"["<<store_record->getPosition()<<"]}, "
            <<"dst: {"<<dest_box_id<<", "<<dest_box_name<<"["<<dest_cryo_pos<<"]}, "
            <<"loc: {"<<storage_str()<<"}";

perhaps?

"commenting out" duplicated debug info:

        //<<" retrieval_status: "<<sample->retrieval_record->getStatus()
        //<<" ("<<sample->retrieval_record->statusString(sample->retrieval_record->getStatus())<<")"
        //<<", cryo_status: "<<sample->cryo_record->getStatus()
        //<<", store_status: "<<sample->store_record->getStatus()
        //<<", barcode: "<<sample->cryovial_barcode.c_str()
        //<<", storage: "<<sample->storage_str().c_str()
        //<<", dest: "<<sample->dest_str().c_str()
        //<<", aliq: "<<sample->cryo_record->getAliquotType()

to add to `SampleRow::str()`:

    <<" retrieval_status: "<<sample->retrieval_record->getStatus()
    <<" ("<<sample->retrieval_record->statusString(sample->retrieval_record->getStatus())<<")"
    <<"aliquot type ID"<<cryo_record->getAliquotType()
    <<"barcode: "<<store_record->getBarcode()
    <<", cryo_status: "<<sample->cryo_record->getStatus()
 


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


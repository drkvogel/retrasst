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

somehow worked in plan... actually, in terms of being an `< int, int >` key, it may not have mattered that it was the wrong way round...

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
 
extra (perhaps unnecessary) debug confirms that pos is 0 for all items

It was constructing SampleRow with the parameter for `dest_cryo_pos` being `qd.readInt("dest_pos")`, and `dest_pos` was coming from 

    lcr.slot_number AS dest_pos

when it should (?) come from

    lcr.new_position AS dest_pos

in plan it comes from 

    s2.cryovial_position as dest_pos

actually, `new_position` is still required by `LCDbCryovialRetrieval`. Use it in place of `dest_pos` to construct SampleRow with correct destination positions (check that `lcr.new_position` is intended for this and populated correctly).

IT WORKS! Finally! (19:32)

I think before I replaced the old code in collect with combineAliquots() it was using the sorted, alternating aliquot types to succesfully match secondaries to primaries, even though they'd lost their destination positions and I hadn't noticed.
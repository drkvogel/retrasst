# Totally messed up now, e.g.

`retrieval_cid` 1086650

     SELECT 
        cbr.retrieval_cid, section AS chunk, cbr.rj_box_cid, cbr.box_id AS dest_id, 
        lcr.position AS lcr_position, lcr.cryovial_barcode, lcr.aliquot_type_cid, 
        lcr.process_cid AS lcr_procid, lcr.status AS lcr_status, lcr.slot_number AS lcr_slot, 
        lcr.slot_number AS dest_pos, 
        cs.box_cid, sb.external_name AS src_box, cs.cryovial_position AS source_pos,  
        db.external_name AS dest_box, 
        cs.note_exists, cs.cryovial_id, cs.cryovial_position, cs.status, 
        c.sample_id, cs.record_id, 
        db.external_name AS dest_name 
     FROM 
        c_box_retrieval cbr, l_cryovial_retrieval lcr, c_box_name db, c_box_name sb, cryovial c, cryovial_store cs 
     WHERE 
        cbr.retrieval_cid   = :rtid AND 
        cbr.rj_box_cid      = lcr.rj_box_cid AND 
        cbr.box_id          = db.box_cid AND 
        c.cryovial_barcode  = lcr.cryovial_barcode AND 
        c.aliquot_type_cid  = lcr.aliquot_type_cid  AND 
        cs.cryovial_id      = c.cryovial_id  AND 
        cbr.retrieval_cid   = cs.retrieval_cid AND 
        cs.box_cid          = sb.box_cid 
     ORDER BY 
        chunk, rj_box_cid, lcr_position, aliquot_type_cid 
no rows

## `c_retrieval_job`

    select * from c_retrieval_job where retrieval_cid = 1086650
1 row

## `c_box_retrieval`

    select * from c_box_retrieval where retrieval_cid = 1086650
1 row

## `l_cryovial_retrieval`

    select * from l_cryovial_retrieval where rj_box_cid = 1086670
40 rows

_vdba 10s does recognise `--` SQL comments_

_or does it?_

## join `c_box_retrieval cbr`, `l_cryovial_retrieval`

     SELECT 
        cbr.retrieval_cid, section AS chunk, cbr.rj_box_cid, cbr.box_id AS dest_id, 
        lcr.position AS lcr_position, lcr.cryovial_barcode, lcr.aliquot_type_cid, 
        lcr.process_cid AS lcr_procid, lcr.status AS lcr_status, lcr.slot_number AS lcr_slot, 
        lcr.slot_number AS dest_pos
     FROM 
        c_box_retrieval cbr, l_cryovial_retrieval lcr
     WHERE 
        cbr.retrieval_cid = 1086650
        AND cbr.rj_box_cid = lcr.rj_box_cid 
     ORDER BY 
        chunk, rj_box_cid, lcr_position, aliquot_type_cid
40 rows, OK.

Fails on joining `c_box_name` as `box_name` records haven't yet been copied into central by Nick's overnight job. 
He is running that now. It saves `box_id`s in the project's `box_name` to `box_cid`s in `c_box_name`, reversing the sign. The job having been run, I tried to load a plan I had already created before, and it wouldn't load - as the plan had oppositely-signed `box_cid`s. Deleted the plan and did it again, can now load it in collect screen.

e.g. `cbr.box_cid AS dest_id = -623454` in the plan was not found in `c_box_name`. But it is now, so it wasn't my program that was wrong this time.

In future, the copying to `c_box_name` will happen automatically by RFPs, so Nick's program will not be needed and things shouldn't get out of sync like this.

I now have 3 new cryovial jobs and plans.

## now

Job 1086654 "Primary and secondary in alternate slots" - the secondaries are showing as well as (and before) the primaries. Hmm.
Logic for dealing with primary/secondary in plan load could be a bit less obscure.

primary:   -31781
secondary: -31782

primary is NOT < secondary, so list should be sorted aliquot_type_cid DESC - primary first, then secondary

but lcr_position makes them in that order - the plan is incorrect?

in RetrAsstPlanSamples, SavePlanThread::save(), Saver::saveSample(),
    LCDbCryovialRetrieval ctor was wrongly given `dest_cryo_pos` as the 2nd argument, to set position - position in lcr means the position in the chunk. Should be autoincremented as records are saved.

## `C_BOX_RETRIEVAL`

    rj_box_cid
        uid
    section
        chunk
    position
        obsolete

## `L_CRYOVIAL_RETRIEVAL`

    rj_box_cid
        Where this sample belongs (chunk, destination box etc)
    position
        Where this entry appears in the relevant chunk of the retrieval plan - samples are retrieved in this order
    old_box_cid
        The box the sample can currently be found in (`c_box_name`)
    old_position
        The expected position of the sample in that box
    new_position
        Where to put the cryovial in the destination box (if two records suggest the same position in the same box, the first should be the primary aliquot; the second will be ignored if the first is found)

should fill in oldbid, oldpos, newpos
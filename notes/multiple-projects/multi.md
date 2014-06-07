how is collect going to deal with multiple projects?
it's not, for now

    { { "proj name: dev_hps2-thrive, id: -149662, db: t_ldb20", 5, {  } } }

    SELECT  
    b1.project_cid,  
    s1.cryovial_id, s1.note_exists, s1.retrieval_cid, s1.box_cid, s1.status, 
    s1.cryovial_position,  s1.record_id, 
    c.sample_id, c.aliquot_type_cid,   c.cryovial_barcode,  
    b1.box_cid as source_id,  b1.external_name as source_name,  
    s1.cryovial_position as source_pos,  s2.box_cid as dest_box_id,  
    b2.external_name as dest_box_name,  b2.box_type_cid as dest_box_type,   
    s2.cryovial_position as dest_pos 
    FROM  cryovial c, cryovial_store s1, c_box_name b1,  cryovial_store s2, box_name b2 
    WHERE  c.cryovial_id = s1.cryovial_id 
    AND  b1.box_cid = s1.box_cid 
    AND  s1.cryovial_id = s2.cryovial_id 
    AND  s2.status = 0 
    AND  b2.box_cid = s2.box_cid 
    AND  s1.retrieval_cid = :jobID

re retrasst:
Don't worry, it will get done
would be nice not to have to see ay's ugly mug every day

wildpi:
not so much

buscopan

bombs out before getStorage(), hence all blank-x

---


plan query:

    SELECT    
        db.project_cid,    /*cbr.retrieval_cid, */
        cbr.section, 
        cbr.rj_box_cid, cbr.box_id, 
        lcr.position AS lcr_position, 
        lcr.cryovial_barcode, lcr.aliquot_type_cid, 
        /* lcr.old_box_cid, lcr.old_position, lcr.process_cid AS lcr_procid, lcr.status AS lcr_status, lcr.new_position, cs.box_cid, sb.external_name AS src_box, cs.cryovial_position AS source_pos,      db.external_name AS dest_box, */
        db.box_type_cid AS dest_box_type,     
        /* cs.note_exists, cs.cryovial_id, cs.cryovial_position, cs.status, c.sample_id, cs.record_id, */
        db.external_name AS dest_name  
    FROM     c_box_retrieval cbr, l_cryovial_retrieval lcr, c_box_name db, c_box_name sb, cryovial c, cryovial_store cs  
    WHERE     cbr.retrieval_cid   = 978253 
        AND cbr.rj_box_cid = lcr.rj_box_cid AND cbr.box_id = db.box_cid AND c.cryovial_barcode  = lcr.cryovial_barcode AND c.aliquot_type_cid = lcr.aliquot_type_cid AND cs.cryovial_id = c.cryovial_id 
        AND cbr.retrieval_cid = cs.retrieval_cid AND cs.box_cid = sb.box_cid  
    ORDER BY /*section, rj_box_cid, lcr_position */
        section, lcr_position


## how are we going to deal with multiple projects?

### try to create then load plan for contrived example (thrive and reveal)

250 vials: 
130 EDTA_1
120 EDTA_2

break into 4 chunks: 79, 51 (`EDTA_1`), 71, 49 (`EDTA_1`), save

load in collect:
thought it worked a couple of times! combinedCount is 120??
but now crashes with invalid project id 0, which it would be with a multi project job.
shelve multi projects for now?

load cbr, lcr into mem? then can do by proj

---

shower
nusol
SELECT 
    db.project_cid, /* cbr.retrieval_cid, */
    cbr.section, 
    lcr.position AS lcr_position, 
    cbr.rj_box_cid, 

    lcr.cryovial_barcode, lcr.aliquot_type_cid, 
    -- lcr.old_box_cid, lcr.old_position, lcr.process_cid AS lcr_procid, lcr.status AS lcr_status,      
    --// lcr.new_position AS dest_pos, // s2.cryovial_position as dest_pos
    -- cs.box_cid, 
    sb.external_name AS src_box, cs.cryovial_position AS source_pos,  
    lcr.new_position, db.external_name AS dest_box, cbr.box_id AS dest_box_id
    -- db.box_type_cid AS dest_box_type, 
    -- cs.note_exists, cs.cryovial_id, cs.cryovial_position, cs.status, c.sample_id, cs.record_id, 
    -- db.external_name AS dest_name 
FROM c_box_retrieval cbr, l_cryovial_retrieval lcr, c_box_name db, c_box_name sb, cryovial c, cryovial_store cs 

WHERE cbr.retrieval_cid = 978253 
AND cbr.rj_box_cid = lcr.rj_box_cid AND cbr.box_id = db.box_cid AND c.cryovial_barcode = lcr.cryovial_barcode 
AND c.aliquot_type_cid = lcr.aliquot_type_cid AND cs.cryovial_id = c.cryovial_id 
AND cbr.retrieval_cid = cs.retrieval_cid AND cs.box_cid = sb.box_cid 

ORDER BY section, lcr_position
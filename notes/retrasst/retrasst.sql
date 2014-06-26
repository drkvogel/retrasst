 SELECT g.retrieval_cid, g.chunk, g.rj_box_cid, g.cbr_status, g.dest_pos, g.lcr_slot, 
 g.lcr_procid, g.lcr_status, g.box_id AS dest_id,     
 c.cryovial_barcode, c.sample_id, c.aliquot_type_cid, c.note_exists AS cryovial_note,     
 s1.cryovial_id, s1.note_exists, s1.retrieval_cid, s1.box_cid, s1.status, s1.tube_position, s1.record_id,     
 s1.status, s1.tube_position, s1.note_exists AS cs_note,     
 b1.external_name AS src_box, b2.external_name AS dest_name, s2.tube_position AS slot_number, s2.status AS dest_status 
 FROM retrieval_assistant_temp_4145931_2014_01_16T16_22_09 g, cryovial c, cryovial_store s1, cryovial_store s2, box_name b1, box_name b2 
 WHERE     c.cryovial_barcode = g.cryovial_barcode     
 AND c.aliquot_type_cid = g.aliquot_type_cid     
 AND s1.cryovial_id = c.cryovial_id     
 AND s1.retrieval_cid = g.retrieval_cid     
 AND b2.box_cid = g.box_id     
 AND b1.box_cid = s1.box_cid     
 AND s2.cryovial_id = c.cryovial_id     
 AND b2.box_cid = s2.box_cid 
 ORDER BY     s1.retrieval_cid, chunk, g.rj_box_cid, c.aliquot_type_cid DESC


-- collect query

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

 SELECT 
    cbr.retrieval_cid, section AS chunk, cbr.rj_box_cid, 
    cs.box_cid, sb.external_name AS src_box, cs.tube_position AS source_pos,  
    cbr.box_id AS dest_id, db.external_name AS dest_box, slot_number AS dest_pos, 
    lcr.process_cid AS lcr_procid, lcr.status AS lcr_status, lcr.slot_number AS lcr_slot, lcr.cryovial_barcode, lcr.aliquot_type_cid, 
    cs.note_exists, cs.cryovial_id, cs.cryovial_position, cs.status, 
    c.sample_id, cs.record_id, 
    db.external_name AS dest_name 
 FROM 
    c_box_retrieval cbr, l_cryovial_retrieval lcr, c_box_name db, c_box_name sb, cryovial c, cryovial_store cs 
 WHERE 
    cbr.retrieval_cid   = -363636 AND 
    cbr.rj_box_cid      = lcr.rj_box_cid AND 
    cbr.box_id          = db.box_cid AND 
    c.cryovial_barcode  = lcr.cryovial_barcode AND 
    c.aliquot_type_cid  = lcr.aliquot_type_cid  AND 
    cs.cryovial_id      = c.cryovial_id  AND 
    cbr.retrieval_cid   = cs.retrieval_cid AND 
    cs.box_cid          = sb.box_cid 
 ORDER BY 
    chunk, source_pos, rj_box_cid, aliquot_type_cid     

 project -149662 (thrive), t_ldb20

  SELECT 
    cbr.retrieval_cid, section AS chunk, cbr.rj_box_cid, cbr.box_id AS dest_id, 
    lcr.position AS lcr_position, lcr.cryovial_barcode, lcr.aliquot_type_cid, 
    lcr.process_cid AS lcr_procid, lcr.status AS lcr_status, lcr.slot_number AS lcr_slot, 
    lcr.slot_number AS dest_pos
/*--    cs.box_cid, sb.external_name AS src_box, cs.cryovial_position AS source_pos,  
--    db.external_name AS dest_box, 
--    cs.note_exists, cs.cryovial_id, cs.cryovial_position, cs.status, 
--    c.sample_id, cs.record_id, 
--    db.external_name AS dest_name */
 FROM 
    c_box_retrieval cbr, l_cryovial_retrieval lcr
/*    c_box_name db, c_box_name sb, cryovial c, cryovial_store cs */
 WHERE 
    cbr.retrieval_cid = 1086650
    AND cbr.rj_box_cid = lcr.rj_box_cid 
/*--    AND cbr.box_id = db.box_cid 
--    AND c.cryovial_barcode = lcr.cryovial_barcode 
--    AND c.aliquot_type_cid = lcr.aliquot_type_cid
--    AND cs.cryovial_id = c.cryovial_id
--    AND cbr.retrieval_cid = cs.retrieval_cid
--    AND cs.box_cid = sb.box_cid */
 ORDER BY 
    chunk, rj_box_cid, lcr_position, aliquot_type_cid

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
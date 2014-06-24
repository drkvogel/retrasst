SELECT    
 cbr.retrieval_cid, 
 section AS chunk, cbr.rj_box_cid,     
 cs.box_cid, 
 sb.external_name AS src_box, 
 cs.tube_position AS source_pos,      
 cbr.box_id AS dest_id, 
 db.external_name AS dest_box, 
 slot_number AS dest_pos,     
 lcr.process_cid AS lcr_procid, 
 lcr.status AS lcr_status, 
 lcr.slot_number AS lcr_slot, 
 lcr.cryovial_barcode, 
 lcr.aliquot_type_cid,     
 cs.note_exists, cs.cryovial_id, cs.cryovial_position, cs.status,     
 c.sample_id, cs.record_id,
 db.external_name AS dest_name  
FROM     c_box_retrieval cbr, l_cryovial_retrieval lcr, c_box_name db, c_box_name sb, cryovial c, cryovial_store cs  
WHERE     cbr.retrieval_cid   = -363636 
AND     cbr.rj_box_cid      = lcr.rj_box_cid 
AND     cbr.box_id          = db.box_cid 
AND     c.cryovial_barcode  = lcr.cryovial_barcode 
AND     c.aliquot_type_cid  = lcr.aliquot_type_cid  
AND     cs.cryovial_id      = c.cryovial_id  
AND     cbr.retrieval_cid   = cs.retrieval_cid 
AND     cs.box_cid          = sb.box_cid  
AND     section = 1
ORDER BY     chunk, source_pos, rj_box_cid, aliquot_type_cid DESC
\g
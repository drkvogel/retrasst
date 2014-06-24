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
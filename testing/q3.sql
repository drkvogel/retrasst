SELECT
    cbr.rj_box_cid, cbr.box_id as dest_box, cbr.section as chunk, lcr.position,
    cbr.status as box_status, lcr.slot_number as cryo_slot, lcr.status as cryo_status,
    s1.cryovial_id, s1.note_exists, s1.retrieval_cid, s1.box_cid, s1.status, s1.tube_position, 
    s1.record_id, c.sample_id, c.aliquot_type_cid, c.cryovial_barcode, s1.tube_position as source_pos
FROM  
    c_box_retrieval cbr, l_cryovial_retrieval lcr,
    cryovial c, cryovial_store s1, box_name b1
WHERE    
    cbr.retrieval_cid = 2018 AND
    cbr.rj_box_cid = lcr.rj_box_cid AND
    lcr.cryovial_barcode =  c.cryovial_barcode AND lcr.aliquot_type_cid = c.aliquot_type_cid AND
    c.cryovial_id = s1.cryovial_id AND
    s1.status = 2 AND
    b1.box_cid = s1.box_cid
ORDER BY
    chunk, cbr.rj_box_cid, lcr.position
    
 -- no rows cause I haven't created a plan in retrasst yet!
 -- or have I? job 2018 looks 'in progress'
 -- 2018: a few...
 -- nothing in c_box_retrieval or l_cryovial_retrieval
 -- reset jobs and recreated plan - now 27 rows.... I thought there were 28?
 
SELECT
    cbr.rj_box_cid, 
    cbr.box_id as dest_box, 
    cbr.section as chunk,
    lcr.position,
    cbr.status as box_status,
    lcr.slot_number as cryo_slot,
    lcr.status as cryo_status,
    s1.cryovial_id, s1.note_exists, s1.retrieval_cid, s1.box_cid, s1.status, s1.tube_position, 
    s1.record_id, c.sample_id, c.aliquot_type_cid,  
    c.cryovial_barcode,
    s1.tube_position as source_pos
FROM  
    c_box_retrieval cbr, l_cryovial_retrieval lcr,
    cryovial c, cryovial_store s1, box_name b1
WHERE    
    cbr.retrieval_cid = 2018 AND
    cbr.rj_box_cid = lcr.rj_box_cid AND
    lcr.cryovial_barcode =  c.cryovial_barcode AND lcr.aliquot_type_cid = c.aliquot_type_cid AND
    c.cryovial_id = s1.cryovial_id AND
    s1.status = 2 AND
    b1.box_cid = s1.box_cid
ORDER BY
    chunk, cbr.rj_box_cid, lcr.position
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

 -- Nick's query
 -- Hackery of the highest order
 -- I have reorganised your query slightly and added a second box_name:

SELECT
    s1.retrieval_cid,cbr.section as chunk, lcr.position, cbr.rj_box_cid, cbr.status as cbr_status, 			/* where we are in the retrieval plan        */
    s1.cryovial_id,  c.cryovial_barcode, c.sample_id, c.aliquot_type_cid, c.note_exists as cryovial_note,   /* the cryovial we're trying to find         */
    s1.box_cid, b1.external_name as source_box, s1.status, s1.tube_position, s1.note_exists as cs_note, 	/* where that cryovial was before we started */
    cbr.box_id as dest_id,  b2.external_name as dest_name, lcr.slot_number, lcr.status as dest_status		/* where that cryovial is supposed to go     */
  FROM
    c_box_retrieval cbr, l_cryovial_retrieval lcr, cryovial c, cryovial_store s1, box_name b1, box_name b2
  WHERE    
    cbr.rj_box_cid = lcr.rj_box_cid AND
    lcr.cryovial_barcode =  c.cryovial_barcode AND lcr.aliquot_type_cid = c.aliquot_type_cid AND
    c.cryovial_id = s1.cryovial_id AND
    s1.retrieval_cid = cbr.retrieval_cid AND
    b1.box_cid = s1.box_cid AND
    b2.box_cid = cbr.box_id
  ORDER BY
    s1.retrieval_cid,chunk, lcr.position

  -- Maybe we can come up with a third version we can both understand!
  
  -- I thought I'd do a cross-check:

SELECT
    s1.retrieval_cid,cbr.section as chunk, cbr.rj_box_cid, cbr.status as cbr_status, 
    s1.cryovial_id,  c.cryovial_barcode, c.sample_id, c.aliquot_type_cid, c.note_exists as cryovial_note, 
    s1.box_cid, b1.external_name as source_box, s1.status, s1.tube_position, s1.note_exists as cs_note,
    cbr.box_id as dest_id,  b2.external_name as dest_name, s2.tube_position as slot_number, s2.status as dest_status
  FROM
    c_box_retrieval cbr, cryovial c, cryovial_store s1, box_name b1, cryovial_store s2, box_name b2
  WHERE    
    s1.retrieval_cid = cbr.retrieval_cid AND
    b2.box_cid = cbr.box_id AND
    c.cryovial_id = s1.cryovial_id AND
    c.cryovial_id = s2.cryovial_id AND
    b1.box_cid = s1.box_cid AND
    s2.box_cid = b2.box_cid
  ORDER BY
    s1.retrieval_cid,chunk, cbr.rj_box_cid

  -- but there's obviously something wrong - it ends up with the source and destination boxes being the same in t_ldb20_ddb   
  
  -- (me) now one too few vials in each chunk
  
  -- mine
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
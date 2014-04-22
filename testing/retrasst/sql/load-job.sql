SELECT
  s1.cryovial_id, s1.note_exists, s1.retrieval_cid, s1.box_cid, s1.status, s1.cryovial_position,
  s1.record_id, c.sample_id, c.aliquot_type_cid, 
  c.cryovial_barcode,
  b1.box_cid as source_id,
  b1.external_name as source_name,
		  s1.cryovial_position as source_pos,
  s2.box_cid as dest_id,
  b2.external_name as dest_name,
  s2.cryovial_position as dest_pos
 FROM
  cryovial c, cryovial_store s1, box_name b1,
  cryovial_store s2, box_name b2
 WHERE
  c.cryovial_id = s1.cryovial_id AND
  b1.box_cid = s1.box_cid AND
  s1.cryovial_id = s2.cryovial_id AND
  s2.status = 0 AND
  b2.box_cid = s2.box_cid AND
  s1.retrieval_cid = 
 ORDER BY
  cryovial_barcode, aliquot_type_cid
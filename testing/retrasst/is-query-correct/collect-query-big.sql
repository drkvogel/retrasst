SELECT    
cbr.retrieval_cid,
cbr.section,
cbr.box_id,
cbr.rj_box_cid,
cbr.status,
cbr.time_stamp,
cbr.project_cid,
lcr.rj_box_cid,
lcr.position,
lcr.cryovial_barcode,
lcr.aliquot_type_cid,
lcr.process_cid,
lcr.time_stamp,
lcr.status,
lcr.slot_number,
db.project_cid,
db.box_cid,
db.barcode,
db.box_type_cid,
db.box_capacity,
db.external_name,
db.process_cid,
db.status,
db.time_stamp,
sb.project_cid,
sb.box_cid,
sb.barcode,
sb.box_type_cid,
sb.box_capacity,
sb.external_name,
sb.process_cid,
sb.status,
sb.time_stamp,
c.cryovial_id,
c.sample_id,
c.cryovial_barcode,
c.aliquot_type_cid,
c.note_exists,
c.process_cid,
c.time_stamp,
c.status,
cs.record_id,
cs.cryovial_id,
cs.box_cid,
cs.cryovial_position,
cs.tube_position,
cs.time_stamp,
cs.removed,
cs.status,
cs.note_exists,
cs.process_cid,
cs.retrieval_cid,
cs.sample_volume
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
ORDER BY cbr.section, cs.tube_position, cbr.rj_box_cid, lcr.aliquot_type_cid DESC
\g
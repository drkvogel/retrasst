go.sh
  all rows: about 40s
  section = 1: about 12s

sql <db>/star # actually seems to break mintty terminal!

rows 2 and 3 look the same

these are different:

    lcr.position, # 'Where this entry appears in the relevant chunk of the retrieval plan - samples are retrieved in this order'
    lcr.time_stamp, # 25/02/2014 15:15 - yesterday. 

Reset jobs
Load job shows 1674 = 2 * 837 vials found (primary and secondary) - correct
 * Usual sort order before chunking: destination box and position
 * Usual sort order for each chunk: vessel position, shelf, structure, slot and current position
 should do one test in usual sort order, one jumbled u

not sure if now correct but loads up previously saved rows, inc secondaries, looks ok
need to skip forwards now

from 
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


SELECT     
cbr.retrieval_cid, section AS chunk, cbr.rj_box_cid,     
cs.box_cid, sb.external_name AS src_box, cs.tube_position AS source_pos,      
cbr.box_id AS dest_id, db.external_name AS dest_box, slot_number AS dest_pos,     
lcr.process_cid AS lcr_procid, lcr.status AS lcr_status, lcr.slot_number AS lcr_slot, 
lcr.cryovial_barcode, lcr.aliquot_type_cid,     
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
chunk, source_pos, rj_box_cid, aliquot_type_cid DESC    
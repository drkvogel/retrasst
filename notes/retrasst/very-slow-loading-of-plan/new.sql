﻿SELECT 
    section, position, cbr.rj_box_cid, cbr.status, 
    cs.box_cid AS source_id, sb.external_name AS source_box, cs.tube_position AS source_pos,  
    cbr.box_id AS dest_id, db.external_name AS dest_box, slot_number AS dest_pos, 
    lcr.process_cid, lcr.status, lcr.cryovial_barcode, lcr.aliquot_type_cid 
FROM 
    c_box_retrieval cbr, l_cryovial_retrieval lcr, c_box_name db, c_box_name sb, cryovial c, cryovial_store cs 
WHERE 
    cbr.rj_box_cid      = lcr.rj_box_cid AND 
    cbr.box_id          = db.box_cid AND 
    c.cryovial_barcode  = lcr.cryovial_barcode AND 
    c.aliquot_type_cid  = lcr.aliquot_type_cid  AND 
    cs.cryovial_id      = c.cryovial_id  AND 
    cbr.retrieval_cid   = cs.retrieval_cid AND 
    cs.box_cid          = sb.box_cid AND 
    cbr.retrieval_cid   = :rtid
ORDER BY 
    section, position

-- This still takes getting on for a minute.  Since the slowdown seems to appear when you add in the link to cryovial_store, I suspect your version is slower because it does that twice.

-- Copying the relevant data into the central database allows a simpler query:
-- (The necessary fields will be implemented in the next database version)

SELECT 
    section, position, cbr.rj_box_cid, cbr.status, 
    old_box_id AS source_id, sb.external_name AS source_box, old_position AS source_pos, 
    cbr.box_id AS dest_id, db.external_name AS dest_box, new_position AS dest_pos, 
    lcr.process_cid, lcr.status, lcr.cryovial_barcode, lcr.aliquot_type_cid 
FROM 
    c_box_retrieval cbr, l_cryovial_retrieval  lcr, c_box_name db, c_box_name sb 
WHERE
    cbr.rj_box_cid = lcr.rj_box_cid AND 
    cbr.box_id = db.box_cid AND 
    old_box_id = sb.box_cid AND 
    cbr.retrieval_cid = :rtid 
ORDER BY 
    section, position

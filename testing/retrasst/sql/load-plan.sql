 SELECT 
            cbr.retrieval_cid, section AS chunk, cbr.rj_box_cid, cbr.box_id AS dest_id, 
            lcr.position AS lcr_position, lcr.cryovial_barcode, lcr.aliquot_type_cid, 
            lcr.process_cid AS lcr_procid, lcr.status AS lcr_status, lcr.slot_number AS lcr_slot, 
            lcr.slot_number AS dest_pos, 
            cs.box_cid, sb.external_name AS src_box, cs.cryovial_position AS source_pos,  
            db.external_name AS dest_box, 
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
            cs.box_cid          = sb.box_cid AND 
            db.status != 99 AND sb.status != 99
         ORDER BY 
            chunk, rj_box_cid, lcr_position, aliquot_type_cid 
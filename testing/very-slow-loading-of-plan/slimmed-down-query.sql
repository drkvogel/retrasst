/* sql vnode_vlab::t_ldb20_ddb/star < <name>.sql. Encode in ANSI to avoid E_US0A94 Unexpected character. */

set qep;
Set optimizeonly;
\p\g
SELECT
    s1.retrieval_cid, cbr.section as chunk, cbr.rj_box_cid, cbr.status as cbr_status,
    lcr.position as dest_pos, lcr.slot_number as lcr_slot, lcr.process_cid as lcr_procid, lcr.status as lcr_status,
    s1.cryovial_id, s1.note_exists, s1.retrieval_cid, s1.box_cid, s1.status, s1.tube_position, 
    s1.record_id, c.cryovial_barcode, c.sample_id, c.aliquot_type_cid, c.note_exists as cryovial_note,
    s1.box_cid, b1.external_name as src_box, s1.status, s1.tube_position, s1.note_exists as cs_note,
    cbr.box_id as dest_id, b2.external_name as dest_name, s2.tube_position as slot_number, s2.status as dest_status
 FROM
    c_box_retrieval cbr, l_cryovial_retrieval lcr, cryovial c, cryovial_store s1, box_name b1, cryovial_store s2, box_name b2
 WHERE
    cbr.retrieval_cid = -1015 AND
    s1.retrieval_cid = cbr.retrieval_cid AND
    lcr.rj_box_cid = cbr.rj_box_cid AND
    lcr. cryovial_barcode = c.cryovial_barcode AND lcr.aliquot_type_cid = c.aliquot_type_cid AND
    b2.box_cid = cbr.box_id AND
    /*t.object_cid = c.aliquot_type_cid AND*/
    c.cryovial_id = s1.cryovial_id AND
    c.cryovial_id = s2.cryovial_id AND
    b1.box_cid = s1.box_cid AND
    s2.box_cid = b2.box_cid
 ORDER BY
    s1.retrieval_cid, chunk, cbr.rj_box_cid, lcr.position
\p\g
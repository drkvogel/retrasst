 SELECT
    s1.retrieval_cid,cbr.section as chunk, cbr.rj_box_cid, lcr.position as dest_pos, cbr.status as cbr_status,
    s1.cryovial_id, s1.note_exists, s1.retrieval_cid, s1.box_cid, s1.status, s1.tube_position, // for LPDbCryovialStore
    s1.record_id, c.cryovial_barcode, c.sample_id, c.aliquot_type_cid, c.note_exists as cryovial_note,
    s1.box_cid, b1.external_name as source_box, s1.status, s1.tube_position, s1.note_exists as cs_note,
    cbr.box_id as dest_id, b2.external_name as dest_name, s2.tube_position as slot_number, s2.status as dest_status
 FROM
    c_box_retrieval cbr, l_cryovial_retrieval lcr, cryovial c, cryovial_store s1, box_name b1, cryovial_store s2, box_name b2
 WHERE
    cbr.retrieval_cid = 2018 AND
    s1.retrieval_cid = cbr.retrieval_cid AND
    lcr.rj_box_cid = cbr.rj_box_cid AND
    lcr. cryovial_barcode = c.cryovial_barcode AND lcr.aliquot_type_cid = c.aliquot_type_cid AND
    b2.box_cid = cbr.box_id AND
    c.cryovial_id = s1.cryovial_id AND
    c.cryovial_id = s2.cryovial_id AND
    b1.box_cid = s1.box_cid AND
    s2.box_cid = b2.box_cid
 ORDER BY
    s1.retrieval_cid,chunk, cbr.rj_box_cid, lcr.position
    
 -- firstly, the number of vials in each chunk is wrong by 1 too few. as dest_pos = 1, 6, 11, etc is missing, it looks like the first one in each chunk
 -- fixed that: i=1, what was I thinking of (from when things were 1-indexed), 
 
 -- but otherwise, the query looks correct - the vials are missing from the l_cryovial_retrieval, so the plan was incorrect
 
 -- pos 1 is still not shown, and the chunks are a sum of all preceding chunks - showChunk is now nearly identical to that in frmSamples, so may be error earlier
 
 -- plan shown by: select * from c_box_retrieval cbr, l_cryovial_retrieval lcr where cbr.rj_box_cid = lcr.rj_box_cid
 
 -- 'sections' (chunks) go 1, 2, 3...
 
 -- Chunk(StringGridWrapper< T > * w, int sc, int s, int e) : sgw(w), section(sc), start(s), end(e) { }
 
 -- and the end of the last chunk wasn't set - now done outside thread, no, inside thread using friend
 
 -- now: chunks don't display box, and list doesn't display aliquot, current box
-- TfrmProcess
    -- orig

SELECT
   *
 FROM
   c_box_retrieval b, l_cryovial_retrieval c
 WHERE
   retrieval_cid = 2018 AND
   b.rj_box_cid = c.rj_box_cid 
 ORDER BY
   b.section, b.rj_box_cid, c.position
   
    -- ?
/*
Rosetta error: ROSETTA Error: member "dest_pos" not found
Rosetta error: ROSETTA Error: member "dest_name" not found
Rosetta error: ROSETTA Error: member "dest_id" not found
Rosetta error: ROSETTA Error: member "source_name" not found
Rosetta error: ROSETTA Error: member "aliquot" not found
Rosetta error: ROSETTA Error: member "Cryovial_id" not found
Rosetta error: ROSETTA Error: member "Note_Exists" not found
Rosetta error: ROSETTA Error: member "cryovial_id" not found
Rosetta error: ROSETTA Error: member "box_cid" not found
Rosetta error: ROSETTA Error: member "cryovial_position" not found
Rosetta error: ROSETTA Error: member "Cryovial_id" not found
Rosetta error: ROSETTA Error: member "Note_Exists" not found
Rosetta error: ROSETTA Error: member "box_cid" not found
Rosetta error: ROSETTA Error: member "sample_id" not found
Rosetta error: ROSETTA Error: member "record_id" not found
Rosetta error: ROSETTA Error: member "cryovial_position" not found'.*/    

/* 
### c_box_retrieval

rj_box_cid      1		            Unique ID for this retrieval list entry (also determines retrieval order for box retrievals)
retrieval_cid       c_retrieval_job The retrieval task this entry is part of
box_id              box_name        The box being retrieved (for box retrieval/disposal) or retrieved into (for sample retrieval/disposal)
project_cid                         The project the destination box belongs to (assumes cryovials in destination box are all from the same project; should match project_cid in c_retrieval_job if task includes samples from only one project)
section                             Which chunk of the retrieval plan this entry belongs to (0 = no chunks: retrieve all boxes in parallel)
status                              0: new record; 1: part-filled, 2: collected; 3: not found; 99: record deleted
time_stamp                          When this record was inserted or updated

### l_cryovial_retrieval

rj_box_cid      1   c_box_retrieval box the sample should be placed into
position	    2		            Where sample appears in [corresponding section of] retrieval plan
cryovial_barcode	                barcode on the sample    
aliquot_type_cid	c_object_name   The aliquot type (cryovial_barcode + aliquot_type_cid should uniquely identify the cryovial within the project)
slot_number                         expected pos of vial in dest box (if 2 recs suggest same pos in same box, 1st should be primary aliquot; 2nd will be ignored if 1st is found)
process_cid	        c_audit_trail	Who stored it, which program etc.
status	                            0: expected; 1: ignored, 2: collected; 3: not found; 99: record deleted
time_stamp

-- cryovial_barcode, aliquot_type_cid, slot_number *cannot* be replaced by cryovial_id 
-- because cryovial_id is not unique between projects. could link to c_project,
-- easier to replicate and key on cryovial_barcode and aliquot_type_cid, which is unique

s1.cryovial_id, s1.note_exists, s1.retrieval_cid, s1.box_cid, s1.status, s1.tube_position," // for LPDbCryovialStore
s1.record_id, c.sample_id, c.aliquot_type_cid, " // for LPDbCryovial

*/

-- new query to include details needed by LPDbCryovial, LPDbCryovialStore

/* This broke t_ldbc20_ddb - cart prod of 8 tables with only two join conditions produced
        E_US1263 Error allocating file system resource[...] ie. ran out of space trying to order by chunk
   ps. cmdline sql switch for distributed (star) database:  $ sql vnode_vlab::t_ldb20_ddb/star               
   without switch you get E_DM012A The database configuration file was not found. */

  s1.cryovial_id, s1.note_exists, s1.retrieval_cid, s1.box_cid, s1.status, s1.tube_position, -- for LPDbCryovialStore
  s1.record_id, c.sample_id, c.aliquot_type_cid,  -- for LPDbCryovial
  c.cryovial_barcode, 
  t.external_name AS aliquot,
  b1.box_cid as source_id,
  b1.external_name as source_name,
  s1.tube_position as source_pos,
  s2.box_cid as dest_id,
  b2.external_name as dest_name,
  s2.tube_position as dest_pos   
   
SELECT
  br.rj_box_cid as dest_box, br.section as chunk,
  s1.cryovial_id, s1.note_exists, s1.retrieval_cid, s1.box_cid, s1.status, s1.tube_position, 
  s1.record_id, c.sample_id, c.aliquot_type_cid,  
  c.cryovial_barcode, t.external_name AS aliquot,
  b1.box_cid as source_id,
  b1.external_name as source_name,
  s1.tube_position as source_pos,
  --s2.box_cid as dest_id,
  --b2.external_name as dest_name,
  --s2.tube_position as dest_pos
FROM  
  cryovial c, cryovial_store s1, box_name b1,
  c_box_retrieval br, l_cryovial_retrieval cr,
  --cryovial_store s2, box_name b2,
  c_object_name t
WHERE
    -- don't have cryovial_id in l_cryovial_retrieval - join on barcode and aliquot_type
  --c.cryovial_id = s1.cryovial_id AND
  b1.box_cid = s1.box_cid AND
  --s1.cryovial_id = s2.cryovial_id AND
  s2.status = 0 AND
  b2.box_cid = s2.box_cid AND
  t.object_cid = aliquot_type_cid AND
  br.rj_box_cid = cr.rj_box_cid AND
  br.retrieval_cid = 2018
ORDER BY
  -- br.section, br.rj_box_cid, cr.position    
  chunk
   
-- TfrmSamples
   
SELECT
  s1.cryovial_id, s1.note_exists, s1.retrieval_cid, s1.box_cid, s1.status, s1.tube_position, -- for LPDbCryovialStore
  s1.record_id, c.sample_id, c.aliquot_type_cid,  -- for LPDbCryovial
            // LPDbCryovial::storeID( query.readInt( record_id ) ) <-- record_id comes from cryovial_store?
  c.cryovial_barcode, t.external_name AS aliquot,
  b1.box_cid as source_id,
  b1.external_name as source_name,
  s1.tube_position as source_pos,
  s2.box_cid as dest_id,
  b2.external_name as dest_name,
  s2.tube_position as dest_pos
 FROM
  cryovial c, cryovial_store s1, box_name b1,
  cryovial_store s2, box_name b2,
  c_object_name t
 WHERE
  c.cryovial_id = s1.cryovial_id AND
  b1.box_cid = s1.box_cid AND
  s1.cryovial_id = s2.cryovial_id AND
  s2.status = 0 AND
  b2.box_cid = s2.box_cid AND
  t.object_cid = aliquot_type_cid AND
  s1.retrieval_cid = 2018
 ORDER BY
  cryovial_barcode
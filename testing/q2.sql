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

/* c_box_retrieval
retrieval_cid	
#retrieval_type	
box_id	
section		
#position	?
#box_name	
rj_box_cid	
status	 		
time_stamp

l_cryovial_retrieval
rj_box_cid	 i4	 1	 c_box_retrieval	 The box the sample should be placed into
position	 i2	 2		 Where this sample appears in [the corresponding section of] the retrieval plan

    -- following 3 *cannot* be replaced by cryovial_id because cryovial_id is not unique between projects. could make sure by linking to c_project but 
    -- easier to replicate these and key on cryovial_barcode and aliquot_type_cid, which is unique
    cryovial_barcode	 v64			 The barcode on the sample    
    aliquot_type_cid	 i4		 c_object_name	 The aliquot type (cryovial_barcode + aliquot_type_cid should uniquely identify the cryovial within the project)
    slot_number	 i2		expected pos of vial in dest box (if 2 recs suggest same pos in same box, 1st should be primary aliquot; 2nd will be ignored if 1st is found)
process_cid	 i4		 c_audit_trail	 Who stored it, which program etc.
status	 i2			 0: expected; 1: ignored, 2: collected; 3: not found; 99: record deleted
time_stamp

s1.cryovial_id, s1.note_exists, s1.retrieval_cid, s1.box_cid, s1.status, s1.tube_position," // for LPDbCryovialStore
s1.record_id, c.sample_id, c.aliquot_type_cid, " // for LPDbCryovial

*/

-- new query to include details needed by LPDbCryovial, LPDbCryovialStore
SELECT
  rj_box_cid as dest_box, 
  s1.cryovial_id, s1.note_exists, s1.retrieval_cid, s1.box_cid, s1.status, s1.tube_position, -- for LPDbCryovialStore
  s1.record_id, c.sample_id, c.aliquot_type_cid,  -- for LPDbCryovial
  c.cryovial_barcode, t.external_name AS aliquot,
  b1.box_cid as source_id,
  b1.external_name as source_name,
  s1.tube_position as source_pos,
  s2.box_cid as dest_id,
  b2.external_name as dest_name,
  s2.tube_position as dest_pos
FROM
  c_box_retrieval br, l_cryovial_retrieval cr
  cryovial c, cryovial_store s1, box_name b1,
  cryovial_store s2, box_name b2,
  c_object_name t
WHERE
  
  br.rj_box_cid = cr.rj_box_cid AND
  retrieval_cid = 2018
ORDER BY
  b.section, b.rj_box_cid, c.position    
   
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
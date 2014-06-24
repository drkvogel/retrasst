SELECT     
	cbr.retrieval_cid, 
	section AS chunk, 
	cbr.rj_box_cid, cbr.status,     
	cs.box_cid AS source_id, 
	sb.external_name AS source_box, 
	cs.tube_position AS source_pos,      
	cbr.box_id AS dest_id, 
	db.external_name AS dest_box, 
	slot_number AS dest_pos,     
	lcr.process_cid, 
	lcr.status, 
	lcr.cryovial_barcode, 
	lcr.aliquot_type_cid 
FROM     
	c_box_retrieval cbr, 
	l_cryovial_retrieval lcr, 
	c_box_name db, 
	c_box_name sb, 
	cryovial c, 
	cryovial_store cs 
WHERE     
	cbr.rj_box_cid      = lcr.rj_box_cid AND     
	cbr.box_id          = db.box_cid AND     
	c.cryovial_barcode  = lcr.cryovial_barcode AND     
	c.aliquot_type_cid  = lcr.aliquot_type_cid  AND     
	cs.cryovial_id      = c.cryovial_id  AND     
	cbr.retrieval_cid   = cs.retrieval_cid AND     
	cs.box_cid          = sb.box_cid AND     
	cbr.retrieval_cid   = :rtid ORDER BY     chunk, position, cbr.rj_box_cid, c.aliquot_type_cid DESC

SELECT     cbr.retrieval_cid, section AS chunk, cbr.rj_box_cid, cbr.status,     cs.box_cid AS source_id, sb.external_name AS source_box, cs.tube_position AS source_pos,      cbr.box_id AS dest_id, db.external_name AS dest_box, slot_number AS dest_pos,     lcr.process_cid, lcr.status, lcr.cryovial_barcode, lcr.aliquot_type_cid FROM     c_box_retrieval cbr, l_cryovial_retrieval lcr, c_box_name db, c_box_name sb, cryovial c, cryovial_store cs WHERE     cbr.rj_box_cid      = lcr.rj_box_cid AND     cbr.box_id          = db.box_cid AND     c.cryovial_barcode  = lcr.cryovial_barcode AND     c.aliquot_type_cid  = lcr.aliquot_type_cid  AND     cs.cryovial_id      = c.cryovial_id  AND     cbr.retrieval_cid   = cs.retrieval_cid AND     cs.box_cid          = sb.box_cid AND     cbr.retrieval_cid   = :rtid ORDER BY     chunk, position, cbr.rj_box_cid, c.aliquot_type_cid DESC

SELECT     cbr.retrieval_cid, section AS chunk, cbr.rj_box_cid, cbr.status,     cs.box_cid AS source_id, sb.external_name AS source_box, cs.tube_position AS source_pos,      cbr.box_id AS dest_id, db.external_name AS dest_box, slot_number AS dest_pos,     lcr.process_cid, lcr.status, lcr.cryovial_barcode, lcr.aliquot_type_cid FROM     c_box_retrieval cbr, l_cryovial_retrieval lcr, c_box_name db, c_box_name sb, cryovial c, cryovial_store cs WHERE     cbr.retrieval_cid   = :rtid AND     cbr.rj_box_cid      = lcr.rj_box_cid AND     cbr.box_id          = db.box_cid AND     c.cryovial_barcode  = lcr.cryovial_barcode AND     c.aliquot_type_cid  = lcr.aliquot_type_cid  AND     cs.cryovial_id      = c.cryovial_id  AND     cbr.retrieval_cid   = cs.retrieval_cid AND     cs.box_cid          = sb.box_cid ORDER BY     chunk, source_pos, rj_box_cid, aliquot_type_cid DESC

/*
---------------------------
Debugger Exception Notification
---------------------------
Project Storeman.exe raised exception class Exception with message 'XDB error: unsupported parameter type
XDB error: IIAPI_ST_FAILUREInfo: ERROR '5000R' 13172743: Query cannot be closed in the current state, it may only be cancelled.'.
---------------------------
Break   Continue   Help   
---------------------------

---------------------------
Debugger Exception Notification
---------------------------
Project Storeman.exe raised exception class Exception with message 'Rosetta error: ROSETTA Error: member "dest_name" not found
Rosetta error: ROSETTA Error: member "status" not found
Rosetta error: ROSETTA Error: member "cryovial_position" not found
Rosetta error: ROSETTA Error: member "sample_id" not found
Rosetta error: ROSETTA Error: member "record_id" not found
Rosetta error: ROSETTA Error: member "status" not found
Rosetta error: ROSETTA Error: member "cryovial_position" not found'.
---------------------------
Break   Continue   Help   
---------------------------



*/
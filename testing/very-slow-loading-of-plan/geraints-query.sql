CREATE TABLE my_extra_table AS
SELECT 
    cbr.rj_box_cid, cbr.retrieval_cid, cbr.section chunk, cbr.status cbr_status, cbr.box_id,
    lcr.position dest_pos, lcr.slot_number lcr_slot, lcr.process_cid lcr_procid, lcr.status lcr_status,
    lcr.cryovial_barcode, lcr.aliquot_type_cid
FROM 
    c_box_retrieval cbr, l_cryovial_retrieval lcr
WHERE 
    cbr.retrieval_cid=-1015
AND
    lcr.rj_box_cid=cbr.rj_box_cid;

COMMIT; 

SELECT
    s1.retrieval_cid, g.chunk, g.rj_box_cid, g.cbr_status, g.dest_pos, g.lcr_slot, g.lcr_procid, g.lcr_status,
    s1.cryovial_id, s1.note_exists, s1.retrieval_cid, s1.box_cid, s1.status, s1.tube_position,
    s1.record_id, c.cryovial_barcode, c.sample_id, c.aliquot_type_cid, c.note_exists AS cryovial_note,
    s1.box_cid, b1.external_name AS src_box, s1.status, s1.tube_position, s1.note_exists AS cs_note,
    g.box_id AS dest_id, b2.external_name AS dest_name, s2.tube_position AS slot_number, s2.status AS dest_status
FROM
    my_extra_table g, cryovial c, cryovial_store s1, cryovial_store s2, box_name b1, box_name b2
WHERE 
    c.cryovial_barcode = g.cryovial_barcode
    AND c.aliquot_type_cid = g.aliquot_type_cid
    AND s1.cryovial_id = c.cryovial_id
    AND s1.retrieval_cid = g.retrieval_cid
    AND b2.box_cid = g.box_id     
    AND b1.box_cid = s1.box_cid     
    AND s2.cryovial_id =c.cryovial_id
    AND b2.box_cid = s2.box_cid
ORDER BY 
    s1.retrieval_cid,chunk, g.rj_box_cid, dest_pos;
    
DROP my_extra_table;

-- load         2018    -1015
-- rows         28      4000        
-- before       30s     25m (1500s)
--  s/row       1.0     0.375
-- after (peak) 8s      failed with exceptions below
--  s/row       0.28    n/a
--  increase    3.5     n/a
-- after (off)  5s      failed
--  s/row       0.17    (255s/4m15s?)
--  increase    6       n/a

-- db performance graph meter?
-- ssh lust@vlab; top;
/*
---------------------------
Debugger Exception Notification
---------------------------
Project Storeman.exe raised exception class Exception with message 'XDB error: IIAPI_ST_ERRORInfo: ERROR '42500' 2117: Table 'z902911569' does not exist or is not owned by you.
 svr_id_error     = 67653632
 svr_local error  = 2117
 svr_id_server    = 2911
 svr_server_type  = 0
 svr_severity     = IIAPI_SVR_DEFAULT ( 0x0 )
	: 'Thu Nov 21 16:04:56 2013 E_US0845_2117	Table 'z902911569' does not exist or is not owned by you.'WARNING '01000' 2117: Preceding message is from LDB=t_ldb20,  NODE=vlab.ctsu.ox.ac.uk, DBMS=INGRES
 svr_id_error     = 262144
 svr_local error  = 2117
 svr_id_server    = 2911
 svr_server_type  = 0
 svr_severity     = IIAPI_SVR_WARNING ( 0x2 )
	: 'Thu Nov 21 16:04:56 2013 E_RQ0042_LDB_ERROR_MSG	Preceding message is from LDB=t_ldb20,  NODE=vlab.ctsu.ox.ac.uk, DBMS=INGRES'ERROR '50000' 394564: RQF detected a CURSOR OPEN failure.
 svr_id_error     = 83886080
 svr_local error  = 394564
 svr_id_server    = 2911
 svr_server_type  = 0
 svr_severity     = IIAPI_SVR_DEFAULT ( 0x0 )
	: 'Thu Nov 21 16:04:56 2013 E_QE0544_CURSOR_OPEN_FAILED	RQF detected a CURSOR OPEN failure.'
XDB error: IIAPI_ST_ERRORInfo: ERROR '24000' 590366: Specified cursor is not known to the server.
 svr_id_error     = 34603008
 svr_local error  = 590366
 svr_id_server    = 2911
 svr_server_type  = 0
 svr_severity     = IIAPI_SVR_DEFAULT ( 0x0 )
	: 'Thu Nov 21 16:04:56 2013 E_SC021E_CURSOR_NOT_FOUND	Specified cursor is not known to the server.''.
---------------------------
Break   Continue   Help   
---------------------------
*/
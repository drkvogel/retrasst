Very slow loading of retrieval plan

Loading -1015 Thrive Lp(a) Analysis takes many minutes and eventually throws an Ingres error (shown in full below). 

2018 "A few THRIVE..." takes about 30 seconds but works OK. Suspect size of QEP for -1015 is too big for server.

Precede the query with:
Set qep;
Set optimizeonly;
\p\g
<your query>
\p\g


 SELECT
    s1.retrieval_cid, cbr.section as chunk, cbr.rj_box_cid, cbr.status as cbr_status,
    lcr.position as dest_pos, lcr.slot_number as lcr_slot, lcr.process_cid as lcr_procid, lcr.status as lcr_status,
    s1.cryovial_id, s1.note_exists, s1.retrieval_cid, s1.box_cid, s1.status, s1.tube_position, 
    s1.record_id, c.cryovial_barcode, c.sample_id, c.aliquot_type_cid, c.note_exists as cryovial_note,
    s1.box_cid, b1.external_name as src_box, s1.status, s1.tube_position, s1.note_exists as cs_note,
    cbr.box_id as dest_id, b2.external_name as dest_name, s2.tube_position as slot_number, s2.status as dest_status
 FROM
    c_box_retrieval cbr, l_cryovial_retrieval lcr, cryovial c, cryovial_store s1, box_name b1, cryovial_store s2, box_name b2,
    c_object_name t
 WHERE
    cbr.retrieval_cid = -1015 AND
    s1.retrieval_cid = cbr.retrieval_cid AND
    lcr.rj_box_cid = cbr.rj_box_cid AND
    lcr. cryovial_barcode = c.cryovial_barcode AND lcr.aliquot_type_cid = c.aliquot_type_cid AND
    b2.box_cid = cbr.box_id AND
    t.object_cid = c.aliquot_type_cid AND
    c.cryovial_id = s1.cryovial_id AND
    c.cryovial_id = s2.cryovial_id AND
    b1.box_cid = s1.box_cid AND
    s2.box_cid = b2.box_cid
 ORDER BY
    s1.retrieval_cid,chunk, cbr.rj_box_cid, lcr.position
    
    
---------------------------
Debugger Exception Notification
---------------------------
Project Storeman.exe raised exception class Exception with message 'XDB error: IIAPI_ST_ERRORInfo: ERROR '42500' 2117: Table 'z379009422' does not exist or is not owned by you.
 svr_id_error     = 67653632
 svr_local error  = 2117
 svr_id_server    = 2911
 svr_server_type  = 0
 svr_severity     = IIAPI_SVR_DEFAULT ( 0x0 )
	: 'Wed Nov 20 14:52:24 2013 E_US0845_2117	Table 'z379009422' does not exist or is not owned by you.'WARNING '01000' 2117: Preceding message is from LDB=t_ldb20,  NODE=vlab.ctsu.ox.ac.uk, DBMS=INGRES
 svr_id_error     = 262144
 svr_local error  = 2117
 svr_id_server    = 2911
 svr_server_type  = 0
 svr_severity     = IIAPI_SVR_WARNING ( 0x2 )
	: 'Wed Nov 20 14:52:24 2013 E_RQ0042_LDB_ERROR_MSG	Preceding message is from LDB=t_ldb20,  NODE=vlab.ctsu.ox.ac.uk, DBMS=INGRES'ERROR '42500' 2117: Table 'z379010422' does not exist or is not owned by you.
 svr_id_error     = 67653632
 svr_local error  = 2117
 svr_id_server    = 2911
 svr_server_type  = 0
 svr_severity     = IIAPI_SVR_DEFAULT ( 0x0 )
	: 'Wed Nov 20 14:52:24 2013 E_US0845_2117	Table 'z379010422' does not exist or is not owned by you.'WARNING '01000' 2117: Preceding message is from LDB=t_ldbc,  NODE=vlab.ctsu.ox.ac.uk, DBMS=INGRES
 svr_id_error     = 262144
 svr_local error  = 2117
 svr_id_server    = 2911
 svr_server_type  = 0
 svr_severity     = IIAPI_SVR_WARNING ( 0x2 )
	: 'Wed Nov 20 14:52:24 2013 E_RQ0042_LDB_ERROR_MSG	Preceding message is from LDB=t_ldbc,  NODE=vlab.ctsu.ox.ac.uk, DBMS=INGRES'ERROR '50000' 394564: RQF detected a CURSOR OPEN failure.
 svr_id_error     = 83886080
 svr_local error  = 394564
 svr_id_server    = 2911
 svr_server_type  = 0
 svr_severity     = IIAPI_SVR_DEFAULT ( 0x0 )
	: 'Wed Nov 20 14:52:24 2013 E_QE0544_CURSOR_OPEN_FAILED	RQF detected a CURSOR OPEN failure.'
XDB error: IIAPI_ST_ERRORInfo: ERROR '24000' 590366: Specified cursor is not known to the server.
 svr_id_error     = 34603008
 svr_local error  = 590366
 svr_id_server    = 2911
 svr_server_type  = 0
 svr_severity     = IIAPI_SVR_DEFAULT ( 0x0 )
	: 'Wed Nov 20 14:52:24 2013 E_SC021E_CURSOR_NOT_FOUND	Specified cursor is not known to the server.''.
---------------------------
Break   Continue   Help   
---------------------------

    ---------------------------
    Debugger Exception Notification
    ---------------------------
    Project Storeman.exe raised exception class Exception with message 'XDB error: IIAPI_ST_ERRORInfo: ERROR '42500' 2117: Table 'c_box_name' does not exist or is not owned by you.
     svr_id_error     = 67653632
     svr_local error  = 2117
     svr_id_server    = 10154
     svr_server_type  = 0
     svr_severity     = IIAPI_SVR_DEFAULT ( 0x0 )
        : 'Tue Jun  3 14:36:33 2014 E_US0845_2117   Table 'c_box_name' does not exist or is not owned by you.''.
    ---------------------------
    Break   Continue   Help   
    ---------------------------

`LPDbBoxNames::readRecord()` now reads from `c_box_name` and not `box_name`.


    { { "proj name: dev_hps2-thrive, id: -149662, db: t_ldb20", 5, {  } } }

    SELECT  
    b1.project_cid,  
    s1.cryovial_id, s1.note_exists, s1.retrieval_cid, s1.box_cid, s1.status, 
    s1.cryovial_position,  s1.record_id, 
    c.sample_id, c.aliquot_type_cid,   c.cryovial_barcode,  
    b1.box_cid as source_id,  b1.external_name as source_name,  
    s1.cryovial_position as source_pos,  s2.box_cid as dest_box_id,  
    b2.external_name as dest_box_name,  b2.box_type_cid as dest_box_type,   
    s2.cryovial_position as dest_pos 
    FROM  cryovial c, cryovial_store s1, c_box_name b1,  cryovial_store s2, box_name b2 
    WHERE  c.cryovial_id = s1.cryovial_id 
    AND  b1.box_cid = s1.box_cid 
    AND  s1.cryovial_id = s2.cryovial_id 
    AND  s2.status = 0 
    AND  b2.box_cid = s2.box_cid 
    AND  s1.retrieval_cid = :jobID

re retrasst:
Don't worry, it will get done
would be nice not to have to see ay's ugly mug every day

wildpi:
not so much

buscopan

getstorage use LQuery q(LIMSDatabase::getCentralDb())?

bombs out before getStorage(), hence all blank
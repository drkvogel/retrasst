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

`getStorage()`: use LQuery q(LIMSDatabase::getCentralDb())?

bombs out before getStorage(), hence all blank-x

## no storage for contrived secondaries

no records found for secondaries of `979124 "Retrieval_1", "A contrived example mixing THRIVE and REVEAL" primary: [-31781] EDTA_1 secondary: [0] Not specified`

`getStorage()`
`findBox()` <findBox.sql>

e.g. box -623955: no results for `select * from c_slot_allocation where box_cid = -623955` - well there ain't no storage records it seems, so printing "no records found" would seem to be correct.

in collect, sorting into primary and secondary (which is ok in plan), ruins the order of the plan. combineAliquots() could perhaps do its thing in a different way on one vector

sample::debug_str() says proj is 0 for e.g. from box -623955:

    03/06/2014 18:46:04: id: 378304, proj: 0, status: 2, barc: "112089327", aliq: -31782 "EDTA_2", cryo_status: 2, src: {-623955, "EDTAs 10_623955" [29]}, dst: {-624094 "EDTA1_2 1_624094" [50], type: 978201 "QClot_new"}, loc: {No records found[0]: :0[0]/[0]}

no project id shouldn't affect `getStorage()`/`findBox()` , but..
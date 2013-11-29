# Very slow loading of retrieval plan

## Abstract

Loading -1015 Thrive Lp(a) Analysis takes many minutes and eventually throws an Ingres error (shown in full below). 

2018 "A few THRIVE..." takes about 30 seconds but works OK. Suspect size of QEP for -1015 is too big for server.

### scratch

#### load job

29/11/2013 17:48:13: preparing query (1743)
29/11/2013 17:48:13: opening query
29/11/2013 17:49:52: query open (about 1m40)
29/11/2013 17:49:55: finished retrieving rows, getting storage details
29/11/2013 17:49:58: finished getting storage details (3s)

#### process job

29/11/2013 14:57:19: loadRows for job 1743 started
29/11/2013 14:57:30: finished create temp table
29/11/2013 15:02:45: finished loading samples (5m15)
29/11/2013 15:02:45: finished drop temp table
29/11/2013 15:02:47: finished load storage details (2s)


## Rows not being sorted properly?

 * Is sort being applied to all columns?
 * Is correct type of sort (numeric/alpha) being used on each column?


## showChunk()/showChunks() - royally messed that up din I

## 2ndary aliquots - don't display at first, only at end of chunk if some vials not collected

## empty boxes - tbc

  show list, confirm
  update db to free positions
  allow mark for referral
  
## columns - fit to screen, resize by percentage

## add 1 chunk - must be last, not split mid chunk

## box move jobs? display in front screen

## does target db have correct statistics?

### Displaying the QEP

Precede the query with:
Set qep;
Set optimizeonly;
\p\g
<your query>
\p\g

## Original Query

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
    
### Original query exception

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

### QEP of original query

    ********************************************************************

    QUERY PLAN 83,1, timed out, of main query

                                                                                        Site ID 2->0
                                                                                        Sort Keep dups
                                                                                        Pages 34 Tups 305
                                                                                        D7314 C5964 N307
                                                                             /
                                                                            Site ID 2
                                                                            FSM Join(rj_box_cid,
                                                                             cryovial_barcode,
                                                                             aliquot_type_cid)
                                                                            Heap
                                                                            Pages 34 Tups 305
                                                                            D7314 C5964 N230
                                                     /                                              \
                                                    Site ID 3->2                                    Site ID 2
                                                    K Join(box_cid)                                 Proj-rest
                                                    Sort on(rj_box_cid,                             Sort on(rj_box_cid,
                                                     cryovial_barcode,                               cryovial_barcode,
                                                     aliquot_type_cid)                               aliquot_type_cid)
                                                    Pages 9 Tups 286                                Pages 139 Tups 4847
                                                    D7255 C5863 N230                                D59 C48 N0
                                         /                      \                        /
                                        Site ID 3               Site ID 3               Site ID 2
                                        FSM Join(aliquot_type_cib1                      lcr
                                        Sort on(box_cid)        (box_name)              (l_cryovial_retrieval)
                                        Pages 7 Tups 286        B-Tree(box_cid)         B-Tree(NU)
                                        D7126 C5854 N103        Pages 152 Tups 5798     Pages 235 Tups 4847
                 /                                              \
                Site ID 2->3                                    Site ID 3
                Proj-rest                                       K Join(cryovial_id)
                Sorted(object_cid)                              Sort on(aliquot_type_cid)
                Pages 4 Tups 384                                Pages 13 Tups 535
                D6 C4 N19                                       D7120 C5838 N84
     /                                               /                      \
    Site ID 2                                       Site ID 3               Site ID 3
    t                                               FSM Join(box_cid)       c
    (c_object_name)                                 Partial(cryovial_id)    (cryovial)
    B-Tree(NU)                                      Pages 8 Tups 535        B-Tree(cryovial_id)
    Pages 24 Tups 384                               D6585 C5827 N84         Pages 9047 Tups 526483
                             /                                              \
                            Site ID 4->3                                    Site ID 3
                            FSM Join(box_id)                                Proj-rest
                            Sorted(box_id)                                  Sort on(box_cid)
                            Pages 5 Tups 570                                Pages 54 Tups 5798
                            D6547 C5704 N84                                 D38 C58 N0
                 /                      \                        /
                Site ID 2->4            Site ID 4               Site ID 3
                Proj-rest               T Join(tidp)            b2
                Sort on(box_id)         Sort on(box_cid)        (box_name)
                Pages 2 Tups 320        Pages 41 Tups 5617      B-Tree(NU)
                D2 C3 N26               D6545 C5640 N0          Pages 152 Tups 5798
     /                       /                      \
    Site ID 2               Site ID 4               Site ID 4
    cbr                     K Join(cryovial_id)     s2
    (c_box_retrieval)       Sort on(tidp)           (cryovial_store)
    B-Tree(NU)              Pages 40 Tups 5617      B-Tree(NU)
    Pages 8 Tups 344        D2328 C5526 N0          Pages 9310 Tups 541802
                 /                      \
                Site ID 4               Site ID 4
                Proj-rest               ddx_1754_1756
                Sorted(cryovial_id)     (cryovial_store_idx1)
                Pages 34 Tups 5418      I(s2)
                D2328 C5418 N0          B-Tree(cryovial_id,
                                         status)
                                        Pages 6522 Tups 541802
     /
    Site ID 4
    s1
    (cryovial_store)
    B-Tree(NU)
    Pages 9310 Tups 541802
    Distributed Site information
        Site 0 has node name= VLAB_CTSU_OX_AC_UK, LDB name= not applicable
        Site 1 has node name= vlab.ctsu.ox.ac.uk, LDB name= iit_ldb20_ddb
        Site 2 has node name= vlab.ctsu.ox.ac.uk, LDB name= t_ldbc
        Site 3 has node name= vlab.ctsu.ox.ac.uk, LDB name= t_ldb20
        Site 4 has node name= , LDB name= t_ldb20

    ********************************************************************

## Slimmed-down query

I took out c_object_name (supposed to be used only for alquot type name, but I wasn't even using it for that anyway); it seemed to speed up loading the 'few' plan to about 15 seconds (from what?), but the lpa plan just seemed to hang. 
(The QEP for this new plan earlier contained a cart-prod - might have left c_object_name in FROM clause)

********************************************************************

QUERY PLAN 34,1, timed out, of main query

                                                                                                                        Site ID 2->0
                                                                                                                        Sort Keep dups
                                                                                                                        Pages 64 Tups 570
                                                                                                                        D7217 C6064 N452
                                                                                                             /
                                                                                                            Site ID 2
                                                                                                            FSM Join(rj_box_cid,
                                                                                                             cryovial_barcode,
                                                                                                             aliquot_type_cid)
                                                                                                            Heap
                                                                                                            Pages 64 Tups 570
                                                                                                            D7217 C6064 N307
                                                                                     /                                              \
                                                                                    Site ID 3->2                                    Site ID 2
                                                                                    K Join(cryovial_id)                             Proj-rest
                                                                                    Sort on(rj_box_cid,                             Sort on(rj_box_cid,
                                                                                     cryovial_barcode,                               cryovial_barcode,
                                                                                     aliquot_type_cid)                               aliquot_type_cid)
                                                                                    Pages 16 Tups 535                               Pages 139 Tups 4847
                                                                                    D7158 C5960 N307                                D59 C48 N0
                                                                         /                      \                        /
                                                                        Site ID 3               Site ID 3               Site ID 2
                                                                        FSM Join(box_cid)       c                       lcr
                                                                        Partial(cryovial_id)    (cryovial)              (l_cryovial_retrieval)
                                                                        Pages 10 Tups 535       B-Tree(cryovial_id)     B-Tree(NU)
                                                                        D6623 C5950 N84         Pages 9047 Tups 526483  Pages 235 Tups 4847
                                                 /                                                          \
                                                Site ID 3                                                   Site ID 3
                                                FSM Join(box_cid)                                           Proj-rest
                                                Sort on(box_cid)                                            Sort on(box_cid)
                                                Pages 8 Tups 535                                            Pages 54 Tups 5798
                                                D6585 C5827 N84                                             D38 C58 N0
                         /                                              \                        /
                        Site ID 4->3                                    Site ID 3               Site ID 3
                        FSM Join(box_id)                                Proj-rest               b1
                        Sorted(box_id)                                  Sort on(box_cid)        (box_name)
                        Pages 5 Tups 570                                Pages 54 Tups 5798      B-Tree(NU)
                        D6547 C5704 N84                                 D38 C58 N0              Pages 152 Tups 5798
             /                      \                        /
            Site ID 2->4            Site ID 4               Site ID 3
            Proj-rest               T Join(tidp)            b2
            Sort on(box_id)         Sort on(box_cid)        (box_name)
            Pages 2 Tups 320        Pages 41 Tups 5617      B-Tree(NU)
            D2 C3 N26               D6545 C5640 N0          Pages 152 Tups 5798
 /                       /                      \
Site ID 2               Site ID 4               Site ID 4
cbr                     K Join(cryovial_id)     s2
(c_box_retrieval)       Sort on(tidp)           (cryovial_store)
B-Tree(NU)              Pages 40 Tups 5617      B-Tree(NU)
Pages 8 Tups 344        D2328 C5526 N0          Pages 9310 Tups 541802
             /                      \
            Site ID 4               Site ID 4
            Proj-rest               ddx_1754_1756
            Sorted(cryovial_id)     (cryovial_store_idx1)
            Pages 34 Tups 5418      I(s2)
            D2328 C5418 N0          B-Tree(cryovial_id,
                                     status)
                                    Pages 6522 Tups 541802
 /
Site ID 4
s1
(cryovial_store)
B-Tree(NU)
Pages 9310 Tups 541802
Distributed Site information
    Site 0 has node name= VLAB_CTSU_OX_AC_UK, LDB name= not applicable
    Site 1 has node name= vlab.ctsu.ox.ac.uk, LDB name= iit_ldb20_ddb
    Site 2 has node name= vlab.ctsu.ox.ac.uk, LDB name= t_ldbc
    Site 3 has node name= vlab.ctsu.ox.ac.uk, LDB name= t_ldb20
    Site 4 has node name= , LDB name= t_ldb20

********************************************************************

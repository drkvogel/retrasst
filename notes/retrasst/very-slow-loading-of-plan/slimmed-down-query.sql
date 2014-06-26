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
    c.cryovial_id = s1.cryovial_id AND
    c.cryovial_id = s2.cryovial_id AND
    b1.box_cid = s1.box_cid AND
    s2.box_cid = b2.box_cid
 ORDER BY
    s1.retrieval_cid, chunk, cbr.rj_box_cid, lcr.position
\p\g

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
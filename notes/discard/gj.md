﻿select * from c_box_retrieval where rj_box_cid = 2057

select * from c_box_retrieval cbr, l_cryovial_retrieval lcr where cbr.rj_box_cid = lcr.rj_box_cid 
 
 DECLARE GLOBAL TEMPORARY TABLE session.temptable AS select * from myview
ON COMMIT PRESERVE ROWS WITH NORECOVERY;

select * from c_retrieval_job

   SELECT COUNT(*)
/*       cbr.section AS chunk, cbr.rj_box_cid, cbr.retrieval_cid, cbr.status AS cbr_status, cbr.box_id,
       lcr.position AS dest_pos, lcr.slot_number AS lcr_slot, lcr.process_cid AS lcr_procid, lcr.status AS lcr_status,
       lcr.cryovial_barcode, lcr.aliquot_type_cid*/
   FROM
       c_box_retrieval cbr, l_cryovial_retrieval lcr
   WHERE
       cbr.retrieval_cid = -1015 /*1743  -632349*/
   /*AND cbr.section = 2*/
   AND
       lcr.rj_box_cid = cbr.rj_box_cid
 
SELECT     s1.retrieval_cid, g.chunk, g.rj_box_cid, g.cbr_status, g.dest_pos, g.lcr_slot, g.lcr_procid, g.lcr_status,     s1.cryovial_id, s1.note_exists, s1.retrieval_cid, s1.box_cid, s1.status, s1.tube_position,     s1.record_id, c.cryovial_barcode, c.sample_id, c.aliquot_type_cid, c.note_exists AS cryovial_note,     s1.box_cid, b1.external_name AS src_box, s1.status, s1.tube_position, s1.note_exists AS cs_note,     g.box_id AS dest_id, b2.external_name AS dest_name, s2.tube_position AS slot_number, s2.status AS dest_status FROM retrieval_assistant_temp g, cryovial c, cryovial_store s1, cryovial_store s2, box_name b1, box_name b2 WHERE     c.cryovial_barcode = g.cryovial_barcode     AND c.aliquot_type_cid = g.aliquot_type_cid     AND s1.cryovial_id = c.cryovial_id     AND s1.retrieval_cid = g.retrieval_cid     AND b2.box_cid = g.box_id     AND b1.box_cid = s1.box_cid     AND s2.cryovial_id = c.cryovial_id     AND b2.box_cid = s2.box_cid ORDER BY     s1.retrieval_cid, chunk, g.rj_box_cid, dest_pos

sql in shell script (from GJ):

It depends on the version of Ingres where you’re running the “sql” command. There are “-d” and “-s” command line options, which get rid of some of the header info.

More recent versions also have “-S” which is for “silent mode”, it gets rid of the everything except the data you select, so you won’t get any column titles, separators, rowcounts.
If you need something in between all the info and none of the info, there are some other flags which I never remember but can look them up.

For the column widths, and this is in only the most recent 10.1 versions, you can do:

\colformat resultcolumnname formatspec

Examples:

\colformat first_barcode c10
or
\colformat myfloatcolumn +f5.2
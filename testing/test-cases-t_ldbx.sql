\sql
set autocommit on
\nocontinue
\p\g

update box_store s 
from box_name n 
set retrieval_cid = -636363 
where box_type_cid = 18 
and n.box_cid = s.box_cid 
and n.box_cid 
between 2868300 and 2868400;
\p\g

update cryovial_store s from cryovial c set retrieval_cid = -363636
where s.cryovial_id = c.cryovial_id 
and aliquot_type_cid = -31781 
and cryovial_barcode like '112155%';
\p\g

update cryovial_store s from cryovial c set retrieval_cid = -363636
where s.cryovial_id = c.cryovial_id 
and aliquot_type_cid = -31782 
and cryovial_barcode like '112155%';
\p\g

/* n.b. extra barcode field in box_name is filled in if target columns are specified
if non-defaulted columns are not specified as targets, all column values must be given otherwise this error results:
E_US0AD7 INSERT: number of target columns must equal the number of specified values.

box_cid, box_type_cid, box_capacity, external_name, status, slot_cid, time_stamp, note_exists, process_cid [barcode] */
--insert into box_name (box_cid, box_type_cid, box_capacity, external_name, status, slot_cid, time_stamp, note_exists, process_cid) values ( -111, 101, 0, 'Example retrieval box 1',2,0,'now',0,0);
--insert into box_name (box_cid, box_type_cid, box_capacity, external_name, status, slot_cid, time_stamp, note_exists, process_cid) values ( -112, 101, 0, 'Example retrieval box 2',2,0,'now',0,0);
--insert into box_name (box_cid, box_type_cid, box_capacity, external_name, status, slot_cid, time_stamp, note_exists, process_cid) values ( -113, 101, 0, 'Example retrieval box 3',2,0,'now',0,0);
--insert into box_name (box_cid, box_type_cid, box_capacity, external_name, status, slot_cid, time_stamp, note_exists, process_cid) values ( -114, 101, 0, 'Example retrieval box 4',2,0,'now',0,0);
--insert into box_name (box_cid, box_type_cid, box_capacity, external_name, status, slot_cid, time_stamp, note_exists, process_cid) values ( -115, 101, 0, 'Example retrieval box 5',2,0,'now',0,0);
--insert into box_name (box_cid, box_type_cid, box_capacity, external_name, status, slot_cid, time_stamp, note_exists, process_cid) values ( -116, 101, 0, 'Example retrieval box 6',2,0,'now',0,0);
--insert into box_name (box_cid, box_type_cid, box_capacity, external_name, status, slot_cid, time_stamp, note_exists, process_cid) values ( -117, 101, 0, 'Example retrieval box 7',2,0,'now',0,0);
--insert into box_name (box_cid, box_type_cid, box_capacity, external_name, status, slot_cid, time_stamp, note_exists, process_cid) values ( -118, 101, 0, 'Example retrieval box 8',2,0,'now',0,0);
--insert into box_name (box_cid, box_type_cid, box_capacity, external_name, status, slot_cid, time_stamp, note_exists, process_cid) values ( -119, 101, 0, 'Example retrieval box 9',2,0,'now',0,0);
--\p\g

/*
select count(*) from cryovial_store s, cryovial c 
where s.cryovial_id = c.cryovial_id
and aliquot_type_cid = -31781 
and cryovial_barcode like '112155%' -- 837 */

drop sequence temp_box_position;
create sequence temp_box_position start with 11101;
\p\g

insert into cryovial_store (record_id, cryovial_id, box_cid, cryovial_position, time_stamp, status, note_exists)
select -s.cryovial_id, s.cryovial_id, -(next value for temp_box_position)/100,  mod(next value for temp_box_position,100),'now',0,0
from cryovial_store s, cryovial c 
where s.cryovial_id = c.cryovial_id 
and aliquot_type_cid = -31781 
and cryovial_barcode like '112155%';
\p\g

/* to delete the above:

delete from cryovial_store s
where exists (
 select 1 from cryovial c  
 where s.cryovial_id = c.cryovial_id
 and aliquot_type_cid = -31781 
 and cryovial_barcode like '112155%'
) 
*/

drop sequence temp_box_position;
\p\g
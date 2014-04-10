/* 

Set up test boxs and vials for retrieval jobs -636363 and -363636 for retrrieval assistant testing

After installing Ingres/Actian 64-bit (IJ), started new shell to pick up new sql binary at

/cygdrive/c/Program Files/Actian/IngresIJ/ingres/bin/sql

where vnode_vlab_64 points to vlabdev, where Geraint gave me delete permissions on box_name, cryovial_store, rather than vlab

Also, previous attempts to insert test data had left records in box_name where external_name like 'Example retrieval box%', 

but box_cid is not in (-111, -112, -113, -114, -115, -116, -117, -118, -119)

index idx_box_name_external keys external_name
*/

\sql
set autocommit on
\nocontinue
\p\g

update box_store s 
from box_name n 
set retrieval_cid = -636363 
where 
	box_type_cid = 18 and 
	n.box_cid = s.box_cid and
	n.box_cid between 2868300 and
	2868400;
\p\g

update cryovial_store s 
from cryovial c 
set retrieval_cid = -363636 
where 
	s.cryovial_id = c.cryovial_id and
	aliquot_type_cid = -31781 and
	cryovial_barcode like '112155%';
\p\g

update cryovial_store s from cryovial c 
set retrieval_cid = -363636 
where 
	s.cryovial_id = c.cryovial_id and
	aliquot_type_cid = -31782 and
	cryovial_barcode like '112155%';
\p\g

delete from box_name where box_cid in (-111, -112, -113, -114, -115, -116, -117, -118, -119)
\p\g

-- t_ldb20.box_content -623411 EDTA_any aliquot_type1 = -31782, aliquot_type2 = -31782
    
insert into box_name (box_cid, box_type_cid, box_capacity, external_name, status, slot_cid, time_stamp, note_exists, process_cid) values (-111, -623411, 0, 'Example retrieval box 1',2,0,'now',0,0);
insert into box_name (box_cid, box_type_cid, box_capacity, external_name, status, slot_cid, time_stamp, note_exists, process_cid) values (-112, -623411, 0, 'Example retrieval box 2',2,0,'now',0,0);
insert into box_name (box_cid, box_type_cid, box_capacity, external_name, status, slot_cid, time_stamp, note_exists, process_cid) values (-113, -623411, 0, 'Example retrieval box 3',2,0,'now',0,0);
insert into box_name (box_cid, box_type_cid, box_capacity, external_name, status, slot_cid, time_stamp, note_exists, process_cid) values (-114, -623411, 0, 'Example retrieval box 4',2,0,'now',0,0);
insert into box_name (box_cid, box_type_cid, box_capacity, external_name, status, slot_cid, time_stamp, note_exists, process_cid) values (-115, -623411, 0, 'Example retrieval box 5',2,0,'now',0,0);
insert into box_name (box_cid, box_type_cid, box_capacity, external_name, status, slot_cid, time_stamp, note_exists, process_cid) values (-116, -623411, 0, 'Example retrieval box 6',2,0,'now',0,0);
insert into box_name (box_cid, box_type_cid, box_capacity, external_name, status, slot_cid, time_stamp, note_exists, process_cid) values (-117, -623411, 0, 'Example retrieval box 7',2,0,'now',0,0);
insert into box_name (box_cid, box_type_cid, box_capacity, external_name, status, slot_cid, time_stamp, note_exists, process_cid) values (-118, -623411, 0, 'Example retrieval box 8',2,0,'now',0,0);
insert into box_name (box_cid, box_type_cid, box_capacity, external_name, status, slot_cid, time_stamp, note_exists, process_cid) values (-119, -623411, 0, 'Example retrieval box 9',2,0,'now',0,0);
\p\g

drop sequence temp_box_position;
create sequence temp_box_position start with 11101;
\p\g

/* 

duplicate insert error here
select * from cryovial_store where box_cid in ( -111, -112, -113, -114, -115, -116, -117, -118, -119)
 gives no results

indexes key cryovial_store on

cryovial_store_idx1
  cryovial_id
  status
  Unique: No

cryovial_store_idx3
  box_cid
  Unique: No

cryovial_store_idx3
  cryovial_id
  retrieval_d
  box_cid
  Unique: No

* help constraint cryovial_store\g
Constraints defined for lust.cryovial_store are:

Constraint Name: fk_cryovial_store
 FOREIGN KEY (cryovial_id) REFERENCES "lust".cryovial WITH (INDEX = "cryovial_store_idx1")

Constraint Name: fk_cryovial_store_box
 FOREIGN KEY (box_cid) REFERENCES "lust".box_name ON UPDATE CASCADE WITH (INDEX = "cryovial_store_idx2")

Constraint Name: pk_cryovial_store
 PRIMARY KEY(record_id) WITH (INDEX = BASE TABLE STRUCTURE)


* select rule_name from iirules where table_name = 'cryovial_store'\g

...                                                                                                                                                                                                                                   |
|cryovial_store_ins     

* help rule cryovial_store_ins\g

Rule:                 cryovial_store_ins
Procedure Type:       user object
Owner:                lust
Table:                cryovial_store

Rule Definition:
create rule  cryovial_store_ins before insert into cryovial_store for
each row execute procedure "lust". rfp_cryovial_store_constr(
record_id = new.record_id, cryovial_id = new.cryovial_id )

* help procedure rfp_cryovial_store_constr\g

Procedure:            rfp_cryovial_store_constr
Owner:                lust
Procedure Type:       native
Object type:          user object
Created:              04-apr-2014 13:17:52

Procedure Definition:
create procedure  rfp_cryovial_store_constr( 
	INOUT
	record_id integer4 not null not default, 
	cryovial_id integer4 not null
	not default ) as 
begin 
	if (record_id = 0 or record_id = cryovial_id) then 
		select id_sequence.nextval into :record_id;
	endif;
end

procedure rfp_cryovial_store_constr possibly should be removed.

suggest replacing 

"-s.cryovial_id"
with 
"next value for id_sequence"

to get around it now.

*/


insert into 
	cryovial_store (record_id, cryovial_id, box_cid, cryovial_position, time_stamp, status, note_exists)
select 
	next value for id_sequence,
	-- -s.cryovial_id, 
	s.cryovial_id, 
	-(next value for temp_box_position)/100, 
	mod(next value for temp_box_position,100),'now', 0, 0
from 
	cryovial_store s, cryovial c 
where 
	s.cryovial_id = c.cryovial_id 
	and aliquot_type_cid = -31782 
	and cryovial_barcode like '112155%';
\p\g

-- select the above: select count(*) from cryovial_store s, cryovial c where s.cryovial_id = c.cryovial_id and aliquot_type_cid = -31781 and cryovial_barcode like '112155%' -- 837 
-- delete the above: delete from cryovial_store s where exists (select 1 from cryovial c where s.cryovial_id = c.cryovial_id and aliquot_type_cid = -31781 and cryovial_barcode like '112155%') 

drop sequence temp_box_position;
\p\g
\q

/* n.b. extra barcode field in box_name is filled in if target columns are specified
if non-defaulted columns are not specified as targets, all column values must be given otherwise this error results:
E_US0AD7 INSERT: number of target columns must equal the number of specified values.
    box_cid, box_type_cid, box_capacity, external_name, status, slot_cid, time_stamp, note_exists, process_cid [barcode] */

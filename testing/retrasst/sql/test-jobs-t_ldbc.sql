/* Here's the SQL for the mirror system.  You'll need to modify the project CID for the test system: */

insert into c_retrieval_job 
values (-636363, 'Example box retrieval','A box retrieval for Chris', 0,-149662,1,2,'',-31781,0,'10/07/13','', 0);

insert into c_retrieval_job 
values (-363636, 'Example cryovial retrieval','A cryovial retrieval for Chris',0,-149662,1,4,'',-31781,0,'10/07/13','',-31782);

update box_store s 
from box_name n 
set retrieval_cid = -636363 
where box_type_cid = 18 
and n.box_cid = s.box_cid 
and n.box_cid 
between 2868300 and 2868400;

update cryovial_store s from cryovial c set retrieval_cid = -363636
where s.cryovial_id = c.cryovial_id 
and aliquot_type_cid = -31781 
and cryovial_barcode like '112155%';

update cryovial_store s from cryovial c set retrieval_cid = -363636
where s.cryovial_id = c.cryovial_id 
and aliquot_type_cid = -31782 
and cryovial_barcode like '112155%';

insert into box_name values ( -111, 101, 0, 'Example retrieval box 1',2,0,'now',0,0);
insert into box_name values ( -112, 101, 0, 'Example retrieval box 2',2,0,'now',0,0);
insert into box_name values ( -113, 101, 0, 'Example retrieval box 3',2,0,'now',0,0);
insert into box_name values ( -114, 101, 0, 'Example retrieval box 4',2,0,'now',0,0);
insert into box_name values ( -115, 101, 0, 'Example retrieval box 5',2,0,'now',0,0);
insert into box_name values ( -116, 101, 0, 'Example retrieval box 6',2,0,'now',0,0);
insert into box_name values ( -117, 101, 0, 'Example retrieval box 7',2,0,'now',0,0);
insert into box_name values ( -118, 101, 0, 'Example retrieval box 8',2,0,'now',0,0);
insert into box_name values ( -119, 101, 0, 'Example retrieval box 9',2,0,'now',0,0);

create sequence temp_box_position start with 11101;

insert into cryovial_store
select -s.cryovial_id, s.cryovial_id, -(next value for temp_box_position)/100,  mod(next value for temp_box_position,100),'now','',0,0,0,0
from cryovial_store s, cryovial c 
where s.cryovial_id = c.cryovial_id 
and aliquot_type_cid = -31781 
and cryovial_barcode like '112155%';

drop sequence temp_box_position;
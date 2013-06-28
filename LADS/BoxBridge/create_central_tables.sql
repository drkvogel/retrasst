/* create LADS database on laptop */

/* 
    Do:

	sql vlab::t_ldbc <create_central_tables.sql >create_central_tables.lis
    
*/

create table c_box_arrival(
	laptop_cid integer not null,
	box_arrival_id integer not null,
	project_cid integer not null,
	operator_cid integer not null,
	swipe_time date not null,
	box_name varchar(32) not null,
	status integer not null,
	note_exists integer not null,
	first_barcode varchar(20) not null,
	first_position integer not null,
	last_barcode varchar(20) not null,
	last_position integer not null,
	tank_cid integer not null,
	rack_number varchar(12) not null,
	rack_slot smallint not null
);
\g
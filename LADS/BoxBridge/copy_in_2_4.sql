
/* SQL script created by COPYDB, version II 9.2.1 (int.w32/103). */
/* Modified to create v2.5 laptop database for Box Reception - AM/NG	*/

\sql
set autocommit on
\p\g
set nojournaling
\p\g
\sql
set session with privileges=all
\p\g

	/* SEQUENCES */
create sequence c_box_w_record_no as integer
    start with 2045 increment by 1
    minvalue 1 maxvalue 2147483647
    cache 20 no cycle no order
\p\g
create sequence c_buddy_w_record_no as integer
    start with 8223 increment by 1
    minvalue 1 maxvalue 2147483647
    cache 20 no cycle no order
\p\g

	/* TABLES */
\nocontinue
create table archive_c_audit_trail(
	message_date ingresdate not null default 'now',
	process_cid integer not null,
	pid_sequence smallint not null default 0,
	operator_cid integer not null default 0,
	message_type smallint not null,
	machine_cid integer not null,
	database_name varchar(32) not null,
	program_name varchar(32),
	details varchar(255) not null,
	action char(1) not null default ' ',
	action_date ingresdate not null default ' ',
	action_user_name varchar(32) not null default ' '
)
with duplicates,
nojournaling,
page_size = 4096,
location = (ii_database),
security_audit=(table,norow)
\p\g
create table archive_c_block(
	running integer not null,
	blocked integer not null,
	block_type smallint not null default 0,
	action char(1) not null default ' ',
	action_date ingresdate not null default ' ',
	action_user_name varchar(32) not null default ' '
)
with duplicates,
nojournaling,
page_size = 4096,
location = (ii_database),
security_audit=(table,norow)
\p\g
modify archive_c_block to btree on
	running,
	blocked,
	action_date
with nonleaffill = 80,
	leaffill = 70,
	fillfactor = 80,
	extend = 16,
	page_size = 4096
\p\g
create table archive_c_box_retrieval(
	retrieval_cid integer not null not default,
	section smallint not null not default,
	position smallint not null not default,
	box_id integer not null not default,
	box_name varchar(32) not null not default,
	rj_box_cid integer not null not default,
	status smallint not null not default,
	time_stamp ingresdate not null default 'now',
	action char(1) not null default ' ',
	action_date ingresdate not null default ' ',
	action_user_name varchar(32) not null default ' '
)
with duplicates,
nojournaling,
page_size = 4096,
location = (ii_database),
security_audit=(table,norow)
\p\g
modify archive_c_box_retrieval to btree on
	rj_box_cid,
	action_date
with nonleaffill = 80,
	leaffill = 70,
	fillfactor = 80,
	extend = 16,
	page_size = 4096
\p\g
create table archive_c_box_size(
	box_size_cid integer not null,
	description varchar(32) not null,
	box_capacity smallint not null default 100,
	empty_position smallint not null default 0,
	valid_from ingresdate not null default ' ',
	valid_to ingresdate not null default ' ',
	status smallint not null default 0,
	tube_type smallint not null default 0,
	action char(1) not null default ' ',
	action_date ingresdate not null default ' ',
	action_user_name varchar(32) not null default ' '
)
with duplicates,
nojournaling,
page_size = 4096,
location = (ii_database),
security_audit=(table,norow)
\p\g
modify archive_c_box_size to btree on
	box_size_cid,
	action_date
with nonleaffill = 80,
	leaffill = 70,
	fillfactor = 80,
	extend = 16,
	page_size = 4096
\p\g
create table archive_c_box_worklist(
	record_no integer not null default 0,
	barcode varchar(32) not null,
	sample_id integer not null,
	status varchar(1) not null,
	project_cid integer not null,
	profile_name varchar(32) not null,
	box_type_cid integer not null,
	cryovial_barcode varchar(16) not null,
	time_stamp ingresdate not null,
	ts_sequence smallint not null default 0,
	group_id integer not null default 0,
	category_id integer not null default 0,
	action char(1) not null default ' ',
	action_date ingresdate not null default ' ',
	action_user_name varchar(32) not null default ' '
)
with duplicates,
nojournaling,
page_size = 4096,
location = (ii_database),
security_audit=(table,norow)
\p\g
modify archive_c_box_worklist to btree on
	project_cid,
	sample_id,
	box_type_cid,
	action_date
with nonleaffill = 80,
	leaffill = 70,
	fillfactor = 80,
	extend = 16,
	page_size = 4096
\p\g
create table archive_c_buddy_worklist(
	machine_cid integer not null,
	barcode varchar(32) not null,
	test_cid integer not null,
	group_id integer not null default 0,
	category_id integer not null default 0,
	sample_id integer not null,
	range_set varchar(1) not null default 'Y',
	min_value f4 not null default 0,
	max_value f4 not null default 0,
	project_cid integer not null,
	profile_id integer not null default 0,
	profile_name varchar(32) not null default '.',
	buddy_read varchar(1) not null,
	time_stamp ingresdate not null default 'now',
	ts_sequence smallint not null default 0,
	in_date ingresdate not null,
	trigger_id integer not null default 0,
	lower_trigger_limit f4 not null default 0,
	upper_trigger_limit f4 not null default 0,
	private_result varchar(1) not null default 'N',
	status char(1) not null default 'Q',
	diluent f4 not null default 0,
	action char(1) not null default ' ',
	action_date ingresdate not null default ' ',
	action_user_name varchar(32) not null default ' '
)
with duplicates,
nojournaling,
page_size = 4096,
location = (ii_database),
security_audit=(table,norow)
\p\g
modify archive_c_buddy_worklist to btree on
	barcode,
	machine_cid,
	test_cid,
	group_id,
	action_date
with nonleaffill = 80,
	leaffill = 70,
	fillfactor = 80,
	extend = 16,
	page_size = 4096
\p\g
create table archive_c_calibreag_test(
	calibreag_cid integer not null,
	test_cid integer not null,
	record_cid integer not null,
	status smallint not null,
	action char(1) not null default ' ',
	action_date ingresdate not null default ' ',
	action_user_name varchar(32) not null default ' '
)
with duplicates,
nojournaling,
page_size = 4096,
location = (ii_database),
security_audit=(table,norow)
\p\g
modify archive_c_calibreag_test to btree on
	calibreag_cid,
	test_cid,
	action_date
with nonleaffill = 80,
	leaffill = 70,
	fillfactor = 80,
	extend = 16,
	page_size = 4096
\p\g
create table archive_c_cluster_machine(
	record_cid integer not null not default,
	machine_cid integer not null not default,
	cluster_cid integer not null not default,
	status smallint not null not default,
	action char(1) not null default ' ',
	action_date ingresdate not null default 'now',
	action_user_name varchar(32) not null default ' '
)
with duplicates,
nojournaling,
page_size = 4096,
location = (ii_database),
security_audit=(table,norow)
\p\g
modify archive_c_cluster_machine to btree on
	record_cid,
	action_date
with nonleaffill = 80,
	leaffill = 70,
	fillfactor = 80,
	extend = 16,
	page_size = 4096
\p\g
create table archive_c_event_log(
	event_cid integer not null,
	event_code integer not null,
	machine_cid integer not null,
	test_cid integer not null,
	related_id integer not null,
	operator_cid integer not null default 0,
	event_date ingresdate not null,
	log_date ingresdate not null default 'now',
	process_cid integer not null,
	action char(1) not null default ' ',
	action_date ingresdate not null default ' ',
	action_user_name varchar(32) not null default ' '
)
with duplicates,
nojournaling,
page_size = 4096,
location = (ii_database),
security_audit=(table,norow)
\p\g
modify archive_c_event_log to btree on
	event_cid,
	action_date
with nonleaffill = 80,
	leaffill = 70,
	fillfactor = 80,
	extend = 16,
	page_size = 4096
\p\g
create table archive_c_lease(
	proj_id integer not null,
	task_id integer not null,
	task_desc varchar(60) not null,
	lease_type integer not null,
	start ingresdate not null,
	expiry ingresdate not null,
	action char(1) not null default ' ',
	action_date ingresdate not null default ' ',
	action_user_name varchar(32) not null default ' '
)
with duplicates,
nojournaling,
page_size = 4096,
location = (ii_database),
security_audit=(table,norow)
\p\g
modify archive_c_lease to btree on
	action_date
with nonleaffill = 80,
	leaffill = 70,
	fillfactor = 80,
	extend = 16,
	page_size = 4096
\p\g
create table archive_c_machine(
	machine_cid integer not null,
	external_name varchar(16) not null,
	external_full varchar(64) not null,
	valid_from ingresdate not null,
	valid_to ingresdate not null,
	status smallint not null,
	location varchar(64) not null,
	cluster_cid integer not null default 0,
	action char(1) not null default ' ',
	action_date ingresdate not null default ' ',
	action_user_name varchar(32) not null default ' '
)
with duplicates,
nojournaling,
page_size = 4096,
location = (ii_database),
security_audit=(table,norow)
\p\g
modify archive_c_machine to btree on
	machine_cid,
	action_date
with nonleaffill = 80,
	leaffill = 70,
	fillfactor = 80,
	extend = 16,
	page_size = 4096
\p\g
create table archive_c_object_name(
	object_cid integer not null,
	process_cid integer not null default 0,
	external_name varchar(16) not null,
	external_full varchar(64) not null,
	object_type smallint not null,
	status smallint not null,
	time_stamp ingresdate not null default 'now',
	action char(1) not null default ' ',
	action_date ingresdate not null default ' ',
	action_user_name varchar(32) not null default ' '
)
with duplicates,
nojournaling,
page_size = 4096,
location = (ii_database),
security_audit=(table,norow)
\p\g
modify archive_c_object_name to btree on
	object_cid,
	action_date
with nonleaffill = 80,
	leaffill = 70,
	fillfactor = 80,
	extend = 16,
	page_size = 4096
\p\g
create table archive_c_operator(
	operator_cid integer not null,
	external_name varchar(16) not null,
	external_full varchar(128) not null,
	valid_from ingresdate not null,
	valid_to ingresdate not null,
	status smallint not null,
	abilities integer default 1,
	password_md5 varchar(32) not null default ' ',
	action char(1) not null default ' ',
	action_date ingresdate not null default ' ',
	action_user_name varchar(32) not null default ' '
)
with duplicates,
nojournaling,
page_size = 4096,
location = (ii_database),
security_audit=(table,norow)
\p\g
modify archive_c_operator to btree on
	operator_cid,
	action_date
with nonleaffill = 80,
	leaffill = 70,
	fillfactor = 80,
	extend = 16,
	page_size = 4096
\p\g
create table archive_c_permission(
	permission_cid integer not null not default,
	operator_cid integer not null not default,
	program_cid integer not null not default,
	project_cid integer not null not default,
	page_number smallint not null default 0,
	status smallint not null default 0,
	action char(1) not null default ' ',
	action_date ingresdate not null default 'now',
	action_user_name varchar(32) not null default ' '
)
with duplicates,
nojournaling,
page_size = 2048,
location = (ii_database),
security_audit=(table,norow)
\p\g
modify archive_c_permission to heap
with extend = 16,
	allocation = 4,
	page_size = 2048
\p\g
create table archive_c_project(
	project_cid integer not null,
	external_name varchar(16) not null,
	external_full varchar(128) not null,
	valid_from ingresdate not null,
	valid_to ingresdate not null,
	status smallint not null,
	database_version integer not null default 1,
	database_minor_version integer not null default 0,
	db_name varchar(16) not null,
	info_url varchar(128) not null,
	study_code char(4) not null default ' ',
	action char(1) not null default ' ',
	action_date ingresdate not null default ' ',
	action_user_name varchar(32) not null default ' '
)
with duplicates,
nojournaling,
page_size = 4096,
location = (ii_database),
security_audit=(table,norow)
\p\g
modify archive_c_project to btree on
	project_cid,
	action_date
with nonleaffill = 80,
	leaffill = 70,
	fillfactor = 80,
	extend = 16,
	page_size = 4096
\p\g
create table archive_c_rack_capacity(
	rack_type_cid integer not null,
	rack_capacity smallint not null,
	description varchar(32) not null,
	box_size_cid integer not null,
	first_rack varchar(12) not null default ' ',
	action char(1) not null default ' ',
	action_date ingresdate not null default ' ',
	action_user_name varchar(32) not null default ' '
)
with duplicates,
nojournaling,
page_size = 4096,
location = (ii_database),
security_audit=(table,norow)
\p\g
modify archive_c_rack_capacity to btree on
	rack_type_cid,
	action_date
with nonleaffill = 80,
	leaffill = 70,
	fillfactor = 80,
	extend = 16,
	page_size = 4096
\p\g
create table archive_c_rack_number(
	rack_cid integer not null,
	tank_cid integer not null,
	position smallint not null,
	external_name varchar(32) not null,
	rack_type_cid integer default NULL,
	default_box_type integer not null,
	project_cid integer not null,
	action char(1) not null default ' ',
	action_date ingresdate not null default ' ',
	action_user_name varchar(32) not null default ' '
)
with duplicates,
nojournaling,
page_size = 4096,
location = (ii_database),
security_audit=(table,norow)
\p\g
modify archive_c_rack_number to btree on
	rack_cid,
	action_date
with nonleaffill = 80,
	leaffill = 70,
	fillfactor = 80,
	extend = 16,
	page_size = 4096
\p\g
create table archive_c_reagent_lot(
	wedge_cid integer not null not default,
	serial_number char(4) not null not default,
	lot_number char(6) not null not default,
	test_cid integer not null not default,
	machine_cid integer not null not default,
	first_result ingresdate not null default 'now',
	action char(1) not null default ' ',
	action_date ingresdate not null default 'now',
	action_user_name varchar(32) not null default ' '
)
with duplicates,
nojournaling,
page_size = 4096,
location = (ii_database),
security_audit=(table,norow)
\p\g
modify archive_c_reagent_lot to btree on
	wedge_cid,
	action_date
with nonleaffill = 80,
	leaffill = 70,
	fillfactor = 80,
	extend = 16,
	page_size = 4096
\p\g
create table archive_c_retrieval_job(
	retrieval_cid integer not null not default,
	external_name varchar(16) not null not default,
	description varchar(64) not null not default,
	process_cid integer not null not default,
	project_cid integer not null not default,
	action char(1) not null default ' ',
	action_date ingresdate not null default ' ',
	action_user_name varchar(32) not null default ' '
)
with duplicates,
nojournaling,
page_size = 4096,
location = (ii_database),
security_audit=(table,norow)
\p\g
modify archive_c_retrieval_job to btree on
	retrieval_cid,
	action_date
with nonleaffill = 80,
	leaffill = 70,
	fillfactor = 80,
	extend = 16,
	page_size = 4096
\p\g
create table archive_c_slot_allocation(
	slot_cid integer not null,
	rack_cid integer not null,
	slot_position smallint not null,
	status smallint not null,
	action char(1) not null default ' ',
	action_date ingresdate not null default ' ',
	action_user_name varchar(32) not null default ' ',
	project_cid integer not null default 0,
	box_cid integer not null default 0
)
with duplicates,
nojournaling,
page_size = 4096,
location = (ii_database),
security_audit=(table,norow)
\p\g
modify archive_c_slot_allocation to btree on
	slot_cid,
	action_date
with nonleaffill = 80,
	leaffill = 70,
	fillfactor = 80,
	extend = 16,
	page_size = 4096
\p\g
create table archive_c_storage_details(
	storage_cid integer not null,
	storage_type smallint not null,
	location_cid integer not null,
	position smallint default 0,
	serial_number varchar(30) not null,
	physical_description varchar(64) not null,
	status smallint not null,
	time_stamp ingresdate not null,
	capacity smallint not null default 0,
	rack_layout_cid integer not null,
	action char(1) not null default ' ',
	action_date ingresdate not null default ' ',
	action_user_name varchar(32) not null default ' '
)
with duplicates,
nojournaling,
page_size = 4096,
location = (ii_database),
security_audit=(table,norow)
\p\g
modify archive_c_storage_details to btree on
	storage_cid,
	action_date
with nonleaffill = 80,
	leaffill = 70,
	fillfactor = 80,
	extend = 16,
	page_size = 4096
\p\g
create table archive_c_tank_layout(
	rack_layout_cid integer default NULL,
	fill_order smallint default NULL,
	section_prefix varchar(6) default NULL,
	first_rack smallint default NULL,
	last_rack smallint default NULL,
	rack_type_cid integer default NULL,
	record_cid integer not null,
	status smallint not null,
	rack_capacity smallint not null,
	confirm_rack smallint not null,
	action char(1) not null default ' ',
	action_date ingresdate not null default ' ',
	action_user_name varchar(32) not null default ' '
)
with duplicates,
nojournaling,
page_size = 4096,
location = (ii_database),
security_audit=(table,norow)
\p\g
modify archive_c_tank_layout to btree on
	action_date
with nonleaffill = 80,
	leaffill = 70,
	fillfactor = 80,
	extend = 16,
	page_size = 4096
\p\g
create table archive_c_tank_map(
	tank_cid integer not null,
	storage_cid integer not null,
	valid_from ingresdate not null,
	status smallint not null,
	valid_to ingresdate not null,
	rack_type_cid integer not null default 0,
	record_cid integer not null default 0,
	position smallint not null default 0,
	action char(1) not null default ' ',
	action_date ingresdate not null default ' ',
	action_user_name varchar(32) not null default ' '
)
with duplicates,
nojournaling,
page_size = 4096,
location = (ii_database),
security_audit=(table,norow)
\p\g
modify archive_c_tank_map to btree on
	tank_cid,
	storage_cid,
	valid_from,
	action_date
with nonleaffill = 80,
	leaffill = 70,
	fillfactor = 80,
	extend = 16,
	page_size = 4096
\p\g
create table archive_c_test(
	test_cid integer not null,
	external_name varchar(32) not null,
	external_full varchar(255) not null,
	valid_from ingresdate not null,
	valid_to ingresdate not null,
	status smallint not null,
	data_type integer not null,
	min_datapoints integer not null,
	max_datapoints integer not null,
	default_lower_limit varchar(12) not null,
	default_upper_limit varchar(12) not null,
	sample_type varchar(4) not null,
	precision smallint not null,
	action char(1) not null default ' ',
	action_date ingresdate not null default ' ',
	action_user_name varchar(32) not null default ' '
)
with duplicates,
nojournaling,
page_size = 4096,
location = (ii_database),
security_audit=(table,norow)
\p\g
modify archive_c_test to btree on
	test_cid,
	action_date
with nonleaffill = 80,
	leaffill = 70,
	fillfactor = 80,
	extend = 16,
	page_size = 4096
\p\g
create table archive_c_test_definition(
	result_test_cid integer not null,
	calculation smallint not null,
	test_position smallint not null,
	source_test_cid integer not null,
	source_multiple f4 not null,
	valid_from ingresdate default NULL,
	valid_to ingresdate default NULL,
	record_cid integer not null,
	status smallint not null,
	action char(1) not null default ' ',
	action_date ingresdate not null default ' ',
	action_user_name varchar(32) not null default ' '
)
with duplicates,
nojournaling,
page_size = 4096,
location = (ii_database),
security_audit=(table,norow)
\p\g
modify archive_c_test_definition to btree on
	record_cid,
	action_date
with nonleaffill = 80,
	leaffill = 70,
	fillfactor = 80,
	extend = 16,
	page_size = 4096
\p\g
create table archive_c_test_machine(
	test_cid integer not null,
	machine_cid integer not null,
	machine_test_name varchar(16) not null,
	protocol smallint not null default 0,
	test_order smallint not null default 0,
	valid_from ingresdate not null default 'now',
	valid_to ingresdate not null default '9/9/2099',
	status smallint not null default 0,
	record_cid integer not null default 0,
	action char(1) not null default ' ',
	action_date ingresdate not null default ' ',
	action_user_name varchar(32) not null default ' '
)
with duplicates,
nojournaling,
page_size = 4096,
location = (ii_database),
security_audit=(table,norow)
\p\g
modify archive_c_test_machine to btree on
	test_cid,
	machine_cid,
	protocol,
	action_date
with nonleaffill = 80,
	leaffill = 70,
	fillfactor = 80,
	extend = 16,
	page_size = 4096
\p\g
create table archive_qc_gate(
	qc_id integer not null,
	machine_cid integer not null,
	test_cid integer not null,
	mean float not null,
	stddev float not null,
	time_stamp ingresdate not null default 'now',
	action char(1) not null default ' ',
	action_date ingresdate not null default ' ',
	action_user_name varchar(32) not null default ' '
)
with duplicates,
nojournaling,
page_size = 4096,
location = (ii_database),
security_audit=(table,norow)
\p\g
modify archive_qc_gate to btree on
	qc_id,
	machine_cid,
	test_cid,
	time_stamp,
	action_date
with nonleaffill = 80,
	leaffill = 70,
	fillfactor = 80,
	extend = 16,
	page_size = 4096
\p\g
create table archive_qc_level(
	material varchar(2) not null,
	qc_level varchar(1) not null,
	level_descrip varchar(32) not null,
	status smallint not null default 0,
	record_cid integer not null,
	action char(1) not null default ' ',
	action_date ingresdate not null default ' ',
	action_user_name varchar(32) not null default ' '
)
with duplicates,
nojournaling,
page_size = 4096,
location = (ii_database),
security_audit=(table,norow)
\p\g
modify archive_qc_level to btree on
	material,
	qc_level,
	action_date
with nonleaffill = 80,
	leaffill = 70,
	fillfactor = 80,
	extend = 16,
	page_size = 4096
\p\g
create table archive_qc_lot(
	qc_id integer not null,
	material varchar(2) not null,
	qc_level varchar(1) not null,
	lot varchar(3) not null,
	lot_descrip varchar(32) not null,
	arrival_date ingresdate not null,
	level_cid integer not null,
	action char(1) not null default ' ',
	action_date ingresdate not null default ' ',
	action_user_name varchar(32) not null default ' '
)
with duplicates,
nojournaling,
page_size = 4096,
location = (ii_database),
security_audit=(table,norow)
\p\g
modify archive_qc_lot to btree on
	qc_id,
	action_date
with nonleaffill = 80,
	leaffill = 70,
	fillfactor = 80,
	extend = 16,
	page_size = 4096
\p\g
create table archive_qc_material(
	material varchar(2) not null,
	material_descrip varchar(32) not null,
	record_cid integer not null,
	action char(1) not null default ' ',
	action_date ingresdate not null default ' ',
	action_user_name varchar(32) not null default ' '
)
with duplicates,
nojournaling,
page_size = 4096,
location = (ii_database),
security_audit=(table,norow)
\p\g
modify archive_qc_material to btree on
	material,
	action_date
with nonleaffill = 80,
	leaffill = 70,
	fillfactor = 80,
	extend = 16,
	page_size = 4096
\p\g
create table archive_qc_test(
	qc_id integer not null,
	test_cid integer not null,
	packet_mean float not null,
	packet_stddev float not null,
	action char(1) not null default ' ',
	action_date ingresdate not null default ' ',
	action_user_name varchar(32) not null default ' '
)
with duplicates,
nojournaling,
page_size = 4096,
location = (ii_database),
security_audit=(table,norow)
\p\g
modify archive_qc_test to btree on
	qc_id,
	test_cid,
	action_date
with nonleaffill = 80,
	leaffill = 70,
	fillfactor = 80,
	extend = 16,
	page_size = 4096
\p\g
create table archive_qc_test_machine(
	record_cid integer not null not default,
	level_cid integer not null not default,
	machine_cid integer not null not default,
	test_cid integer not null not default,
	valid_from ingresdate not null default 'now',
	valid_to ingresdate not null default '12/12/12',
	status smallint not null default 0,
	diluent f4 not null default 0,
	action char(1) not null default ' ',
	action_date ingresdate not null default 'now',
	action_user_name varchar(32) not null default ' '
)
with duplicates,
nojournaling,
page_size = 4096,
location = (ii_database),
security_audit=(table,norow)
\p\g
modify archive_qc_test_machine to heap
with extend = 16,
	allocation = 4,
	page_size = 4096
\p\g
create table c_audit_trail(
	message_date ingresdate not null default 'now',
	process_cid integer not null,
	pid_sequence smallint not null default 0,
	operator_cid integer not null default 0,
	message_type smallint not null default 0,
	machine_cid integer not null,
	database_name varchar(32) not null,
	program_name varchar(32) not null default '.',
	details varchar(255) not null
)
with duplicates,
nojournaling,
page_size = 4096,
location = (ii_database),
security_audit=(table,norow)
\p\g
create table c_block(
	running integer not null,
	blocked integer not null,
	block_type smallint not null default 0
)
with duplicates,
nojournaling,
page_size = 4096,
location = (ii_database),
security_audit=(table,norow)
\p\g
modify c_block to btree unique on
	running,
	blocked
with nonleaffill = 80,
	leaffill = 70,
	fillfactor = 80,
	extend = 16,
	page_size = 4096,
	unique_scope = statement
\p\g
create table c_box_retrieval(
	retrieval_cid integer not null not default,
	section smallint not null not default,
	position smallint not null not default,
	box_id integer not null not default,
	box_name varchar(32) not null not default,
	rj_box_cid integer not null not default,
	status smallint not null not default,
	time_stamp ingresdate not null default 'now'
)
with duplicates,
nojournaling,
page_size = 4096,
location = (ii_database),
security_audit=(table,norow)
\p\g
modify c_box_retrieval to btree unique on
	rj_box_cid
with nonleaffill = 80,
	leaffill = 70,
	fillfactor = 80,
	extend = 16,
	page_size = 4096,
	unique_scope = statement
\p\g
create table c_box_size(
	box_size_cid integer not null,
	description varchar(32) not null,
	box_capacity smallint not null default 100,
	empty_position smallint not null default 0,
	valid_from ingresdate not null default 'now',
	valid_to ingresdate not null default '1/1/2030',
	status smallint not null default 0,
	tube_type smallint not null default 1
)
with duplicates,
nojournaling,
page_size = 4096,
location = (ii_database),
security_audit=(table,norow)
\p\g
modify c_box_size to btree unique on
	box_size_cid
with nonleaffill = 80,
	leaffill = 70,
	fillfactor = 80,
	extend = 16,
	page_size = 4096,
	unique_scope = statement
\p\g
create table c_box_worklist(
	record_no integer not null default 0,
	barcode varchar(32) not null,
	sample_id integer not null,
	status varchar(1) not null,
	project_cid integer not null,
	profile_name varchar(32) not null,
	box_type_cid integer not null,
	cryovial_barcode varchar(16) not null,
	time_stamp ingresdate not null,
	ts_sequence smallint not null default 0,
	group_id integer not null default 0,
	category_id integer not null default 0
)
with duplicates,
nojournaling,
page_size = 4096,
location = (ii_database),
security_audit=(table,norow)
\p\g
modify c_box_worklist to btree unique on
	project_cid,
	sample_id,
	box_type_cid
with nonleaffill = 80,
	leaffill = 70,
	fillfactor = 80,
	extend = 16,
	page_size = 4096,
	unique_scope = statement
\p\g
create table c_buddy_worklist(
	record_no integer not null default 0,
	machine_cid integer not null,
	barcode varchar(32) not null,
	test_cid integer not null,
	group_id integer not null default 0,
	category_id integer not null default 0,
	sample_id integer not null,
	range_set varchar(1) not null default 'Y',
	min_value f4 not null default 0,
	max_value f4 not null default 0,
	project_cid integer not null,
	profile_id integer not null default 0,
	profile_name varchar(32) not null default '.',
	buddy_read varchar(1) not null,
	time_stamp ingresdate not null default 'now',
	ts_sequence smallint not null default 0,
	in_date ingresdate not null,
	trigger_id integer not null default 0,
	lower_trigger_limit f4 not null default 0,
	upper_trigger_limit f4 not null default 0,
	private_result varchar(1) not null default 'N',
	status char(1) not null default 'Q',
	diluent f4 not null default 0
)
with duplicates,
nojournaling,
page_size = 4096,
location = (ii_database),
security_audit=(table,norow)
\p\g
modify c_buddy_worklist to btree unique on
	barcode,
	machine_cid,
	test_cid,
	group_id
with nonleaffill = 80,
	leaffill = 70,
	fillfactor = 80,
	extend = 16,
	page_size = 4096,
	unique_scope = statement
\p\g
create table c_calibreag_test(
	calibreag_cid integer not null,
	test_cid integer not null,
	record_cid integer not null,
	status smallint not null
)
with duplicates,
nojournaling,
page_size = 4096,
location = (ii_database),
security_audit=(table,norow)
\p\g
modify c_calibreag_test to btree unique on
	calibreag_cid,
	test_cid
with nonleaffill = 80,
	leaffill = 70,
	fillfactor = 80,
	extend = 16,
	page_size = 4096,
	unique_scope = statement
\p\g
create table c_cluster_machine(
	record_cid integer not null not default,
	machine_cid integer not null not default,
	cluster_cid integer not null not default,
	status smallint not null not default
)
with duplicates,
nojournaling,
page_size = 4096,
location = (ii_database),
security_audit=(table,norow)
\p\g
modify c_cluster_machine to btree unique on
	record_cid
with nonleaffill = 80,
	leaffill = 70,
	fillfactor = 80,
	extend = 16,
	page_size = 4096,
	unique_scope = statement
\p\g
create table c_event_log(
	event_cid integer not null,
	event_code integer not null,
	machine_cid integer not null,
	test_cid integer not null default 0,
	related_id integer not null default 0,
	operator_cid integer not null default 0,
	event_date ingresdate not null,
	log_date ingresdate not null default 'now',
	process_cid integer not null
)
with duplicates,
nojournaling,
page_size = 4096,
location = (ii_database),
security_audit=(table,norow)
\p\g
modify c_event_log to btree unique on
	event_cid
with nonleaffill = 80,
	leaffill = 70,
	fillfactor = 80,
	extend = 16,
	page_size = 4096,
	unique_scope = statement
\p\g
create table c_lease(
	proj_id integer not null,
	task_id integer not null,
	task_desc varchar(60) not null,
	lease_type integer not null,
	start ingresdate not null,
	expiry ingresdate not null
)
with duplicates,
nojournaling,
page_size = 2048,
location = (ii_database),
security_audit=(table,norow)
\p\g
modify c_lease to heap
with extend = 16,
	allocation = 4,
	page_size = 2048
\p\g
create table c_machine(
	machine_cid integer not null,
	external_name varchar(16) not null,
	external_full varchar(64) not null,
	valid_from ingresdate not null,
	valid_to ingresdate not null,
	status smallint not null,
	location varchar(64) not null,
	cluster_cid integer not null default 0
)
with duplicates,
nojournaling,
page_size = 4096,
location = (ii_database),
security_audit=(table,norow)
\p\g
modify c_machine to btree unique on
	machine_cid
with nonleaffill = 80,
	leaffill = 70,
	fillfactor = 80,
	extend = 16,
	page_size = 4096,
	unique_scope = statement
\p\g
create table c_next_cid(
	next_cid integer not null,
	available_range smallint not null default 0
)
with duplicates,
nojournaling,
page_size = 2048,
location = (ii_database),
security_audit=(table,norow)
\p\g
modify c_next_cid to heap
with extend = 16,
	allocation = 4,
	page_size = 2048
\p\g
create table c_object_name(
	object_cid integer not null,
	process_cid integer not null default 0,
	external_name varchar(16) not null,
	external_full varchar(64) not null,
	object_type smallint not null,
	status smallint not null,
	time_stamp ingresdate not null default 'now'
)
with duplicates,
nojournaling,
page_size = 4096,
location = (ii_database),
security_audit=(table,norow)
\p\g
modify c_object_name to btree unique on
	object_cid
with nonleaffill = 80,
	leaffill = 70,
	fillfactor = 80,
	extend = 16,
	page_size = 4096,
	unique_scope = statement
\p\g
create table c_operator(
	operator_cid integer not null,
	external_name varchar(16) not null,
	external_full varchar(128) not null,
	valid_from ingresdate not null,
	valid_to ingresdate not null,
	status smallint not null,
	abilities integer not null default 1,
	password_md5 varchar(32) not null not default
)
with duplicates,
nojournaling,
page_size = 4096,
location = (ii_database),
security_audit=(table,norow)
\p\g
modify c_operator to btree unique on
	operator_cid
with nonleaffill = 80,
	leaffill = 70,
	fillfactor = 80,
	extend = 16,
	page_size = 4096,
	unique_scope = statement
\p\g
create table c_permission(
	permission_cid integer not null not default,
	operator_cid integer not null not default,
	program_cid integer not null not default,
	project_cid integer not null default 0,
	page_number smallint not null default 0,
	status smallint not null default 0
)
with noduplicates,
nojournaling,
page_size = 4096,
location = (ii_database),
security_audit=(table,norow)
\p\g
modify c_permission to btree unique on
	permission_cid
with nonleaffill = 80,
	leaffill = 70,
	fillfactor = 80,
	extend = 16,
	page_size = 4096,
	unique_scope = statement
\p\g
create table c_project(
	project_cid integer not null,
	external_name varchar(16) not null,
	external_full varchar(128) not null,
	valid_from ingresdate not null,
	valid_to ingresdate not null,
	status smallint not null,
	database_version integer not null default 2,
	database_minor_version integer not null default 5,
	db_name varchar(16) not null,
	info_url varchar(128) not null default '.',
	study_code char(4) not null default '.'
)
with duplicates,
nojournaling,
page_size = 4096,
location = (ii_database),
security_audit=(table,norow)
\p\g
modify c_project to btree unique on
	project_cid
with nonleaffill = 80,
	leaffill = 70,
	fillfactor = 80,
	extend = 16,
	page_size = 4096,
	unique_scope = statement
\p\g
create table c_rack_capacity(
	rack_type_cid integer not null,
	rack_capacity smallint not null,
	description varchar(32) not null,
	box_size_cid integer not null,
	first_rack varchar(12) not null default ' '
)
with duplicates,
nojournaling,
page_size = 4096,
location = (ii_database),
security_audit=(table,norow)
\p\g
modify c_rack_capacity to btree unique on
	rack_type_cid
with nonleaffill = 80,
	leaffill = 70,
	fillfactor = 80,
	extend = 16,
	page_size = 4096,
	unique_scope = statement
\p\g
create table c_rack_number(
	rack_cid integer not null,
	tank_cid integer not null,
	position smallint not null,
	external_name varchar(32) not null,
	rack_type_cid integer not null default 0,
	default_box_type integer not null default 0,
	project_cid integer not null default 0
)
with duplicates,
nojournaling,
page_size = 4096,
location = (ii_database),
security_audit=(table,norow)
\p\g
modify c_rack_number to btree unique on
	rack_cid
with nonleaffill = 80,
	leaffill = 70,
	fillfactor = 80,
	extend = 16,
	page_size = 4096,
	unique_scope = statement
\p\g
create table c_reagent_lot(
	wedge_cid integer not null not default,
	serial_number char(4) not null not default,
	lot_number char(6) not null not default,
	test_cid integer not null not default,
	machine_cid integer not null not default,
	first_result ingresdate not null default 'now'
)
with duplicates,
nojournaling,
page_size = 4096,
location = (ii_database),
security_audit=(table,norow)
\p\g
modify c_reagent_lot to btree unique on
	wedge_cid
with nonleaffill = 80,
	leaffill = 70,
	fillfactor = 80,
	extend = 16,
	page_size = 4096,
	unique_scope = statement
\p\g
create table c_retrieval_job(
	retrieval_cid integer not null not default,
	external_name varchar(16) not null not default,
	description varchar(64) not null not default,
	process_cid integer not null not default,
	project_cid integer not null not default
)
with duplicates,
nojournaling,
page_size = 4096,
location = (ii_database),
security_audit=(table,norow)
\p\g
modify c_retrieval_job to btree unique on
	retrieval_cid
with nonleaffill = 80,
	leaffill = 70,
	fillfactor = 80,
	extend = 16,
	page_size = 4096,
	unique_scope = statement
\p\g
create table c_slot_allocation(
	slot_cid integer not null,
	rack_cid integer not null,
	slot_position smallint not null,
	status smallint not null,
	project_cid integer not null default 0,
	box_cid integer not null default 0
)
with duplicates,
nojournaling,
page_size = 4096,
location = (ii_database),
security_audit=(table,norow)
\p\g
modify c_slot_allocation to btree unique on
	slot_cid
with nonleaffill = 80,
	leaffill = 70,
	fillfactor = 80,
	extend = 16,
	page_size = 4096,
	unique_scope = statement
\p\g
create table c_storage_details(
	storage_cid integer not null,
	storage_type smallint not null,
	location_cid integer not null,
	position smallint not null default 0,
	serial_number varchar(30) not null,
	physical_description varchar(64) not null default '.',
	status smallint not null,
	time_stamp ingresdate not null default 'now',
	capacity smallint not null default 0,
	rack_layout_cid integer not null default 0
)
with duplicates,
nojournaling,
page_size = 4096,
location = (ii_database),
security_audit=(table,norow)
\p\g
modify c_storage_details to btree unique on
	storage_cid
with nonleaffill = 80,
	leaffill = 70,
	fillfactor = 80,
	extend = 16,
	page_size = 4096,
	unique_scope = statement
\p\g
create table c_tank_layout(
	rack_layout_cid integer not null not default,
	fill_order smallint not null not default,
	section_prefix varchar(6) not null not default,
	first_rack smallint not null not default,
	last_rack smallint not null not default,
	rack_type_cid integer not null default 0,
	record_cid integer not null,
	status smallint not null,
	rack_capacity smallint not null,
	confirm_rack smallint not null
)
with duplicates,
nojournaling,
page_size = 4096,
location = (ii_database),
security_audit=(table,norow)
\p\g
modify c_tank_layout to btree unique on
	record_cid
with nonleaffill = 80,
	leaffill = 70,
	fillfactor = 80,
	extend = 16,
	page_size = 4096,
	unique_scope = statement
\p\g
create table c_tank_map(
	tank_cid integer not null,
	storage_cid integer not null,
	valid_from ingresdate not null,
	status smallint not null,
	valid_to ingresdate not null,
	rack_type_cid integer not null default 0,
	record_cid integer not null default 0,
	position smallint not null default 0
)
with duplicates,
nojournaling,
page_size = 4096,
location = (ii_database),
security_audit=(table,norow)
\p\g
modify c_tank_map to btree unique on
	tank_cid,
	storage_cid,
	valid_from
with nonleaffill = 80,
	leaffill = 70,
	fillfactor = 80,
	extend = 16,
	page_size = 4096,
	unique_scope = statement
\p\g
create table c_test(
	test_cid integer not null,
	external_name varchar(32) not null,
	external_full varchar(255) not null,
	valid_from ingresdate not null,
	valid_to ingresdate not null,
	status smallint not null,
	data_type integer not null default 4,
	min_datapoints integer not null default 1,
	max_datapoints integer not null default 1,
	default_lower_limit varchar(12) not null,
	default_upper_limit varchar(12) not null,
	sample_type varchar(4) not null,
	precision smallint not null default 2
)
with duplicates,
nojournaling,
page_size = 4096,
location = (ii_database),
security_audit=(table,norow)
\p\g
modify c_test to btree unique on
	test_cid
with nonleaffill = 80,
	leaffill = 70,
	fillfactor = 80,
	extend = 16,
	page_size = 4096,
	unique_scope = statement
\p\g
create table c_test_definition(
	result_test_cid integer not null,
	calculation smallint not null,
	test_position smallint not null,
	source_test_cid integer not null,
	source_multiple f4 not null,
	valid_from ingresdate not null default 'now',
	valid_to ingresdate not null default '9/9/2099',
	record_cid integer not null,
	status smallint not null default 0
)
with duplicates,
nojournaling,
page_size = 4096,
location = (ii_database),
security_audit=(table,norow)
\p\g
modify c_test_definition to btree unique on
	record_cid
with nonleaffill = 80,
	leaffill = 70,
	fillfactor = 80,
	extend = 16,
	page_size = 4096,
	unique_scope = statement
\p\g
create table c_test_machine(
	test_cid integer not null,
	machine_cid integer not null,
	machine_test_name varchar(16) not null,
	protocol smallint not null default 0,
	test_order smallint not null default 0,
	valid_from ingresdate not null default 'now',
	valid_to ingresdate not null default '9/9/2099',
	status smallint not null default 0,
	record_cid integer not null default 0
)
with duplicates,
nojournaling,
page_size = 4096,
location = (ii_database),
security_audit=(table,norow)
\p\g
modify c_test_machine to btree unique on
	test_cid,
	machine_cid,
	protocol
with nonleaffill = 80,
	leaffill = 70,
	fillfactor = 80,
	extend = 16,
	page_size = 4096,
	unique_scope = statement
\p\g
create table l_box_arrival(
	laptop_cid integer not null,
	process_cid integer not null default 0,
	box_arrival_id integer not null,
	project_cid integer not null,
	swipe_time ingresdate not null,
	box_name varchar(32) not null,
	status integer not null default 0,
	first_position integer not null,
	last_position integer not null,
	first_barcode varchar(64) not null default '.',
	last_barcode varchar(64) not null default '.',
	tank_cid integer not null default 0,
	rack_number varchar(12) not null default '.',
	slot_position smallint not null default 0,
	time_stamp ingresdate not null default 'now'
)
with duplicates,
nojournaling,
page_size = 4096,
location = (ii_database),
security_audit=(table,norow)
\p\g
modify l_box_arrival to btree unique on
	project_cid,
	box_arrival_id
with nonleaffill = 80,
	leaffill = 70,
	fillfactor = 80,
	extend = 16,
	page_size = 4096
\p\g
create table l_box_arrival_event_history(
	box_arrival_id integer not null,
	event_cid integer not null,
	operator_cid integer not null,
	event_date ingresdate not null,
	process_cid integer not null,
	project_cid integer not null
)
with duplicates,
nojournaling,
page_size = 4096,
location = (ii_database),
security_audit=(table,norow)
\p\g
modify l_box_arrival_event_history to btree on
	project_cid,
	box_arrival_id,
	event_cid
with nonleaffill = 80,
	leaffill = 70,
	fillfactor = 80,
	extend = 16,
	page_size = 4096
\p\g
create table l_box_expected(
	box_expected_id integer not null not default,
	project_cid integer not null not default,
	box_name varchar(32) not null not default,
	first_position integer not null not default,
	last_position integer not null not default,
	first_barcode varchar(64) not null not default,
	last_barcode varchar(64) not null not default
)
with duplicates,
nojournaling,
page_size = 4096,
location = (ii_database),
security_audit=(table,norow)
\p\g
modify l_box_expected to btree unique on
	project_cid,
	box_expected_id
with nonleaffill = 80,
	leaffill = 70,
	fillfactor = 80,
	extend = 16,
	page_size = 4096
\p\g
create table l_cryovial_expected(
	rj_box_cid integer not null not default,
	position smallint not null not default,
	cryovial_barcode varchar(64) not null not default,
	aliquot_type_cid integer not null not default,
	tank_cid integer not null not default,
	rack_number varchar(12) not null not default,
	slot_position smallint not null not default,
	cryovial_position smallint not null not default,
	status smallint not null not default
)
with duplicates,
nojournaling,
page_size = 4096,
location = (ii_database),
security_audit=(table,norow)
\p\g
modify l_cryovial_expected to btree unique on
	rj_box_cid,
	position
with nonleaffill = 80,
	leaffill = 70,
	fillfactor = 80,
	extend = 16,
	page_size = 4096,
	unique_scope = statement
\p\g
create table l_cryovial_retrieval(
	rj_box_cid integer not null not default,
	position smallint not null not default,
	cryovial_barcode varchar(64) not null not default,
	aliquot_type_cid integer not null not default,
	process_cid integer not null not default,
	time_stamp ingresdate not null not default
)
with duplicates,
nojournaling,
page_size = 4096,
location = (ii_database),
security_audit=(table,norow)
\p\g
modify l_cryovial_retrieval to btree unique on
	rj_box_cid,
	position
with nonleaffill = 80,
	leaffill = 70,
	fillfactor = 80,
	extend = 16,
	page_size = 4096,
	unique_scope = statement
\p\g
create table qc_gate(
	qc_id integer not null,
	machine_cid integer not null,
	test_cid integer not null,
	mean float not null,
	stddev float not null,
	time_stamp ingresdate not null default 'now'
)
with duplicates,
nojournaling,
page_size = 4096,
location = (ii_database),
security_audit=(table,norow)
\p\g
modify qc_gate to btree unique on
	qc_id,
	machine_cid,
	test_cid,
	time_stamp
with nonleaffill = 80,
	leaffill = 70,
	fillfactor = 80,
	extend = 16,
	page_size = 4096,
	unique_scope = statement
\p\g
create table qc_level(
	material varchar(2) not null,
	qc_level varchar(1) not null,
	level_descrip varchar(32) not null,
	status smallint not null default 0,
	record_cid integer not null
)
with duplicates,
nojournaling,
page_size = 4096,
location = (ii_database),
security_audit=(table,norow)
\p\g
modify qc_level to btree unique on
	material,
	qc_level
with nonleaffill = 80,
	leaffill = 70,
	fillfactor = 80,
	extend = 16,
	page_size = 4096,
	unique_scope = statement
\p\g
create table qc_lot(
	qc_id integer not null,
	material varchar(2) not null,
	qc_level varchar(1) not null,
	lot varchar(3) not null,
	lot_descrip varchar(32) not null,
	arrival_date ingresdate not null,
	level_cid integer not null default 0
)
with duplicates,
nojournaling,
page_size = 4096,
location = (ii_database),
security_audit=(table,norow)
\p\g
modify qc_lot to btree unique on
	qc_id
with nonleaffill = 80,
	leaffill = 70,
	fillfactor = 80,
	extend = 16,
	page_size = 4096,
	unique_scope = statement
\p\g
create table qc_material(
	material varchar(2) not null,
	material_descrip varchar(32) not null,
	record_cid integer not null
)
with duplicates,
nojournaling,
page_size = 4096,
location = (ii_database),
security_audit=(table,norow)
\p\g
modify qc_material to btree unique on
	material
with nonleaffill = 80,
	leaffill = 70,
	fillfactor = 80,
	extend = 16,
	page_size = 4096,
	unique_scope = statement
\p\g
create table qc_test(
	qc_id integer not null,
	test_cid integer not null,
	packet_mean float not null,
	packet_stddev float not null
)
with duplicates,
nojournaling,
page_size = 4096,
location = (ii_database),
security_audit=(table,norow)
\p\g
modify qc_test to btree unique on
	qc_id,
	test_cid
with nonleaffill = 80,
	leaffill = 70,
	fillfactor = 80,
	extend = 16,
	page_size = 4096,
	unique_scope = statement
\p\g
create table qc_test_machine(
	record_cid integer not null not default,
	level_cid integer not null not default,
	machine_cid integer not null not default,
	test_cid integer not null not default,
	valid_from ingresdate not null default 'now',
	valid_to ingresdate not null default '12/12/12',
	status smallint not null default 0,
	diluent f4 not null default 0
)
with duplicates,
nojournaling,
page_size = 4096,
location = (ii_database),
security_audit=(table,norow)
\p\g
modify qc_test_machine to btree unique on
	record_cid
with nonleaffill = 80,
	leaffill = 70,
	fillfactor = 80,
	extend = 16,
	page_size = 4096,
	unique_scope = statement
\p\g

	/* INDEXES */
create index idx_cat_message_date on c_audit_trail (
	message_date)
with structure = btree,
	page_size = 2048,
	nocompression,
	key = (message_date),
	nonleaffill = 80,
	leaffill = 70,
	fillfactor = 80,
	location = (ii_database)
\p\g
create index idx_c_block_blocked on c_block (
	blocked)
with structure = btree,
	page_size = 2048,
	nocompression,
	key = (blocked),
	persistence,
	nonleaffill = 80,
	leaffill = 70,
	fillfactor = 80,
	location = (ii_database)
\p\g
create unique index idx_cbr_rsp on c_box_retrieval (
	retrieval_cid,
	section,
	position)
with structure = btree,
	page_size = 2048,
	nocompression,
	key = (retrieval_cid, section, position),
	unique_scope = statement,
	persistence,
	nonleaffill = 80,
	leaffill = 70,
	fillfactor = 80,
	location = (ii_database)
\p\g
create index cbw_cid_barcode_idx on c_box_worklist (
	box_type_cid,
	barcode)
with structure = btree,
	page_size = 2048,
	nocompression,
	key = (box_type_cid, barcode),
	nonleaffill = 80,
	leaffill = 70,
	fillfactor = 80,
	location = (ii_database)
\p\g
create unique index cbw_record_no_idx on c_box_worklist (
	record_no)
with structure = btree,
	page_size = 2048,
	nocompression,
	key = (record_no),
	unique_scope = statement,
	persistence,
	nonleaffill = 80,
	leaffill = 70,
	fillfactor = 80,
	location = (ii_database)
\p\g
create index cbw_status_idx on c_box_worklist (
	status,
	barcode)
with structure = btree,
	page_size = 2048,
	nocompression,
	key = (status),
	nonleaffill = 80,
	leaffill = 70,
	fillfactor = 80,
	location = (ii_database)
\p\g
create index cbw_time_stamp_idx on c_box_worklist (
	time_stamp,
	ts_sequence)
with structure = btree,
	page_size = 2048,
	nocompression,
	key = (time_stamp, ts_sequence),
	persistence,
	nonleaffill = 80,
	leaffill = 70,
	fillfactor = 80,
	location = (ii_database)
\p\g
create unique index idx_cbw_project_cid on c_buddy_worklist (
	project_cid,
	sample_id,
	test_cid)
with structure = btree,
	page_size = 2048,
	nocompression,
	key = (project_cid, sample_id, test_cid),
	unique_scope = statement,
	persistence,
	nonleaffill = 80,
	leaffill = 70,
	fillfactor = 80,
	location = (ii_database)
\p\g
create unique index idx_cbw_record_no on c_buddy_worklist (
	record_no)
with structure = btree,
	page_size = 2048,
	nocompression,
	key = (record_no),
	unique_scope = statement,
	persistence,
	nonleaffill = 80,
	leaffill = 70,
	fillfactor = 80,
	location = (ii_database)
\p\g
create index idx_cbw_test_cid on c_buddy_worklist (
	test_cid)
with structure = btree,
	page_size = 2048,
	nocompression,
	key = (test_cid),
	persistence,
	nonleaffill = 80,
	leaffill = 70,
	fillfactor = 80,
	location = (ii_database)
\p\g
create index idx_cbw_time_stamp on c_buddy_worklist (
	time_stamp,
	ts_sequence)
with structure = btree,
	page_size = 2048,
	nocompression,
	key = (time_stamp, ts_sequence),
	persistence,
	nonleaffill = 80,
	leaffill = 70,
	fillfactor = 80,
	location = (ii_database)
\p\g
create index idx_c_cal_test_test_cid on c_calibreag_test (
	test_cid)
with structure = btree,
	page_size = 2048,
	nocompression,
	key = (test_cid),
	persistence,
	nonleaffill = 80,
	leaffill = 70,
	fillfactor = 80,
	location = (ii_database)
\p\g
create index idx_ccm_cluster_cid on c_cluster_machine (
	cluster_cid)
with structure = btree,
	page_size = 2048,
	nocompression,
	key = (cluster_cid),
	persistence,
	nonleaffill = 80,
	leaffill = 70,
	fillfactor = 80,
	location = (ii_database)
\p\g
create unique index idx_ccm_mac_clustr on c_cluster_machine (
	machine_cid,
	cluster_cid)
with structure = btree,
	page_size = 2048,
	nocompression,
	key = (machine_cid, cluster_cid),
	unique_scope = statement,
	persistence,
	nonleaffill = 80,
	leaffill = 70,
	fillfactor = 80,
	location = (ii_database)
\p\g
create unique index idx_c_operator_external_full on c_operator (
	external_full)
with structure = btree,
	page_size = 2048,
	nocompression,
	key = (external_full),
	unique_scope = statement,
	persistence,
	nonleaffill = 80,
	leaffill = 70,
	fillfactor = 80,
	location = (ii_database)
\p\g
create unique index idx_c_operator_external_name on c_operator (
	external_name)
with structure = btree,
	page_size = 2048,
	nocompression,
	key = (external_name),
	unique_scope = statement,
	persistence,
	nonleaffill = 80,
	leaffill = 70,
	fillfactor = 80,
	location = (ii_database)
\p\g
create index c_permission_operator_cid on c_permission (
	operator_cid)
with structure = btree,
	page_size = 2048,
	nocompression,
	key = (operator_cid),
	persistence,
	nonleaffill = 80,
	leaffill = 70,
	fillfactor = 80,
	location = (ii_database)
\p\g
create index c_permission_program_cid on c_permission (
	program_cid)
with structure = btree,
	page_size = 2048,
	nocompression,
	key = (program_cid),
	persistence,
	nonleaffill = 80,
	leaffill = 70,
	fillfactor = 80,
	location = (ii_database)
\p\g
create index c_permission_project_cid on c_permission (
	project_cid)
with structure = btree,
	page_size = 2048,
	nocompression,
	key = (project_cid),
	persistence,
	nonleaffill = 80,
	leaffill = 70,
	fillfactor = 80,
	location = (ii_database)
\p\g
create index idx_c_rack_number_project on c_rack_number (
	project_cid)
with structure = btree,
	page_size = 2048,
	nocompression,
	key = (project_cid),
	persistence,
	nonleaffill = 80,
	leaffill = 70,
	fillfactor = 80,
	location = (ii_database)
\p\g
create index idx_c_reagent_lot_machine on c_reagent_lot (
	machine_cid)
with structure = btree,
	page_size = 2048,
	nocompression,
	key = (machine_cid),
	persistence,
	nonleaffill = 80,
	leaffill = 70,
	fillfactor = 80,
	location = (ii_database)
\p\g
create index idx_c_reagent_lot_test on c_reagent_lot (
	test_cid)
with structure = btree,
	page_size = 2048,
	nocompression,
	key = (test_cid),
	persistence,
	nonleaffill = 80,
	leaffill = 70,
	fillfactor = 80,
	location = (ii_database)
\p\g
create unique index idx_crj_external_name on c_retrieval_job (
	external_name)
with structure = btree,
	page_size = 2048,
	nocompression,
	key = (external_name),
	unique_scope = statement,
	persistence,
	nonleaffill = 80,
	leaffill = 70,
	fillfactor = 80,
	location = (ii_database)
\p\g
create index idx_crj_project_cid on c_retrieval_job (
	project_cid)
with structure = btree,
	page_size = 2048,
	nocompression,
	key = (project_cid),
	persistence,
	nonleaffill = 80,
	leaffill = 70,
	fillfactor = 80,
	location = (ii_database)
\p\g
create index idx_c_storage_details_locn on c_storage_details (
	location_cid)
with structure = btree,
	page_size = 2048,
	nocompression,
	key = (location_cid),
	persistence,
	nonleaffill = 80,
	leaffill = 70,
	fillfactor = 80,
	location = (ii_database)
\p\g
create unique index idx_c_tank_layout on c_tank_layout (
	rack_layout_cid,
	fill_order)
with structure = btree,
	page_size = 2048,
	nocompression,
	key = (rack_layout_cid, fill_order),
	unique_scope = statement,
	persistence,
	nonleaffill = 80,
	leaffill = 70,
	fillfactor = 80,
	location = (ii_database)
\p\g
create index idx_c_tank_map_storage on c_tank_map (
	storage_cid)
with structure = btree,
	page_size = 2048,
	nocompression,
	key = (storage_cid),
	persistence,
	nonleaffill = 80,
	leaffill = 70,
	fillfactor = 80,
	location = (ii_database)
\p\g
create index idx_c_tank_map_tsp on c_tank_map (
	tank_cid,
	storage_cid,
	position)
with structure = btree,
	page_size = 2048,
	nocompression,
	key = (tank_cid, storage_cid, position),
	persistence,
	nonleaffill = 80,
	leaffill = 70,
	fillfactor = 80,
	location = (ii_database)
\p\g
create unique index idx_ctd_result on c_test_definition (
	result_test_cid,
	test_position)
with structure = btree,
	page_size = 2048,
	nocompression,
	key = (result_test_cid, test_position),
	unique_scope = statement,
	persistence,
	nonleaffill = 80,
	leaffill = 70,
	fillfactor = 80,
	location = (ii_database)
\p\g
create index idx_lce_aliquot_type_cid on l_cryovial_expected (
	aliquot_type_cid)
with structure = btree,
	page_size = 2048,
	nocompression,
	key = (aliquot_type_cid),
	persistence,
	nonleaffill = 80,
	leaffill = 70,
	fillfactor = 80,
	location = (ii_database)
\p\g
create index idx_lcr_aliquot_type_cid on l_cryovial_retrieval (
	aliquot_type_cid)
with structure = btree,
	page_size = 2048,
	nocompression,
	key = (aliquot_type_cid),
	persistence,
	nonleaffill = 80,
	leaffill = 70,
	fillfactor = 80,
	location = (ii_database)
\p\g
create unique index idx_qcl_record_cid on qc_level (
	record_cid)
with structure = btree,
	page_size = 2048,
	nocompression,
	key = (record_cid),
	unique_scope = statement,
	persistence,
	nonleaffill = 80,
	leaffill = 70,
	fillfactor = 80,
	location = (ii_database)
\p\g
create unique index idx_qclot_matlevlot on qc_lot (
	material,
	qc_level,
	qc_id)
with structure = isam,
	page_size = 2048,
	nocompression,
	key = (material, qc_level, qc_id),
	persistence,
	fillfactor = 80,
	location = (ii_database)
\p\g
create unique index idx_qc_material_descrip on qc_material (
	material_descrip)
with structure = btree,
	page_size = 2048,
	nocompression,
	key = (material_descrip),
	unique_scope = statement,
	persistence,
	nonleaffill = 80,
	leaffill = 70,
	fillfactor = 80,
	location = (ii_database)
\p\g
create index idx_qctm_level_cid on qc_test_machine (
	level_cid)
with structure = btree,
	page_size = 2048,
	nocompression,
	key = (level_cid),
	persistence,
	nonleaffill = 80,
	leaffill = 70,
	fillfactor = 80,
	location = (ii_database)
\p\g
create index idx_qctm_machine_cid on qc_test_machine (
	machine_cid)
with structure = btree,
	page_size = 2048,
	nocompression,
	key = (machine_cid),
	persistence,
	nonleaffill = 80,
	leaffill = 70,
	fillfactor = 80,
	location = (ii_database)
\p\g
create index idx_qctm_test_cid on qc_test_machine (
	test_cid)
with structure = btree,
	page_size = 2048,
	nocompression,
	key = (test_cid),
	persistence,
	nonleaffill = 80,
	leaffill = 70,
	fillfactor = 80,
	location = (ii_database)
\p\g

	/* PERMISSIONS */
grant select on  archive_c_operator to group lab_group
\p\g
grant select on  archive_c_operator to group wizard_group
\p\g
grant select on  c_audit_trail to group wizard_group
\p\g
grant insert on  c_audit_trail to group wizard_group
\p\g
grant select on  c_audit_trail to group lab_group
\p\g
grant insert on  c_audit_trail to group lab_group
\p\g
grant select on  c_audit_trail to group thrive_group
\p\g
grant insert on  c_audit_trail to group thrive_group
\p\g
grant select on  c_audit_trail to search
\p\g
grant insert on  c_audit_trail to search
\p\g
grant select on  c_audit_trail to group hps_group
\p\g
grant insert on  c_audit_trail to group hps_group
\p\g
grant select on  c_audit_trail to harp
\p\g
grant insert on  c_audit_trail to harp
\p\g
grant select on  c_audit_trail to group sharp_group
\p\g
grant insert on  c_audit_trail to group sharp_group
\p\g
grant select on  c_audit_trail to kadoorie
\p\g
grant insert on  c_audit_trail to kadoorie
\p\g
grant select on  c_audit_trail to group ascend_group
\p\g
grant insert on  c_audit_trail to group ascend_group
\p\g
grant select on  c_block to group wizard_group
\p\g
grant select on  c_block to group lab_group
\p\g
grant select on  c_block to group thrive_group
\p\g
grant select on  c_block to search
\p\g
grant select on  c_block to group hps_group
\p\g
grant select on  c_block to harp
\p\g
grant select on  c_block to group sharp_group
\p\g
grant select on  c_block to kadoorie
\p\g
grant select on  c_block to group ascend_group
\p\g
grant   select   on  c_box_retrieval to group wizard_group
\p\g
grant   update   on  c_box_retrieval to group wizard_group
\p\g
grant   insert   on  c_box_retrieval to group wizard_group
\p\g
grant   select   on  c_box_retrieval to group lab_group
\p\g
grant   update   on  c_box_retrieval to group lab_group
\p\g
grant   insert   on  c_box_retrieval to group lab_group
\p\g
grant select on  c_box_size to group wizard_group
\p\g
grant select on  c_box_size to group lab_group
\p\g
grant insert on  c_box_size to group wizard_group
\p\g
grant update on  c_box_size to group wizard_group
\p\g
grant next on sequence  c_box_w_record_no to public
\p\g
grant select on  c_box_worklist to group wizard_group
\p\g
grant delete on  c_box_worklist to group wizard_group
\p\g
grant insert on  c_box_worklist to group wizard_group
\p\g
grant select on  c_box_worklist to group lab_group
\p\g
grant delete on  c_box_worklist to group lab_group
\p\g
grant insert on  c_box_worklist to group lab_group
\p\g
grant update on  c_box_worklist to group wizard_group
\p\g
grant update on  c_box_worklist to group lab_group
\p\g
grant next on sequence  c_buddy_w_record_no to public
\p\g
grant select on  c_buddy_worklist to group wizard_group
\p\g
grant delete on  c_buddy_worklist to group wizard_group
\p\g
grant insert on  c_buddy_worklist to group wizard_group
\p\g
grant select on  c_buddy_worklist to group lab_group
\p\g
grant delete on  c_buddy_worklist to group lab_group
\p\g
grant insert on  c_buddy_worklist to group lab_group
\p\g
grant select on  c_buddy_worklist to group thrive_group
\p\g
grant select on  c_buddy_worklist to search
\p\g
grant select on  c_buddy_worklist to group hps_group
\p\g
grant select on  c_buddy_worklist to harp
\p\g
grant select on  c_buddy_worklist to group sharp_group
\p\g
grant select on  c_buddy_worklist to kadoorie
\p\g
grant select on  c_buddy_worklist to group ascend_group
\p\g
grant update on  c_buddy_worklist to group wizard_group
\p\g
grant update on  c_buddy_worklist to group lab_group
\p\g
grant select on  c_calibreag_test to group wizard_group
\p\g
grant delete on  c_calibreag_test to group wizard_group
\p\g
grant insert on  c_calibreag_test to group wizard_group
\p\g
grant select on  c_calibreag_test to group lab_group
\p\g
grant   select   on  c_cluster_machine to group wizard_group
\p\g
grant   insert   on  c_cluster_machine to group wizard_group
\p\g
grant   select   on  c_cluster_machine to group lab_group
\p\g
grant   insert   on  c_cluster_machine to group lab_group
\p\g
grant select on  c_event_log to group wizard_group
\p\g
grant insert on  c_event_log to group wizard_group
\p\g
grant select on  c_event_log to group lab_group
\p\g
grant insert on  c_event_log to group lab_group
\p\g
grant select on  c_event_log to group thrive_group
\p\g
grant select on  c_event_log to search
\p\g
grant select on  c_event_log to group hps_group
\p\g
grant select on  c_event_log to harp
\p\g
grant select on  c_event_log to group sharp_group
\p\g
grant select on  c_event_log to kadoorie
\p\g
grant select on  c_event_log to group ascend_group
\p\g
grant select on  c_lease to group wizard_group
\p\g
grant delete on  c_lease to group wizard_group
\p\g
grant insert on  c_lease to group wizard_group
\p\g
grant select on  c_lease to group lab_group
\p\g
grant delete on  c_lease to group lab_group
\p\g
grant insert on  c_lease to group lab_group
\p\g
grant select on  c_lease to group thrive_group
\p\g
grant update on  c_lease to group thrive_group
\p\g
grant delete on  c_lease to group thrive_group
\p\g
grant insert on  c_lease to group thrive_group
\p\g
grant select on  c_lease to search
\p\g
grant update on  c_lease to search
\p\g
grant delete on  c_lease to search
\p\g
grant insert on  c_lease to search
\p\g
grant select on  c_lease to group hps_group
\p\g
grant update on  c_lease to group hps_group
\p\g
grant delete on  c_lease to group hps_group
\p\g
grant insert on  c_lease to group hps_group
\p\g
grant select on  c_lease to harp
\p\g
grant update on  c_lease to harp
\p\g
grant delete on  c_lease to harp
\p\g
grant insert on  c_lease to harp
\p\g
grant select on  c_lease to group sharp_group
\p\g
grant update on  c_lease to group sharp_group
\p\g
grant delete on  c_lease to group sharp_group
\p\g
grant insert on  c_lease to group sharp_group
\p\g
grant select on  c_lease to kadoorie
\p\g
grant update on  c_lease to kadoorie
\p\g
grant delete on  c_lease to kadoorie
\p\g
grant insert on  c_lease to kadoorie
\p\g
grant select on  c_lease to group ascend_group
\p\g
grant update on  c_lease to group ascend_group
\p\g
grant delete on  c_lease to group ascend_group
\p\g
grant insert on  c_lease to group ascend_group
\p\g
grant update on  c_lease to group wizard_group
\p\g
grant update on  c_lease to group lab_group
\p\g
grant select on  c_machine to group wizard_group
\p\g
grant select on  c_machine to group lab_group
\p\g
grant update on  c_machine to group wizard_group
\p\g
grant insert on  c_machine to group wizard_group
\p\g
grant select on  c_machine to group thrive_group
\p\g
grant select on  c_machine to search
\p\g
grant select on  c_machine to group hps_group
\p\g
grant select on  c_machine to harp
\p\g
grant select on  c_machine to group sharp_group
\p\g
grant select on  c_machine to kadoorie
\p\g
grant select on  c_machine to group ascend_group
\p\g
grant select on  c_next_cid to group wizard_group
\p\g
grant update on  c_next_cid to group wizard_group
\p\g
grant select on  c_next_cid to group lab_group
\p\g
grant update on  c_next_cid to group lab_group
\p\g
grant select on  c_next_cid to group thrive_group
\p\g
grant update on  c_next_cid to group thrive_group
\p\g
grant select on  c_next_cid to search
\p\g
grant update on  c_next_cid to search
\p\g
grant select on  c_next_cid to group hps_group
\p\g
grant update on  c_next_cid to group hps_group
\p\g
grant select on  c_next_cid to harp
\p\g
grant update on  c_next_cid to harp
\p\g
grant select on  c_next_cid to group sharp_group
\p\g
grant update on  c_next_cid to group sharp_group
\p\g
grant select on  c_next_cid to kadoorie
\p\g
grant update on  c_next_cid to kadoorie
\p\g
grant select on  c_next_cid to group ascend_group
\p\g
grant update on  c_next_cid to group ascend_group
\p\g
grant select on  c_object_name to group wizard_group
\p\g
grant select on  c_object_name to group lab_group
\p\g
grant update on  c_object_name to group wizard_group
\p\g
grant delete on  c_object_name to group wizard_group
\p\g
grant insert on  c_object_name to group wizard_group
\p\g
grant select on  c_operator to group wizard_group
\p\g
grant update on  c_operator to group wizard_group
\p\g
grant select on  c_operator to group lab_group
\p\g
grant update on  c_operator to group lab_group
\p\g
grant insert on  c_operator to group wizard_group
\p\g
grant select on  c_operator to group thrive_group
\p\g
grant select on  c_operator to search
\p\g
grant select on  c_operator to group hps_group
\p\g
grant select on  c_operator to harp
\p\g
grant select on  c_operator to group sharp_group
\p\g
grant select on  c_operator to kadoorie
\p\g
grant select on  c_operator to group ascend_group
\p\g
grant select on  c_permission to group wizard_group
\p\g
grant select on  c_permission to group lab_group
\p\g
grant update on  c_permission to group wizard_group
\p\g
grant insert on  c_permission to group wizard_group
\p\g
grant   insert   on  c_permission to group lab_group
\p\g
grant select on  c_project to group wizard_group
\p\g
grant select on  c_project to group lab_group
\p\g
grant insert on  c_project to group wizard_group
\p\g
grant update excluding (db_name, project_cid) on  c_project to 
group wizard_group
\p\g
grant select on  c_project to group thrive_group
\p\g
grant select on  c_project to search
\p\g
grant select on  c_project to group hps_group
\p\g
grant select on  c_project to harp
\p\g
grant select on  c_project to group sharp_group
\p\g
grant select on  c_project to kadoorie
\p\g
grant select on  c_project to group ascend_group
\p\g
grant select on  c_project to ingres
\p\g
grant select on  c_rack_capacity to group wizard_group
\p\g
grant select on  c_rack_capacity to group lab_group
\p\g
grant update on  c_rack_capacity to group wizard_group
\p\g
grant insert on  c_rack_capacity to group wizard_group
\p\g
grant select on  c_rack_number to group wizard_group
\p\g
grant insert on  c_rack_number to group wizard_group
\p\g
grant select on  c_rack_number to group lab_group
\p\g
grant insert on  c_rack_number to group lab_group
\p\g
grant   select   on  c_reagent_lot to group wizard_group
\p\g
grant   insert   on  c_reagent_lot to group wizard_group
\p\g
grant   select   on  c_reagent_lot to group lab_group
\p\g
grant   insert   on  c_reagent_lot to group lab_group
\p\g
grant   select   on  c_retrieval_job to group wizard_group
\p\g
grant   update   on  c_retrieval_job to group wizard_group
\p\g
grant   insert   on  c_retrieval_job to group wizard_group
\p\g
grant   select   on  c_retrieval_job to group lab_group
\p\g
grant   update   on  c_retrieval_job to group lab_group
\p\g
grant   insert   on  c_retrieval_job to group lab_group
\p\g
grant select on  c_slot_allocation to group wizard_group
\p\g
grant insert on  c_slot_allocation to group wizard_group
\p\g
grant select on  c_slot_allocation to group lab_group
\p\g
grant insert on  c_slot_allocation to group lab_group
\p\g
grant update on  c_slot_allocation to user limstester
\p\g
grant update on  c_slot_allocation to group lab_group
\p\g
grant update on  c_slot_allocation to group wizard_group
\p\g
grant select on  c_storage_details to group wizard_group
\p\g
grant select on  c_storage_details to group lab_group
\p\g
grant insert on  c_storage_details to group wizard_group
\p\g
grant update on c_storage_details to group wizard_group
\p\g
grant select on  c_tank_layout to group wizard_group
\p\g
grant select on  c_tank_layout to group lab_group
\p\g
grant select on  c_tank_map to group wizard_group
\p\g
grant select on  c_tank_map to group lab_group
\p\g
grant insert on  c_tank_map to group wizard_group
\p\g
grant update (status, valid_to, valid_from) on  c_tank_map to 
group wizard_group
\p\g
grant select on  c_test to group wizard_group
\p\g
grant select on  c_test to group lab_group
\p\g
grant insert on  c_test to group wizard_group
\p\g
grant select on  c_test to group thrive_group
\p\g
grant select on  c_test to search
\p\g
grant select on  c_test to group hps_group
\p\g
grant select on  c_test to harp
\p\g
grant select on  c_test to group sharp_group
\p\g
grant select on  c_test to kadoorie
\p\g
grant select on  c_test to group ascend_group
\p\g
grant update on  c_test to group wizard_group
\p\g
grant select on  c_test_definition to group wizard_group
\p\g
grant update on  c_test_definition to group wizard_group
\p\g
grant delete on  c_test_definition to group wizard_group
\p\g
grant insert on  c_test_definition to group wizard_group
\p\g
grant select on  c_test_definition to group lab_group
\p\g
grant select on  c_test_machine to group wizard_group
\p\g
grant update on  c_test_machine to group wizard_group
\p\g
grant delete on  c_test_machine to group wizard_group
\p\g
grant insert on  c_test_machine to group wizard_group
\p\g
grant select on  c_test_machine to group ascend_group
\p\g
grant select on  c_test_machine to group sharp_group
\p\g
grant select on  c_test_machine to group hps_group
\p\g
grant select on  c_test_machine to group thrive_group
\p\g
grant select on  c_test_machine to group lab_group
\p\g
grant select on  c_test_machine to kadoorie
\p\g
grant select on  c_test_machine to harp
\p\g
grant select on  c_test_machine to search
\p\g
grant select on  l_box_arrival to group lab_group
\p\g
grant update on  l_box_arrival to group lab_group
\p\g
grant insert on  l_box_arrival to group lab_group
\p\g
grant   select   on  l_box_arrival to group wizard_group
\p\g
grant   insert   on  l_box_arrival to group wizard_group
\p\g
grant update on  l_box_arrival to group wizard_group
\p\g
grant select on  l_box_arrival_event_history to group lab_group
\p\g
grant insert on  l_box_arrival_event_history to group lab_group
\p\g
grant   select   on  l_box_arrival_event_history to group 
wizard_group
\p\g
grant   insert   on  l_box_arrival_event_history to group 
wizard_group
\p\g
grant   update   on  l_box_arrival_event_history to group 
wizard_group
\p\g
grant   delete   on  l_box_arrival_event_history to group 
wizard_group
\p\g
grant   update   on  l_box_arrival_event_history to group 
lab_group
\p\g
grant   delete   on  l_box_arrival_event_history to group 
lab_group
\p\g
grant select on  l_box_expected to group lab_group
\p\g
grant insert on  l_box_expected to group lab_group
\p\g
grant   select   on  l_box_expected to group wizard_group
\p\g
grant   insert   on  l_box_expected to group wizard_group
\p\g
grant   update   on  l_box_expected to group wizard_group
\p\g
grant   delete   on  l_box_expected to group wizard_group
\p\g
grant   update   on  l_box_expected to group lab_group
\p\g
grant   delete   on  l_box_expected to group lab_group
\p\g
grant   select   on  l_cryovial_expected to group wizard_group
\p\g
grant   update   on  l_cryovial_expected to group wizard_group
\p\g
grant   insert   on  l_cryovial_expected to group wizard_group
\p\g
grant   select   on  l_cryovial_expected to group lab_group
\p\g
grant   update   on  l_cryovial_expected to group lab_group
\p\g
grant   insert   on  l_cryovial_expected to group lab_group
\p\g
grant   select   on  l_cryovial_retrieval to group wizard_group
\p\g
grant   update   on  l_cryovial_retrieval to group wizard_group
\p\g
grant   insert   on  l_cryovial_retrieval to group wizard_group
\p\g
grant   select   on  l_cryovial_retrieval to group lab_group
\p\g
grant   update   on  l_cryovial_retrieval to group lab_group
\p\g
grant   insert   on  l_cryovial_retrieval to group lab_group
\p\g
grant select on  qc_gate to group wizard_group
\p\g
grant insert on  qc_gate to group wizard_group
\p\g
grant select on  qc_gate to group lab_group
\p\g
grant insert on  qc_gate to group lab_group
\p\g
grant select on  qc_level to group wizard_group
\p\g
grant insert on  qc_level to group wizard_group
\p\g
grant update (status) on  qc_level to group wizard_group
\p\g
grant select on  qc_level to group lab_group
\p\g
grant select on  qc_lot to group wizard_group
\p\g
grant select on  qc_lot to group lab_group
\p\g
grant insert on  qc_lot to group wizard_group
\p\g
grant select on  qc_material to group wizard_group
\p\g
grant update on  qc_material to group wizard_group
\p\g
grant insert on  qc_material to group wizard_group
\p\g
grant select on  qc_material to group lab_group
\p\g
grant select on  qc_test to group wizard_group
\p\g
grant select on  qc_test to group lab_group
\p\g
grant update on  qc_test to group wizard_group
\p\g
grant insert on  qc_test to group wizard_group
\p\g
grant select on  qc_test_machine to group lab_group
\p\g
grant   select   on  qc_test_machine to group wizard_group
\p\g
grant   update   on  qc_test_machine to group wizard_group
\p\g
grant   insert   on  qc_test_machine to group wizard_group
\p\g


grant insert on  c_next_cid to group lab_group
\p\g
grant delete on  c_next_cid to group lab_group

\p\g
grant insert on  c_next_cid to group wizard_group
\p\g
grant delete on  c_next_cid to group wizard_group
\p\g

grant delete on  c_audit_trail to group wizard_group
\p\g
grant delete on  c_audit_trail to group lab_group
\p\g

grant insert on  c_audit_trail to group wizard_group
\p\g
grant insert on  c_audit_trail to group lab_group
\p\g


grant delete on  l_box_arrival_event_history to group wizard_group
\p\g
grant delete on  l_box_arrival_event_history to group lab_group
\p\g

grant insert on  l_box_arrival_event_history to group wizard_group
\p\g
grant insert on  l_box_arrival_event_history to group lab_group
\p\g

grant delete on  c_permission to group wizard_group
\p\g
grant delete on  c_permission to group lab_group
\p\g

grant insert on  c_permission to group wizard_group
\p\g
grant insert on  c_permission to group lab_group
\p\g

grant delete on  c_operator to group wizard_group
\p\g
grant delete on  c_operator to group lab_group
\p\g

grant insert on  c_operator to group wizard_group
\p\g
grant insert on  c_operator to group lab_group
\p\g

grant delete on  archive_c_operator to group wizard_group
\p\g
grant delete on  archive_c_operator to group lab_group
\p\g

grant insert on  archive_c_operator to group wizard_group
\p\g
grant insert on  archive_c_operator to group lab_group
\p\g

grant delete on  c_project to group wizard_group
\p\g
grant delete on  c_project to group lab_group
\p\g

grant insert on  c_project to group wizard_group
\p\g
grant insert on  c_project to group lab_group
\p\g

grant delete on  c_box_size to group wizard_group
\p\g
grant delete on  c_box_size to group lab_group
\p\g

grant insert on  c_box_size to group wizard_group
\p\g
grant insert on  c_box_size to group lab_group
\p\g

grant delete on  c_storage_details to group wizard_group
\p\g
grant delete on  c_storage_details to group lab_group
\p\g

grant insert on  c_storage_details to group wizard_group
\p\g
grant insert on  c_storage_details to group lab_group
\p\g

grant delete on  c_tank_map to group wizard_group
\p\g
grant delete on  c_tank_map to group lab_group
\p\g

grant insert on  c_tank_map to group wizard_group
\p\g
grant insert on  c_tank_map to group lab_group
\p\g

grant delete on  c_object_name to group wizard_group
\p\g
grant delete on  c_object_name to group lab_group
\p\g

grant insert on  c_object_name to group wizard_group
\p\g
grant insert on  c_object_name to group lab_group
\p\g

grant delete on  c_tank_layout to group wizard_group
\p\g
grant delete on  c_tank_layout to group lab_group
\p\g

grant insert on  c_tank_layout to group wizard_group
\p\g
grant insert on  c_tank_layout to group lab_group
\p\g

grant delete on  c_block to group wizard_group
\p\g
grant delete on  c_block to group lab_group
\p\g

grant insert on  c_block to group wizard_group
\p\g
grant insert on  c_block to group lab_group
\p\g

grant delete on  c_machine to group wizard_group
\p\g
grant delete on  c_machine to group lab_group
\p\g

grant insert on  c_machine to group wizard_group
\p\g
grant insert on  c_machine to group lab_group
\p\g


create table c_box_arrival(
	laptop_cid integer not null,
	box_arrival_id integer not null,
	project_cid integer not null,
	swipe_time date not null,
	box_name varchar(32) not null,
	status integer not null,
	first_barcode varchar(64) not null,
	first_position integer not null,
	last_barcode varchar(64) not null,
	last_position integer not null,
	tank_cid integer not null,
	rack_number varchar(12) not null,
	slot_position smallint not null,
	process_cid integer not null default 0,
	slot_cid integer not null default 0,
	key_hash varchar(40) not null default ' '
)
with duplicates,
nojournaling,
location = (ii_database),
security_audit=(table,norow)
\p\g

grant select on  c_box_arrival to group wizard_group
\p\g
grant select on  c_box_arrival to group lab_group
\p\g
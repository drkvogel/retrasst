-- fields

create table cbird.c_gcomms_field(
	id integer not null,
	id_filetype integer not null,
	name varchar(32) not null,
	tag varchar(16) not null,
	tagcol integer not null,
	valcol integer not null,
	subcol integer not null,
	itemtype integer not null,
	status integer not null default 0,
	tinsert ingresdate not null default 'now',
	tupdate ingresdate not null default 'now'
)
with duplicates,
nojournaling,
page_size = 2048,
location = (ii_database),
security_audit=(table,norow)
\p\g
modify cbird.c_gcomms_field to heap
with extend = 16,
	allocation = 4,
	page_size = 2048
\p\g
-- copy c_gcomms_field () from '/tmp/t_ldbc/copydb/c_gcomms_field.lust'
-- copy c_gcomms_field () from 'c_gcomms_field.lust'
-- with row_estimate = 0
-- \p\g
set journaling on cbird.c_gcomms_field
\p\g

-- describes CSV format
create table cbird.c_gcomms_filetype(
	id integer not null,
	name varchar(32) not null,
	description long varchar,
	type integer not null,
	firstrow integer not null default 0,
	delim varchar(4),
	subdelim varchar(4),
	date_format varchar(32) not null,
	status integer not null default 0,
	tinsert ingresdate not null default 'now',
	tupdate ingresdate not null default 'now'
)
with duplicates,
nojournaling,
page_size = 2048,
location = (ii_database),
security_audit=(table,norow)
\p\g
modify cbird.c_gcomms_filetype to heap
with extend = 16,
	allocation = 4,
	page_size = 2048
\p\g
-- copy c_gcomms_filetype () from '/tmp/t_ldbc/copydb/c_gcomms_filetype.lust'
-- copy c_gcomms_filetype () from 'c_gcomms_filetype.lust'
-- with row_estimate = 0
-- \p\g
set journaling on cbird.c_gcomms_filetype
\p\g

-- map of filetypes to analysers
create table cbird.c_gcomms_map(
	id_filetype integer not null,
	id_cluster integer not null default 0,
	status integer not null default 0,
	tinsert ingresdate not null default 'now',
	tupdate ingresdate not null default 'now'
)
with duplicates,
nojournaling,
page_size = 2048,
location = (ii_database),
security_audit=(table,norow)
\p\g
modify cbird.c_gcomms_map to heap
with extend = 16,
	allocation = 4,
	page_size = 2048
\p\g
-- copy c_gcomms_map () from '/tmp/t_ldbc/copydb/c_gcomms_map.lust'
-- copy c_gcomms_map () from 'c_gcomms_map.lust'
-- with row_estimate = 0
-- \p\g
set journaling on cbird.c_gcomms_map
\p\g
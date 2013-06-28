-- Note how NEW columns are flagged as such, enabling scripts that parse this file (e.g. dump_example.pl) to use this information.
create table valc_worklist (
    record_no               integer     not null default 0,
    machine_cid             integer     not null,
    barcode                 varchar(32) not null,
    test_cid                integer     not null,
    group_id                integer     not null default 0,
    category_id             integer     not null default 0,
    sample_id               integer     not null, 
    range_set               char(1)     not null default 'Y',
    project_cid             integer     not null,
    profile_id              integer     not null,
    profile_name            varchar(32) not null default '.',
    time_stamp              ingresdate  not null default 'now', 
    ts_sequence             smallint    not null default 0,
    in_date                 ingresdate  not null default 'now',
    trigger_id              integer     not null default 0,
    lower_trigger_limit     float       not null default 0,
    upper_trigger_limit     float       not null default 0,
    private_result          char(1)     not null default 'N',
    status                  char(1)     not null default 'Q',
    diluent                 float       not null default 0,
    buddy_result_id         integer     not null default 0,
    max_results             integer     not null default 1 -- NEW
);



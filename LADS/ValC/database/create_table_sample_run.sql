create table sample_run (
    run_id              integer     not null,
    is_open             integer     not null default 1,
    created_when        ingresdate  not null default 'now',
    closed_when         ingresdate,
    sequence_position   float       not null,
    fao_level_one       char(1)     not null default 'y'
);


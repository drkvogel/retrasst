create table sample_run (
    run_id              integer     not null,
    created_when        ingresdate  not null default 'now',
    closed_when         ingresdate,
    closed_by           integer,
    sequence_position   float       not null,
    fao_level_one       char(1)     not null default 'y',
    group_id            integer
);


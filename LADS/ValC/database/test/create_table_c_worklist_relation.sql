create table c_worklist_relation (
    rerun_id                integer     not null,
    parent_id               integer     not null,
    insert_when             ingresdate  not null,
    motivation              char        not null
);



create table buddy_database (
    buddy_sample_id         integer not null,
    barcode                 varchar(32),
    machine_cid             integer,
    date_analysed           ingresdate,
    action_flag             char(1),
    database_name           varchar(30),
    alpha_sample_id         integer,
    Person1_validate        integer,
    Person2_validate        integer,
    result_type             smallint default 4,
    Note_Exists             integer,
    Delete_status           smallint,
    update_when             ingresdate default 'now',
    rack_number             smallint,
    cup_number              smallint,
    test_type               varchar(20),
    sample_type             varchar(20),
    sample_run_id           integer not null default 0
)


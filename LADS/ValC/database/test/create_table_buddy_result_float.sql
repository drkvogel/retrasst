-- Note how NEW columns are flagged as such, enabling scripts that parse this file (e.g. dump_example.pl) to use this information.
create table buddy_result_float (
    buddy_result_id         integer not null,
    buddy_sample_id         integer,
    machine_cid             integer,
    Test_id                 integer,
    Res_value               float8,
    Seq                     integer,
    Action_Flag             char(1),
    Person1_Validate        integer,
    Person2_Validate        integer,
    Result_Edited           integer,
    Date_Analysed           ingresdate,
    Rerun_Status            integer,
    Trigger_Status          integer,
    Note_Exists             integer,
    Delete_status           smallint,
    process_id              integer,
    is_private              integer,
    res_text                varchar(20),
    reagent_id              varchar(10),
    reagent_lot             varchar(10),
    cuvette_num             smallint,
    unit_code               varchar(20),
    update_when             ingresdate default 'now' not null,
    cbw_record_no           integer default 0 not null -- NEW
)


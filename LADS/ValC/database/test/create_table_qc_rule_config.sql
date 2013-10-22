create table qc_rule_config (
    record_cid              integer not null,
    rule_cid                integer not null,
    test_cid                integer not null,
    machine_cid             integer not null,
    project_cid             integer not null,
    status                  smallint not null default 1,
    valid_from              ingresdate not null default 'now',
    valid_to                ingresdate not null default '01/01/2020'
)


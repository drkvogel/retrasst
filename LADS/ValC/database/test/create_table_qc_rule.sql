create table qc_rule (
    record_cid              integer not null,
    rule_cid                integer not null,
    return_type             smallint not null,
    internal_name           varchar(30) not null,
    script                  varchar(5000) not null,
    status                  smallint not null default 1,
    valid_from              ingresdate not null default 'now',
    valid_to                ingresdate not null default '01/01/2020'
)


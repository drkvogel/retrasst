create table c_object_name (
    object_cid              integer not null,
    process_cid             integer not null default 0,
    external_name           varchar(16) not null,
    external_full           varchar(64) not null,
    object_type             smallint not null default 15,
    status                  smallint not null default 1,
    time_stamp              ingresdate not null default 'now'
)


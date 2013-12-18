create table qc_lot (
    qc_id           integer                     not null,
    lot             varchar(16)                 not null,
    barcode_pattern varchar(24)                 not null, -- NEW
    lot_descrip     varchar(32)                 not null,
    arrival_date    ingresdate  default 'now'   not null,
    level_cid       integer                     not null,
    qc_status       smallint                    not null 
)


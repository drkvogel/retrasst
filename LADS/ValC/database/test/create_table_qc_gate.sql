create table qc_gate(
    qc_id       integer                     not null,
    machine_cid integer                     not null,
    test_cid    integer                     not null,
    mean        float8                      not null,
    stddev      float8                      not null,
    time_stamp  ingresdate  default 'now'   not null,
    record_cid  integer                     not null,
    status      smallint    default 1       not null,
    source      smallint                    not null 
)


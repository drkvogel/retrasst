create table buddy_worklist (
    BATCH_ID                integer,
    MACHINE_CID             integer,
    BARCODE                 varchar(32),
    DUPLICATE               integer,
    TEST_NAME               varchar(32),
    TEST_ID                 integer,
    SAMPLE_ID               integer, 
    RANGE_SET               char(1),
    MIN_VALUE               varchar(12),
    MAX_VALUE               varchar(12),
    STATUS                  char(1),
    PROJECT_CID             integer,
    profile_name            varchar(64),
    BUDDY_READ              char(1),
    IN_DATE                 ingresdate,
    Time_stamp              ingresdate,
    Trigger_id              integer,
    Trigger_upper_limit     varchar(16),
    Trigger_lower_limit     varchar(16),
    private_result          char(1),
    upper_warning           varchar(16),
    lower_warning           varchar(16),
    delete_status           smallint,
    category_id             integer
);



/* create LADS database on laptop */

/* 
    Do:

	sql dune_lust::t_ldbc <create_local_tables.sql >create_local_tables.lis
    
*/

/*  c_box_expected

    box_expected_id i4      A unique record ID     
    project_cid     i4      c_project The project expecting this box 
    box_name        v32     The name on the box 
    external_name   v32     The corresponding specimen entry (if applicable, otherwise '.') 
    status          i2       
    first_barcode   v20     The barcode on the first cryovial in the box 
    first_position  i2      The position of the first cryovial (normally 1) 
    last_barcode    v20     The barcode on the last cryovial in the box 
    first_position  i2      The position of the last cryovial (normally 100)  
*/

CREATE TABLE l_box_expected (
   box_expected_id  INT,
   project_cid      INT,
   box_name         VARCHAR(32),
   first_barcode    VARCHAR(20),
   first_position   INT,
   last_barcode     VARCHAR(20),
   last_position    INT
);
\g

CREATE TABLE properties (
    name 	VARCHAR(64),
    ival   	INT,
    sval	VARCHAR(256)
);

INSERT INTO properties (name, ival, sval) VALUES ('unexpected_id_start', -1, '');
INSERT INTO properties (name, ival, sval) VALUES ('unexpected_id_num',   -1, '');
\g
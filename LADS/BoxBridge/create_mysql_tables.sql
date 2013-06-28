/* create LADS database on laptop */

/* 
    Do:
    
    mysql -u root -p < create_mysql_tables.sql
    
    (password: Bugsy)
*/


DROP DATABASE lads;

CREATE DATABASE lads;

USE lads;

/*  c_box_expected

    box_expected_id  i4      A unique record ID     
    project_cid     i4      c_project The project expecting this box 
    box_name        v32     The name on the box 
    external_name   v32     The corresponding specimen entry (if applicable, otherwise '.') 
    status          i2       
    first_barcode   v20     The barcode on the first cryovial in the box 
    first_position  i2      The position of the first cryovial (normally 1) 
    last_barcode    v20     The barcode on the last cryovial in the box 
    first_position  i2      The position of the last cryovial (normally 100)  
*/

CREATE TABLE c_box_expected (
   box_expected_id  INT,
   project_cid      INT,
   box_name         VARCHAR(32),
   external_name    VARCHAR(32),
   status           SMALLINT,
   first_barcode    VARCHAR(20),
   first_position   INT,
   last_barcode     VARCHAR(20),
   last_position    INT
);

/*  c_project

    project_cid             i4
    external_name           v16
    external_full           v120
    valid_from              d/t
    valid_to                d/t
    status                  i2
    db_name                 v16
    info_url                v128
    database_version        i2
    database_minor_version  i2
*/

CREATE TABLE c_project (
    project_cid             INT,
    external_name           VARCHAR(16),
    external_full           VARCHAR(120),
    valid_from              DATETIME,
    valid_to                DATETIME,
    status                  SMALLINT,
    database_version        SMALLINT,
    database_minor_version  SMALLINT,
    db_name                 VARCHAR(16),
    info_url                VARCHAR(128)
);
    
/*  c_operator

    operator_cid    i4
    external_name   v16
    external_full   v128
    valid_from      d/t
    valid_to        d/t
    status          i2
    abilities       i4
    password_md5    v32
*/

CREATE TABLE c_operator (
    operator_cid    INT,
    external_name   VARCHAR(16),
    external_full   VARCHAR(128),
    valid_from      DATETIME,
    valid_to        DATETIME,
    status          SMALLINT,
    abilities       INT,
    password_md5    VARCHAR(32)
);

CREATE TABLE cids (
    unexpected_id_start INT,
    unexpected_id_num   INT
);

DROP USER 'lust'@'localhost';

CREATE USER 'lust'@'localhost';

GRANT SELECT, UPDATE, INSERT, DELETE, CREATE, DROP ON lads.* TO 'lust'@'localhost';
/*GRANT DROP ON * TO 'lust'@'localhost';*/
/*REVOKE DROP ON lads.c_box_expected FROM 'lust'@'localhost';
REVOKE DROP ON lads.cids FROM 'lust'@'localhost';*/
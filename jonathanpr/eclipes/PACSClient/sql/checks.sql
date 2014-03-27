
/*-------------------- ENCODING -------------------*/

/*
ALTER TABLE encoding DROP CONSTRAINT encoding_coded_as RESTRICT;
\p\g

ALTER TABLE encoding ADD CONSTRAINT encoding_coded_as
        CHECK ( ( encoding_id = 0 AND coded_as = 0 )
                OR coded_as IN (11,41) );
\p\g

ALTER TABLE encoding DROP CONSTRAINT encoding_structure RESTRICT;
\p\g

ALTER TABLE encoding ADD CONSTRAINT encoding_structure
        CHECK ( structure IN (1,2) );
\p\g
*/

/*--------------------- FIELD -------------------*/
/*
ALTER TABLE field DROP CONSTRAINT field_field_id RESTRICT;
\p\g
ALTER TABLE field ADD CONSTRAINT field_field_id
	CHECK ( field_id >= 60000 AND field_id <= 69999 );
\p\g

ALTER TABLE field DROP CONSTRAINT field_value_type_options RESTRICT;
\p\g

ALTER TABLE field ADD CONSTRAINT field_value_type_options
        CHECK ( value_type IN (1,11,21,22,31,41,51,61) );
\p\g
*/
/*---------- VAL_DATE ----------------------------------------*/
/*
ALTER TABLE val_date DROP CONSTRAINT val_date_arr_index RESTRICT;
\p\g
ALTER TABLE val_date ADD CONSTRAINT val_date_arr_index
        CHECK ( arr_index >= 0 );
\p\g

ALTER TABLE val_date DROP CONSTRAINT val_date_ins_index RESTRICT;
\p\g
ALTER TABLE val_date ADD CONSTRAINT val_date_ins_index
        CHECK ( ins_index >= 0 );
\p\g
*/
/*----------- VAL_INT ----------------------------------------*/
/*
ALTER TABLE val_int DROP CONSTRAINT val_int_arr_index RESTRICT;
\p\g
ALTER TABLE val_int ADD CONSTRAINT val_int_arr_index
        CHECK ( arr_index >= 0 );
\p\g

ALTER TABLE val_int DROP CONSTRAINT val_int_ins_index RESTRICT;
\p\g
ALTER TABLE val_int ADD CONSTRAINT val_int_ins_index
        CHECK ( ins_index >= 0 );
\p\g
*/
/*----------- VAL_REAL ----------------------------------------*/
/*
ALTER TABLE val_real DROP CONSTRAINT val_real_arr_index RESTRICT;
\p\g
ALTER TABLE val_real ADD CONSTRAINT val_real_arr_index
        CHECK ( arr_index >= 0 );
\p\g

ALTER TABLE val_real DROP CONSTRAINT val_real_ins_index RESTRICT;
\p\g
ALTER TABLE val_real ADD CONSTRAINT val_real_ins_index
        CHECK ( ins_index >= 0 );
\p\g
*/
/*----------- VAL_STRING --------------------------------------*/
/*
ALTER TABLE val_string DROP CONSTRAINT val_string_arr_index RESTRICT;
\p\g
ALTER TABLE val_string ADD CONSTRAINT val_string_arr_index
        CHECK ( arr_index >= 0 );
\p\g

ALTER TABLE val_string DROP CONSTRAINT val_string_ins_index RESTRICT;
\p\g
ALTER TABLE val_string ADD CONSTRAINT val_string_ins_index
        CHECK ( ins_index >= 0 );
\p\g
*/
/*----------- VAL_TIME ----------------------------------------*/
/*
ALTER TABLE val_time DROP CONSTRAINT val_time_arr_index RESTRICT;
\p\g
ALTER TABLE val_time ADD CONSTRAINT val_time_arr_index
        CHECK ( arr_index >= 0 );
\p\g

ALTER TABLE val_time DROP CONSTRAINT val_time_ins_index RESTRICT;
\p\g
ALTER TABLE val_time ADD CONSTRAINT val_time_ins_index
        CHECK ( ins_index >= 0 );
\p\g
*/

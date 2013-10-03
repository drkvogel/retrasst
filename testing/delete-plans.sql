\sql
set autocommit on
\nocontinue
\p\g

/* select *  from c_box_retrieval
\p\g

select *  from l_cryovial_retrieval
\p\g
 */
 
delete from l_cryovial_retrieval; 
\p\g

delete from c_box_retrieval;
\p\g

/* delete from cryovial_store s where exists 
(select 1 from cryovial c where s.cryovial_id = c.cryovial_id and aliquot_type_cid = -31781 and cryovial_barcode like '112155%')
\p\g */
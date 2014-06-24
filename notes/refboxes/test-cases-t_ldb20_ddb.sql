/* 
hack data for test cases
summary is in bodged-boxes.xlsx

sql vnode_vlab::t_ldb20 < test_cases-t_ldb20_ddb.sql 

*/

\sql
set autocommit on
\nocontinue
\p\g
-- 2205148

/* Delete from box_store where box_cid in (2193361, 2464829, 2571741, 2706572, 2721846);
Delete from box_store where box_cid in (2725250, 2725251, 2731805, 2750504, 2758457);
Delete from box_store where box_cid in (2758458 , 2758459 , 2758460 , 2793174, 6450); */

-- select * from box_store where box_cid in (2193361, 2464829, 2571741, 2706572, 2721846, 2725250, 2725251, 2731805, 2750504, 2758457, 2758458 , 2758459 , 2758460 , 2793174, 6450) order by external_name
-- select * from box_store where box_cid in (2205148, 2721846, 2706572, 2750504, 2793174, 2725250, 2758457, 1507, 2758458 , 2725251, 2193361, 2464829, 2571741, 2758459, 2758460, 2731805)

update box_store set status=99 where box_cid in (2193361, 2464829, 2571741, 2706572, 2721846);
update box_store set status=99 where box_cid in (2725250, 2725251, 2731805, 2750504, 2758457);
update box_store set status=99 where box_cid in (2758458 , 2758459 , 2758460 , 2793174, 6450);

-- select * from box_name where box_cid in (2193361, 2464829, 2571741, 2706572, 2721846, 2725250, 2725251, 2731805, 2750504, 2758457, 2758458 , 2758459 , 2758460 , 2793174) order by box_cid
-- select * from box_name where box_cid in (2205148, 2721846, 2706572, 2750504, 2793174, 2725250, 2758457, 1507, 2758458 , 2725251, 2193361, 2464829, 2571741, 2758459, 2758460, 2731805) order by external_name

Update box_name set status = 2 where box_cid in (2193361, 2464829, 2571741, 2706572, 2721846);
Update box_name set status = 2 where box_cid in (2725250, 2725251, 2731805, 2750504, 2758457);
Update box_name set status = 2 where box_cid in (2758458 , 2758459 , 2758460 , 2793174 );
\p\g

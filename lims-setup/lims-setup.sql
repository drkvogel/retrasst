/* LIMS setup for cbird@Q108 */

\sql
set autocommit on
\nocontinue
\p\g

-- select * from c_machine where external_name = 'Q108'
update c_machine set status = 2, valid_to = date('now') + date('1 year') where external_name = 'Q108'
\p\g

-- select * from c_operator where operator_cid = -31438
update c_operator set valid_to = date('now') + date('1 year'), status = 1, password_md5 = '' where operator_cid = -31438
\p\g

-- select * from c_permission where operator_cid = -31438
insert into c_permission (permission_cid, operator_cid, program_cid, project_cid, page_number, status, machine_cid)
values ((select next_cid from c_next_cid), -31438, 100, 0, 0, 0, -1108607)
\p\g
#!/bin/sh
alterdb user_dbname_test -disable_mvcc
sql user_dbname_test < tables.sql
sql user_dbname_test < checks.sql
sql user_dbname_test < init.sql
sql user_dbname_test < constr.sql
sql user_dbname_test < speed.sql
sql user_dbname_test < grant.sql
sql user_dbname_test < comment.sql
sql user_dbname_test < fill_attribs_table.sql
sql user_dbname_test < fill_ctsu_user_table.sql

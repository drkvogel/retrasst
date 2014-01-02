#!/bin/sh
alterdb erg_attila_test -disable_mvcc
sql erg_attila_test < tables.sql
sql erg_attila_test < checks.sql
sql erg_attila_test < init.sql
sql erg_attila_test < constr.sql
sql erg_attila_test < speed.sql
sql erg_attila_test < grant.sql
sql erg_attila_test < comment.sql

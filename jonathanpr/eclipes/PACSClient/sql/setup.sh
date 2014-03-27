#!/bin/sh
alterdb biocore_pacsclinicdata_test -disable_mvcc
sql biocore_pacsclinicdata_test < tables.sql
sql biocore_pacsclinicdata_test < checks.sql
sql biocore_pacsclinicdata_test < init.sql
sql biocore_pacsclinicdata_test < constr.sql
sql biocore_pacsclinicdata_test < speed.sql
sql biocore_pacsclinicdata_test < grantu.sql
sql biocore_pacsclinicdata_test < comment.sql
sql biocore_pacsclinicdata_test < fillPatientTable.sql

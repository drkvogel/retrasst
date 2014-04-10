#!/bin/sh
alterdb biocore_pacs_live -disable_mvcc
sql biocore_pacs_live < tables.sql
sql biocore_pacs_live < checks.sql
sql biocore_pacs_live < init.sql
sql biocore_pacs_live < constr.sql
sql biocore_pacs_live < speed.sql
sql biocore_pacs_live < grantu.sql
sql biocore_pacs_live < comment.sql
sql biocore_pacs_live < fillPatientTable.sql

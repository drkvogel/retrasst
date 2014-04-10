#!/bin/bash

# this file must have unix-style (lf) endings to run on cygwin

echo "Creating test cases..."
VNODE='vnode_vlab_64'
DBC='t_ldbc'
DBP='t_ldb20'

DELETE="./delete-plans.sql"
CENTRAL="./test-cases-t_ldbc.sql"
PROJECT="./test-cases-t_ldbx.sql"

echo $DELETE
#sql $VNODE::$DBC < $DELETE

echo $CENTRAL
#sql $VNODE::$DBC < $CENTRAL

echo $PROJECT
sql $VNODE::$DBP < $PROJECT

echo "Done."
exit 0
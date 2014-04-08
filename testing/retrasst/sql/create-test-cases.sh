#!/bin/bash

# this file must have unix-style (lf) endings to run on cygwin

echo "Creating test cases..."
VNODE='vnode_vlab'
DBC='t_ldbc'
DBP='t_ldb20'
FILE1="./test-cases-t_ldbc.sql"
FILE2="./test-cases-t_ldbx.sql"
FILE3="./delete-plans.sql"
echo "$FILE1,$FILE2"
sql $VNODE::$DBC < $FILE1
sql $VNODE::$DBP < $FILE2
sql $VNODE::$DBC < $FILE3
echo "Done."
exit 0
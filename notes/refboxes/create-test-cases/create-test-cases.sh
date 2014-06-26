#!/bin/bash
echo "Creating test cases..."
VNODE='vnode_vlab'
DB1='t_ldbc'
DB2='t_ldb20'
# DB1='test_ldbc'
# DB2='test_ldb20'
FILE1='./test-cases-t_ldbc.sql'
FILE2='./test-cases-t_ldb20_ddb.sql'
sql $VNODE::$DB1 < $FILE1
sql $VNODE::$DB2 < $FILE2
echo "Done."
exit 0
#!/bin/bash

#echo $#" arguments"
VNODE='vnode_vlab'
DB1='t_ldbc'
DB2='t_ldb20'

#BOX_CID='2205148'
#BOX_CID='2721846'
#BOX_CID='2706572'
#BOX_CID='2750504'
#BOX_CID='2793174'
#BOX_CID='2725250'
#BOX_CID='2758457'
#BOX_CID='1507' #?? test again
#BOX_CID='2758458'
#BOX_CID='2725251'
#BOX_CID='2193361'
#BOX_CID='7464829' # deliberately wrong
#BOX_CID='2464829' # correct
#BOX_CID='2571741'
#BOX_CID='2758459'
BOX_CID='2758460'
# BOX_CID='2731805'

if [ $# -gt 0 ] ;
then
    BOX_CID=$1
fi

SQL_BOX_STORE="select record_id, rack_cid, slot_position, status, time_stamp, removed from box_store where box_cid = $BOX_CID order by time_stamp desc"
SQL_BOX_NAME="select box_cid, box_capacity, external_name, status, slot_cid, time_stamp from box_name where box_cid = $BOX_CID"
SQL_EVENT_HISTORY="select event_cid, operator_cid, event_date, log_date from box_event_history where box_id = $BOX_CID order by event_date desc"
SQL_L_B_A="select box_arrival_id, time_stamp, box_name, status, tank_cid, rack_number, slot_position from l_box_arrival where box_arrival_id = $BOX_CID"
SQL_L_B_A2="select first_position, first_barcode, last_position, last_barcode from l_box_arrival where box_arrival_id = $BOX_CID"

# only in >= 10.0.1: format columns
# \colformat first_barcode c10
# but can't get to work

# sql -s # silent: surpress some messages
# sql -S # more silent: output data only (no column headings)
# \G # vertical listing?

echo
echo "box_store:"
echo "$SQL_BOX_STORE\G" | sql -s $VNODE::$DB2
echo
echo "box_name:"
echo "$SQL_BOX_NAME\G" | sql -s $VNODE::$DB2
echo
echo "l_box_arrival:"
echo "$SQL_L_B_A\G" | sql -s $VNODE::$DB1
echo
echo "l_box_arrival:"
echo "$SQL_L_B_A2\G" | sql -s $VNODE::$DB1
echo
echo "box_event_history:"
echo "$SQL_EVENT_HISTORY\G" | sql -s $VNODE::$DB2
echo
exit 0
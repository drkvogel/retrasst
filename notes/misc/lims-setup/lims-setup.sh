#!/bin/bash
# lims-setup.sh
# set up login and grants for cbird testing
# this file must have unix-style (lf) endings to run on cygwin
# no 'mail' or 'sendmail' on Cygwin!

echo "Setting up user cbird@Q108"
#VNODE='vnode_vlab_64'
VNODE='vnode_labdev_64'
DBC='t_ldbc'
SETUP="./lims-setup.sql"

# setup LIMS login for cbird@Q108
sql $VNODE::$DBC < $SETUP

# ask for delete perms 
EMLSUBJECT="delete grants on vlabdev"
EMLADDRESS="Ingres@ctsu.ox.ac.uk"
EMLMESSAGE=<<EOF
Marty/Geraint, 
 
Once again I will need delete permissions please on vlabdev: 

t_ldbc: 
 
delete from c_box_retrieval 
delete from l_cryovial_retrieval 
 
t_ldb20: 
 
delete from box_name 
delete from cryovial_store

cheers 
Chris
EOF

#/bin/mail -s "$EMLSUBJECT" "$EMLADDRESS" < $EMLMESSAGE

echo "Done."
exit 0


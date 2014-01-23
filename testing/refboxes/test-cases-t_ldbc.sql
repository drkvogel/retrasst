/*
add some test cases to l_box_arrival
summary is in bodged-boxes.xlsx

  sql vnode_vlab::t_ldbc < test-cases-t_ldbc.sql

all boxes are hps2-thrive ([t_]ldb20[_ddb])
  
LPDbBoxName::Status { EMPTY = 0, IN_USE = 1, CONFIRMED = 2, ANALYSED = 3, IN_TANK = 4, DESTROYED = 8, DELETED = 99 };  
  
LPDbBoxNames::readCurrent()
select, "select * from box_name where status in ( LPDbBoxName::EMPTY [0], LPDbBoxName::IN_USE [1] ) or box_capacity > 0 order by box_cid"

LPDbBoxNames::readFilled()
select * from box_name b where b.status in ( LPDbBoxName::CONFIRMED [2], LPDbBoxName::ANALYSED [3]) order by box_cid"
  
2205148 (the one already IN_TANK has status 4 so not read by readCurrent() or readFilled()

so
        LPDbBoxName boxName = *(boxes.findByID(box->box_arrival_id));
        boxName.setStatus(LPDbBoxName::Status::IN_TANK);
        boxName.saveRecord(qp);
will throw an exception - test for null first, if found, check record exists with select

project db was ldb20_ddb - not t_ldb20_ddb
LQuery qp = Util::projectQuery(box->project_cid, true);
what is box->project_cid?
records come from l_box_arrival
project_cid is all but one of l_b_a where status = 7 (referred) is -149662 - which is dev_hps2-thrive, t_ldb20

testing

select * from box_store where box_cid = 
select * from box_name where box_cid = 
select * from l_box_arrival where box_arrival_id = 
select * from box_event_history where box_id = 

boxStore.saveRecord(qp);
2706572  LPDbBoxType 101 not found
  
*/

\sql
set autocommit on
\nocontinue
\p\g

-- select * from l_box_arrival where box_arrival_id in (2205148, 2721846, 2706572, 2750504, 2793174, 2725250, 2758457, 1507, 2758458 , 2725251, 2193361, 2464829, 2571741, 2758459, 2758460, 2731805) order by box_name
-- (same order as bodged-boxes.xlsx)

Update l_box_arrival set status = 7 where box_arrival_id =2205148;
Update l_box_arrival set status = 7, last_position=72  where box_arrival_id = 2721846;
Update l_box_arrival set status = 7, tank_cid = 0 where box_arrival_id = 2750504;
Update l_box_arrival set status = 7, slot_position = 8 where box_arrival_id = 2793174 ;
Update l_box_arrival set status = 7, box_name = 'HPS2-THRIVE EDTA_1_LT 2725250' where box_arrival_id = 2725250;
Update l_box_arrival set status = 7, tank_cid = -943330  where box_arrival_id = 2758457;
Update l_box_arrival set status = 7, tank_cid = -319598, rack_number='B1', slot_position = 8 where box_arrival_id = 1507;
Update l_box_arrival set status = 7, tank_cid = -943331, rack_number='C5' where box_arrival_id = 2758458;
Update l_box_arrival set status = 7, first_barcode = '112143308' where box_arrival_id = 2725251;
Update l_box_arrival set status = 7 where box_arrival_id = 2193361;
-- delete from l_box_arrival where box_arrival_id = 7464829; -- to avoid clash on next update when 2464829 is changed to 7464829
Update l_box_arrival set status = 7, box_arrival_id = 7464829 where box_arrival_id = 2464829;
Update l_box_arrival set status = 7, box_name ='HPS2-THRIVE FL/OX_NLT 2577741' where box_arrival_id = 2571741;
Update l_box_arrival set status = 7, rack_number = 'B5' where box_arrival_id = 2758459 ;
Update l_box_arrival set status = 7, slot_position = 0 where box_arrival_id = 2758460 ;
Update l_box_arrival set status = 7, last_barcode = '112109745' where box_arrival_id = 2731805;
Update l_box_arrival set status = 7, first_position=0 where box_arrival_id = 2706572;
\p\g

/* t_ldb20_ddb */
/*
Delete from box_store where box_cid in (2193361, 2464829, 2571741, 2706572, 2721846);
Delete from box_store where box_cid in (2725250, 2725251, 2731805, 2750504, 2758457);
Delete from box_store where box_cid in (2758458 , 2758459 , 2758460 , 2793174, 6450);

Update box_name set status = 2 where box_cid in (2193361, 2464829, 2571741, 2706572, 2721846);
Update box_name set status = 2 where box_cid in (2725250, 2725251, 2731805, 2750504, 2758457);
Update box_name set status = 2 where box_cid in (2758458 , 2758459 , 2758460 , 2793174 );
*/
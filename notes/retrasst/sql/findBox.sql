/*

select * from c_retrieval_job where retrieval_cid = 1086786

project_cid = -659 (HPS, t_ldb3)

project_cid "project boxes/vials are from (-1: not known; 0: more than one project)"

has each SampleRow got project_cid stored somewhere?
LPDbCryovial
LPDbCryovialStore
LCDbCryovialRetrieval
.
no to all of these... 

c_box_name has it (used in collect samples) - plan samples should use it and record project_cid in SampleRow...


*/

-- proj_id 155908000
-- box_id  0

SELECT
    v.external_full as vessel_name, shelf_number, r.position as rack_pos,
    r.external_name as structure, slot_position, m.position as tank_pos, 
    l.external_name as site_name
FROM
    c_slot_allocation bs, c_rack_number r, c_tank_map m, c_object_name v, c_object_name l 
WHERE
    box_cid = 
    AND bs.status in (1, 2, 6)
    AND bs.rack_cid = r.rack_cid 
    AND r.tank_cid = m.tank_cid 
    AND m.storage_cid = v.object_cid
    AND m.location_cid = l.object_cid 
    AND m.status = 0
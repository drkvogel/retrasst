/* select boxes */

-- boxes that fail integrity test
SELECT * FROM l_box_arrival a 
	WHERE status <> 99  -- not deleted
	AND project_cid = ( -- in particular test or dev projects?
		SELECT project_cid FROM c_project
		WHERE db_name = LEFT(dbmsinfo('database'), LENGTH(dbmsinfo('database')) - 4)
	)
	AND NOT EXISTS ( -- not things that are correct
	  SELECT * FROM box_name b, c_slot_allocation s, c_rack_number r, c_tank_map t
	  WHERE b.box_cid = s.box_cid
		AND s.rack_cid = r.rack_cid  
		AND r.tank_cid = t.tank_cid
		AND UPPER(b.external_name) = UPPER(a.box_name) 
		AND a.project_cid = s.project_cid
		AND a.slot_position = s.slot_position
		AND a.rack_number = r.external_name
		AND a.tank_cid = t.tank_cid 
	);

-- boxes that have not been marked as dealt with - may be the same?
SELECT * FROM l_box_arrival WHERE status <> 99 AND status <> 4

/* Use case

storage sync should be run first, which
  gathers a list of full and processed 
boxes taken to storage (cowley or worminhall?)
most processed by 
*/

/* matching boxes */

SELECT b.box_cid, b.external_name, b.status, bs.status AS storestatus, bs.slot_position AS slot,
             s1.cryovial_position AS pos1, c1.cryovial_barcode AS barc1,
             s2.cryovial_position AS pos2, c2.cryovial_barcode AS barc2
             FROM box_name b LEFT JOIN box_store bs
             ON b.box_cid = bs.box_cid AND bs.status != 99, 
             cryovial c1, cryovial_store s1, cryovial c2, cryovial_store s2
             WHERE b.box_cid = s1.box_cid AND b.box_cid = s2.box_cid
             AND c1.cryovial_id = s1.cryovial_id AND c2.cryovial_id = s2.cryovial_id
             AND b.box_cid = :bid AND s1.cryovial_position = :fpos AND s2.cryovial_position = :lpos
             ORDER BY b.box_cid

-- eg

-- box_arrival_id 119369
-- first_position 1
-- last_position  100

-- project_cid -1078608, dev_stics, t_ldb26

SELECT b.box_cid, b.external_name, b.status, bs.status AS storestatus, bs.slot_position AS slot,
             s1.cryovial_position AS pos1, c1.cryovial_barcode AS barc1,
             s2.cryovial_position AS pos2, c2.cryovial_barcode AS barc2
             FROM box_name b LEFT JOIN box_store bs
             ON b.box_cid = bs.box_cid AND bs.status != 99, 
             cryovial c1, cryovial_store s1, cryovial c2, cryovial_store s2
             WHERE b.box_cid = s1.box_cid AND b.box_cid = s2.box_cid
             AND c1.cryovial_id = s1.cryovial_id AND c2.cryovial_id = s2.cryovial_id
             AND b.box_cid = 119369 AND s1.cryovial_position = 1 AND s2.cryovial_position = 100
             ORDER BY b.box_cid

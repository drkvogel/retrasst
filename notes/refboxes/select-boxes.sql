

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

# Slow loading

Selecting all 63 rows for one box (rj_box_cid = 4145775) from a temp table [*] took about 1 minute. 

There are 43 distinct rj_box_cid records in retrieval_assistant_temp_4145931_2014_01_16T16_22_09

There are 1674 rows in total in this table. 1674 / 43 = 38.9, so there is an average of about 39 rows for each rj_box_cid in this table. If all boxes had 63 rows in this dataset, there would be 1674 / 63 = 26.75 boxes.

So if the amount of time taken to retrieve a box was proportional to the number of rows per box and the number of boxes, you would expect all boxes to take between 1 minute * 43 = 43 minutes and 1 minute * 63 = 63 minutes. But it doesn't, it takes 7 minutes, every time.

---

## Nick's new queries

The following query gives you almost all the information you need without a temporary table.

	select 
		section, position, cbr.rj_box_cid, cbr.status,  
		cs.box_cid as source_id, sb.external_name as source_box, cs.tube_position as source_pos, 
		cbr.box_id as dest_id, db.external_name as dest_box, slot_number as dest_pos, 
		lcr.process_cid, lcr.status, lcr.cryovial_barcode, lcr.aliquot_type_cid 
	from 
		c_box_retrieval cbr, l_cryovial_retrieval lcr, c_box_name db, c_box_name sb, cryovial c, cryovial_store cs 
	where 
		cbr.rj_box_cid = lcr.rj_box_cid and 
		cbr.box_id = db.box_cid and 
		c.cryovial_barcode = lcr.cryovial_barcode and 
		c.aliquot_type_cid = lcr.aliquot_type_cid and 
		cs.cryovial_id = c.cryovial_id and 
		cbr.retrieval_cid = cs.retrieval_cid and 
		cs.box_cid = sb.box_cid and 
		cbr.retrieval_cid = -363636 
	order by 
		section, position

This still takes getting on for a minute.  Since the slowdown seems to appear when you add in the link to cryovial_store, I suspect your version is slower because it does that twice.

Copying the relevant data into the central database allows a simpler query:

	select 
		section, position, cbr.rj_box_cid, cbr.status, 
		old_box_id as source_id, sb.external_name as source_box, old_position as source_pos, 
		cbr.box_id as dest_id, db.external_name as dest_box, new_position as dest_pos, 
		lcr.process_cid, lcr.status, lcr.cryovial_barcode, lcr.aliquot_type_cid 
	from 
		c_box_retrieval cbr, l_cryovial_retrieval lcr, c_box_name db, c_box_name sb 
	where 
		cbr.rj_box_cid = lcr.rj_box_cid and 
		cbr.box_id = db.box_cid and 
		old_box_id = sb.box_cid and 
		cbr.retrieval_cid = -363636 
	order by 
		section, position

This give the near-instantaneous response you might expect.  Given Geraint's dire warnings about STAR in later versions of Ingres, I think that's the way to go


---

retrieval_assistant_temp_4145931_2014_01_16T16_22_09 

[*]
	 SELECT g.retrieval_cid, g.chunk, g.rj_box_cid, g.cbr_status, g.dest_pos, g.lcr_slot, 
	 g.lcr_procid, g.lcr_status, g.box_id AS dest_id,     
	 c.cryovial_barcode, c.sample_id, c.aliquot_type_cid, c.note_exists AS cryovial_note,     
	 s1.cryovial_id, s1.note_exists, s1.retrieval_cid, s1.box_cid, s1.status, s1.tube_position, s1.record_id,     
	 s1.status, s1.tube_position, s1.note_exists AS cs_note,     
	 b1.external_name AS src_box, b2.external_name AS dest_name, s2.tube_position AS slot_number, s2.status AS dest_status 
	 FROM retrieval_assistant_temp_4145931_2014_01_16T16_22_09 g, cryovial c, cryovial_store s1, cryovial_store s2, box_name b1, box_name b2 
	 WHERE     c.cryovial_barcode = g.cryovial_barcode     
	 AND c.aliquot_type_cid = g.aliquot_type_cid     
	 AND s1.cryovial_id = c.cryovial_id     
	 AND s1.retrieval_cid = g.retrieval_cid     
	 AND b2.box_cid = g.box_id     
	 AND b1.box_cid = s1.box_cid     
	 AND s2.cryovial_id = c.cryovial_id     
	 AND b2.box_cid = s2.box_cid 
	 AND g.rj_box_cid = 4145775
	 ORDER BY     s1.retrieval_cid, chunk, g.rj_box_cid, c.aliquot_type_cid DESC

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

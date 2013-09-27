/* -- ?
SELECT  s1.cryovial_id, s1.note_exists, s1.retrieval_cid, s1.box_cid, s1.status, s1.cryovial_position,  cryovial_barcode, t.external_name AS aliquot,  b1.box_cid as source_id,  b1.external_name as source_name,  s1.cryovial_position as source_pos,  s2.box_cid as dest_id,  b2.external_name as dest_name,  s2.cryovial_position as dest_pos FROM  cryovial c, cryovial_store s1, box_name b1,  cryovial_store s2, box_name b2,  c_object_name t WHERE  c.cryovial_id = s1.cryovial_id AND  b1.box_cid = s1.box_cid AND  s1.cryovial_id = s2.cryovial_id AND  s2.status = 0 AND  b2.box_cid = s2.box_cid AND  t.object_cid = aliquot_type_cid 
AND  s1.retrieval_cid = -1180114
ORDER BY  cryovial_barcode
*/





/*

## boxes 

name, current structure and expected location of each box. the location should include site+position+name+layout

## samples

If no destination boxes have been defined, ask for the box type and create suitable entries in box_name

c_retrieval_job 
    primary_aliquot
    secondary_aliquot
cryovial barcode, destination box+cryo position, current box+cryo position+structure+location of the primary and secondary aliquots

some samples have destination boxes 
    SGVIALS_BARCODE, SGVIALS_DESTBOX, SGVIALS_DESTPOS, SGVIALS_CURRBOX, SGVIALS_CURRPOS, SGVIALS_STRUCTURE, SGVIALS_LOCATION, // site/vessel/
    // secondary aliquots if defined?
    SGVIALS_NUMCOLS} sg_vials_cols;

/* show retrieval jobs */
select * from c_retrieval_job 
where 
    status in  (0 /* NEW_JOB */,  1 /* INPROGRESS */)
    and job_type in (2 /* BOX_RETRIEVAL */, 3 /* BOX_DISCARD */, 4 /* SAMPLE_RETRIEVAL */, 5 /* SAMPLE_DISCARD */)

/* load samples */
SELECT
   cs.cryovial_id, 
   cs.note_exists, 
   cs.retrieval_cid, 
   cs.box_cid, 
   cs.status, 
   cs.cryovial_position,
   cryovial_barcode, 
   b.external_name AS box,
   t.external_name AS aliquot,
   s.external_name AS site, 
   m.position, 
   v.external_full AS vessel,
   shelf_number, r.external_name AS rack, 
   bs.slot_position
FROM
   cryovial c, cryovial_store cs, box_name b, box_store bs, c_rack_number r, c_tank_map m, 
   c_object_name s,   /* site */
   c_object_name v,   /* vessel */
   c_object_name t    /* aliquot? */
WHERE
   c.cryovial_id = cs.cryovial_id AND
   b.box_cid = cs.box_cid AND
   b.box_cid = bs.box_cid AND
   bs.status = 6 AND    /* SLOT_CONFIRMED */
   t.object_cid = aliquot_type_cid AND
   bs.rack_cid = r.rack_cid AND
   r.tank_cid = m.tank_cid AND
   s.object_cid = location_cid AND
   v.object_cid = storage_cid AND
   cs.retrieval_cid = :jobID) /* e.g. -1015 */
   
/* join destination boxes version */
/* BUT!: 'outer join not allowed in distributed Ingres 

so, either:

 * load samples/boxes as normal, and line by line do an normal join and see if there are any rows
 
or

 * check the boxes/vials are in sequence up to the box capacity - if there are any gaps, a dest box is missing?
 
*/

SELECT
    cs.cryovial_id, 
    cs.note_exists, 
    cs.retrieval_cid, 
    cs.box_cid, 
    cs.status, 
    cs.cryovial_position,
    cryovial_barcode, 
    b.external_name AS box,
    t.external_name AS aliquot,
    s.external_name AS site, 
    m.position, 
    v.external_full AS vessel,
    shelf_number, r.external_name AS rack, 
    bs.slot_position
FROM
    cryovial c, cryovial_store cs, box_name b, box_store bs, c_rack_number r, c_tank_map m, 
    c_object_name s,   /* site */
    c_object_name v,   /* vessel */
    c_object_name t,    /* aliquot? */
    cryovial_store s1
LEFT JOIN 
    cryovial c ON c.cryovial_id=s1.cryovial_id
LEFT JOIN  
    box_name n1 ON  n1.box_cid=s1.box_cid
LEFT JOIN 
    cryovial_store s2 ON s1.cryovial_id=s2.cryovial_id AND
    s2.status=0 /* ALLOCATED */
LEFT JOIN
    box_name n2 ON n2.box_cid = s2.box_cid
WHERE
    c.cryovial_id = cs.cryovial_id AND
    b.box_cid = cs.box_cid AND
    b.box_cid = bs.box_cid AND
    bs.status = 6 AND    /* SLOT_CONFIRMED */
    t.object_cid = aliquot_type_cid AND
    bs.rack_cid = r.rack_cid AND
    r.tank_cid = m.tank_cid AND
    s.object_cid = location_cid AND
    v.object_cid = storage_cid AND
    cs.retrieval_cid = -1015 /* :jobID */
AND
    s1.retrieval_cid = jobID

 
   
/* show destination boxes if defined - add this to previous query */

from
    cryovial_store s1
left join
    cryovial c on c.cryovial_id = s1.cryovial_id
left join
    box_name n1 on n1.box_cid = s1.box_cid
left join
    cryovial_store s2 on s1.cryovial_id = s2.cryovial_id and s2.status = 0
left join
    box_name n2 on n2.box_cid = s2.box_cid
where
    s1.retrieval_cid = :jobID
    
/* combined query - show destination boxes    
   
/* show some of a box retrieval plan */

SELECT br.box_id FROM c_box_retrieval br WHERE br.retrieval_cid = :rtid AND br.section = :sect AND status != 99

-- no 'chunks' yet, we haven't created them!
-- they will exist in c_box_retrieval, but don't already exist in cryovial_store where the job comes from
SELECT * FROM c_retrieval_job rj, cryovial_store cs WHERE rj.retrieval_cid = cs.retrieval_cid ORDER BY cs.box_cid

select
  rj.retrieval_cid,
  rj.external_name,
  rj.description,
  rj.primary_aliquot,
  rj.secondary_aliquot,
  br.section,
  br.box_id,
  br.rj_box_cid,
  cr.position,
  cr.cryovial_barcode,
  rj.finish_date
from
  c_retrieval_job rj, c_box_retrieval br
left join
  l_cryovial_retrieval cr on cr.rj_box_cid = br.rj_box_cid
  and cr.position < 10 /* 'some' */
where
  rj.retrieval_cid = br.retrieval_cid
order by
  rj_box_cid, position
  
  
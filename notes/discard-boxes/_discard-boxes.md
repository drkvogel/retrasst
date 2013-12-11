select * from c_project

SELECT 
  DISTINCT d.specimen_field_name, d.descriptor_id
FROM 
  descrip d
JOIN 
  profile_map pm
ON 
  d.descriptor_id = pm.group_id
WHERE
  pm.group_id <> 0 AND 
  d.specimen_field_name <> ''
  
select * from profile_map where group_id =937426


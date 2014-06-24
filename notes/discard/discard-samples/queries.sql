SELECT DISTINCT D.specimen_field_name
FROM DESCRIP D
JOIN PROFILE_MAP PM
ON 1=1
AND D.descriptor_id = PM.group_id
WHERE 1=1
AND PM.group_id <> 0
AND D.specimen_field_name <> ''
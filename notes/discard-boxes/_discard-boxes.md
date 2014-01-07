select * from c_project

select * from profile_map where group_id =937426


Keith's program throws because, with t_ldb20, in a function 'calcPersonFname( )' more than one row is returned from this:

SELECT
   DISTINCT d.specimen_field_name
FROM
   descrip d
JOIN
   profile_map pm
ON
   d.descriptor_id = pm.group_id
WHERE
   pm.group_id <> 0 AND
   d.specimen_field_name <> ''

Perhaps I should ask Keith but this seems to be a database scenario his program was not expecting. calcPersonFname( ) seems to be working out the specimen field name for a person, if it exists at all - is the query perhaps wrong or would you normally expect to see just one match where group_id <> 0 and that would be the person id? I don't really understand what profile_map.group_id is for (having read the docs), I must admit.

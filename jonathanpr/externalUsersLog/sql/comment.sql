/*
COMMENT ON TABLE centre IS
	'Centre information imported from Approved Centres and Researchers';
\p\g
COMMENT ON COLUMN centre.centre_id IS
	'From: Pt number';
\p\g
COMMENT ON COLUMN centre.name IS
	'From: Centre Name';
\p\g
COMMENT ON COLUMN centre.nhstrust IS
	'From: NHS Trust';
\p\g
COMMENT ON COLUMN centre.assocuniv IS
	'From: Associated University';
\p\g
COMMENT ON COLUMN centre.pinvestigator_id IS
	'From: Principal Investigtor, index into Staff table';
\p\g
COMMENT ON COLUMN centre.status IS
	'status 0 is valid, all other values means this entry should be ignored';
\p\g

COMMENT ON TABLE centre_staff IS
	'Shows which staff work at which centre';
\p\g
COMMENT ON COLUMN centre_staff.centre_id IS
	'centre index';
\p\g
COMMENT ON COLUMN centre_staff.staff_id IS
	'staff index';
\p\g
COMMENT ON COLUMN centre_staff.status IS
	'status 0 is valid, all other values means this entry should be ignored';
\p\g

COMMENT ON TABLE staff IS
	'Staff information imported from Approved Centres and Researchers';
\p\g
COMMENT ON COLUMN staff.staff_id IS
	'staff index';
\p\g
COMMENT ON COLUMN staff.status IS
	'status 0 is valid, all other values means this entry should be ignored';
\p\g
COMMENT ON COLUMN staff.pin IS
	'The PIN number to signin to the web site, if the user is a Principal Investigator';
\p\g
COMMENT ON COLUMN staff.role IS
	'Holds the various roles for the staff: PI - Principal Investigator, RW - Research Worker';
\p\g

				
COMMENT ON TABLE person IS
	'Registered persons on the Attila randomisation';
\p\g
COMMENT ON COLUMN person.person_id IS
	'person index, combines with centre_id to generate the ATTILA Study No:[CCPPP]';
\p\g
COMMENT ON COLUMN person.centre_id IS
	'centre index, combines with persion_id to generate the ATTILA Study No:[CCPPP]';
\p\g
COMMENT ON COLUMN person.staffpi_id IS
	'staff index, the Principal Investigator for this person';
\p\g
COMMENT ON COLUMN person.gender IS
	'gender:0 - Female, 1 - Male';
\p\g
COMMENT ON COLUMN person.livewith IS
	'Who the person is living with';
\p\g
COMMENT ON COLUMN person.caregiversupport IS
	'Caregiver Support:0 - Live-in caregiver; 1 - Caregiver visits at least once/day; 2 - Caregiver visits less than once/day';
\p\g
COMMENT ON COLUMN person.riskwandering IS
	'Risk of wandering or leaving the home inappropriately:0 - Low; 1 - Moderate; 2 - High';
\p\g
COMMENT ON COLUMN person.riskhome IS
	'Safety risk within home:0 - Low; 1 - Moderate; 2 - High';
\p\g				
COMMENT ON COLUMN person.informedconsent IS
	'Informed consent: 0 - From patient; 1 - From Carer; 2 - From Consultee; 3 - None';
\p\g				
COMMENT ON COLUMN person.operatorname IS
	'The name of the person randomising';
\p\g				
COMMENT ON COLUMN person.operatortelnum IS
	'The phone number of the person randomising';
\p\g			
COMMENT ON COLUMN person.name IS
	'The name of the person on the trial';
\p\g			
COMMENT ON COLUMN person.name_enc IS
	'The encrypted name of the person on the trial';
\p\g			
COMMENT ON COLUMN person.nhsnumber IS
	'The NHS number of the person on the trial; -1 for a NULL value';
\p\g			
COMMENT ON COLUMN person.nhsnumber_enc IS
	'The encrypted NHS number of the person on the trial';
\p\g			
COMMENT ON COLUMN person.dob IS
	'The date of birth the person on the trial: DD/MM/YYYY';
\p\g	 	
COMMENT ON COLUMN person.agegroup IS
	'The age group this person was put in: 0:<65, 1:65-80, 2:81+)';
\p\g	 	
COMMENT ON COLUMN person.hospitalno IS
	'The persons hospital number; -1 for a null value';
\p\g	 	
COMMENT ON COLUMN person.gp IS
	'The persions gp';
\p\g	 	
COMMENT ON COLUMN person.carersname IS
	'The carer name';
\p\g	
COMMENT ON COLUMN person.carersname_enc IS
	'The encrypted carer name';
\p\g 	
COMMENT ON COLUMN person.carerdob IS
	'The carer date of birth; 1/1/1900 for a null value';
\p\g	 	
COMMENT ON COLUMN person.rssprofessional IS
	'The Responsible Social Services Professional';
\p\g			
COMMENT ON COLUMN person.rnddate IS
	'The date of the randomisation';
\p\g							
COMMENT ON COLUMN person.rndalloc IS
	'The randomisation result: 0 - Control; 1 - Intervention';
\p\g							
COMMENT ON COLUMN person.diagnosis IS
	'Diagnosis of dementia: 0 - No; 1 - Yes';
\p\g							
COMMENT ON COLUMN person.tinsert IS
	'Timestamp when this row was added';
\p\g					
COMMENT ON COLUMN person.rec_status IS
	'The status of this record: 0 valid; <> 0 invalid';
\p\g					
COMMENT ON COLUMN person.ctsu_signon IS
	'If the sign on was done by a member of CTSU staff, the username is filled in here';
\p\g					

*/


				
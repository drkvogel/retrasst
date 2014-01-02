
ALTER TABLE centre
        ADD CONSTRAINT pk_centrenum
        PRIMARY KEY(centre_id);
\p\g

ALTER TABLE person
        ADD CONSTRAINT pk_id
        PRIMARY KEY(person_id);
\p\g

ALTER TABLE staff
        ADD CONSTRAINT pk_staffnum
        PRIMARY KEY(staff_id);
\p\g



/*=================================== encoding_id =========*/

ALTER TABLE centre
        ADD CONSTRAINT fk_pinvestigatorid
        FOREIGN KEY(pinvestigator_id) REFERENCES staff(staff_id);
\p\g

ALTER TABLE centre_staff
        ADD CONSTRAINT fk_centre_staff
        FOREIGN KEY(centre_id) REFERENCES centre(centre_id);
\p\g
ALTER TABLE centre_staff
        ADD CONSTRAINT fk_researchstaffid
        FOREIGN KEY(staff_id) REFERENCES staff(staff_id);
\p\g
		
ALTER TABLE person
        ADD CONSTRAINT fk_centrenum
        FOREIGN KEY(centre_id) REFERENCES centre(centre_id);				
\p\g




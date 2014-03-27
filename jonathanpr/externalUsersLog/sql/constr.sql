/************* PRIMARY KEYS ******************/

ALTER TABLE application
        ADD CONSTRAINT pk_application_id
        PRIMARY KEY(id);
\p\g

ALTER TABLE attribs
        ADD CONSTRAINT pk_attribs_id
        PRIMARY KEY(id);
\p\g

ALTER TABLE ctsu_user
        ADD CONSTRAINT pk_ctus_user_id
        PRIMARY KEY(name);
\p\g

ALTER TABLE user
        ADD CONSTRAINT pk_user_id
        PRIMARY KEY(id);
\p\g

/************* UNIQUE ******************/
ALTER TABLE attribs
        ADD CONSTRAINT unique_text
        UNIQUE("text");
\p\g

/************* FOREIGN KEYS ******************/

ALTER TABLE correspondence
		ADD FOREIGN KEY (application_id) 
		REFERENCES application(id); 
\p\g

ALTER TABLE user
		ADD FOREIGN KEY (application_id) 
		REFERENCES application(id); 
\p\g

ALTER TABLE user_attribs
		ADD FOREIGN KEY (user_id) 
		REFERENCES user(id); 
\p\g

ALTER TABLE user_attribs
		ADD FOREIGN KEY (attribs_id) 
		REFERENCES attribs(id); 
\p\g
 
 



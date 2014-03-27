
ALTER TABLE dicombiobank
        ADD CONSTRAINT pk_dicombiobank
        PRIMARY KEY(seriespk);
\p\g

ALTER TABLE logs
        ADD CONSTRAINT pk_logs
        PRIMARY KEY(id);
\p\g

ALTER TABLE cronstatus
        ADD CONSTRAINT pk_crons
        PRIMARY KEY(name);
\p\g

ALTER TABLE patientquiz
        ADD CONSTRAINT pk_patientquiz
        PRIMARY KEY(participant,firstname,scandate);
\p\g

ALTER TABLE keyword
		ADD CONSTRAINT pk_keyword
		PRIMARY KEY(keywordpk);
\p\g


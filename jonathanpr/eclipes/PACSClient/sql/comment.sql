COMMENT ON TABLE dicombiobank IS
	'When a series is downloaded from the syngo.share, the details are stored in this table.';
\p\g
COMMENT ON COLUMN dicombiobank.patientid IS
	'The DICOM participant id, maybe miss typed';
\p\g
COMMENT ON COLUMN dicombiobank.studyid IS
	'Dicom study id. Retrieved from the Dicom file(s)';
\p\g
COMMENT ON COLUMN dicombiobank.studyid_enc IS
	'Encrypted dicombiobank.studyid';
\p\g
COMMENT ON COLUMN dicombiobank.seriesid IS
	'Dicom series id. Retrieved from the Dicom file(s)';
\p\g
COMMENT ON COLUMN dicombiobank.seriesid_enc IS
	'Encrypted dicombiobank.seriesid';
\p\g
COMMENT ON COLUMN dicombiobank.studydate IS
	'The date the study was inserted into the PACS';
\p\g
COMMENT ON COLUMN dicombiobank.statusenc IS
	'1 if we have not decrypted the encrypted fields, 0 if decrypted. Used by the decryption cron job';
\p\g
COMMENT ON COLUMN dicombiobank.zipmd5 IS
	'MD5 of the zipped DICOM study';
\p\g
COMMENT ON COLUMN dicombiobank.studypk IS
	'The syngo.share primary key of the study';
\p\g
COMMENT ON COLUMN dicombiobank.seriespk IS
	'The syngo.share primary key of the series';
\p\g
COMMENT ON COLUMN dicombiobank.orginalsize IS
	'The pre-zip size of the DICOM series (bytes)';
\p\g
COMMENT ON COLUMN dicombiobank.compressedsize IS
	'The pre-zip size of the DICOM series (bytes)';
\p\g
COMMENT ON COLUMN dicombiobank.importstatus IS
	'Set to 1 once the BioBank import table has been filled';
\p\g
COMMENT ON COLUMN dicombiobank.status IS
	'1 if OK, anything else invalidates this row';
\p\g
COMMENT ON COLUMN dicombiobank.numImages IS
	'The number of images in this Series';
\p\g
COMMENT ON COLUMN dicombiobank.modality IS
	'The modality(s) in this series, see the DICOM standard for meaning';
\p\g
COMMENT ON COLUMN dicombiobank.aet IS
	'The name of the device which geneated this series';
\p\g
COMMENT ON COLUMN dicombiobank.aesvi IS
	'The VI of the AES for this row';
\p\g
COMMENT ON COLUMN dicombiobank.filename IS
	'Series Zip file name. Format: Modality_trid_cid_SeriesPK_StudyPK.zip';
\p\g
COMMENT ON COLUMN dicombiobank.seriesdescription IS
	'The description of the series, taken from the DICOM';
\p\g
COMMENT ON COLUMN dicombiobank.hostname IS
	'sender hostname';
\p\g


COMMENT ON TABLE logs IS
	'Cron errors and general information is stored in this table, which can be viewed using the website';
\p\g
COMMENT ON COLUMN logs.id IS
	'Primary Key';
\p\g
COMMENT ON COLUMN logs.inserttimestamp IS
	'Timestamp';
\p\g
COMMENT ON COLUMN logs.loglevel IS
	'0=DEBUG,1=INFO,2=ERROR,3=CRITIAL,4=ACTIONREQUIRED';
\p\g
COMMENT ON COLUMN logs.logtype IS
	'0=GENERAL,1=DATABASE,2=FILESYSTEM,3=PACS';
\p\g
COMMENT ON COLUMN logs.patientid IS
	'The Dicom patientid, if applicable';
\p\g
COMMENT ON COLUMN logs.studydate IS
	'The study date, if applicable';
\p\g
COMMENT ON COLUMN logs.status      IS
	'1 if OK, anything else invalidates this row';
\p\g
COMMENT ON COLUMN logs.message IS
	'The log message';
\p\g

COMMENT ON TABLE dicomdownloader IS
	'Keeps track of the last time the PACS was scanned for new DICOM entries';
\p\g
COMMENT ON COLUMN dicomdownloader.lastdownload IS
	'Timestamp';
\p\g

COMMENT ON TABLE biobankparticipant IS
	'As BioBank questions are submitted into ICE, a CRON job extracts some of the data and places in this table';
\p\g
COMMENT ON COLUMN biobankparticipant.pid IS
	'The pid of the Patient';
\p\g
COMMENT ON COLUMN biobankparticipant.dicomparticipantid IS
	'DICOM Participent id for matching';
\p\g
COMMENT ON COLUMN biobankparticipant.firstnameid IS
	'DICOM FirstName id for matching';
\p\g
COMMENT ON COLUMN biobankparticipant.cid IS
	'From the ICE row';
\p\g
COMMENT ON COLUMN biobankparticipant.trid IS
	'From the ICE row';
\p\g
COMMENT ON COLUMN biobankparticipant.scandate IS
	'The date it was entered into ICE';
\p\g

COMMENT ON TABLE keyword IS
	'Where details of the series level keywords attached to DICOM is stored';
\p\g
COMMENT ON COLUMN keyword.keywordpk IS
	'Primary Key of the syngo.share keyword';
\p\g
COMMENT ON COLUMN keyword.keyword IS
	'The text of the keyword';
\p\g
COMMENT ON COLUMN keyword.type IS
	'Type of this keyword, taken from syngo.share';
\p\g
COMMENT ON COLUMN keyword.status  IS
	'1 if OK, anything else invalidates this row';
\p\g

COMMENT ON TABLE dicombiobank_keyword IS
	'A table to link together which keywords belong to which series';
\p\g
COMMENT ON COLUMN dicombiobank_keyword.seriespk IS
	'FK into DICOMBIOBANK';
\p\g
COMMENT ON COLUMN dicombiobank_keyword.keywordpk IS
	'FK into KEYWORD';
\p\g

COMMENT ON TABLE cronstatus IS
	'A table to keep track of when each CRON job starts and completes. It is displayed in the web site overview';
\p\g
COMMENT ON COLUMN cronstatus.name IS
	'Name of the CRON';
\p\g
COMMENT ON COLUMN cronstatus.started IS
	'Timestamp of last time it started';
\p\g
COMMENT ON COLUMN cronstatus.finished IS
	'Timestamp of the last time it completed';
\p\g

COMMENT ON TABLE pacsstudy IS
	'Every new study found on the PACS in the predefined AUs, get stored in this table';
\p\g
COMMENT ON COLUMN pacsstudy.patientid IS
	'The patientid found in the DICOM file';
\p\g
COMMENT ON COLUMN pacsstudy.firstname IS
	'The firstname found in the DICOM file';
\p\g
COMMENT ON COLUMN pacsstudy.confirmedpatientid IS
	'An entry here means we have an automatic match on either patientid or firstname into the ICE system or a manually one was entered by hand';
\p\g
COMMENT ON COLUMN pacsstudy.studyuid IS
	'The DICOM study id';
\p\g
COMMENT ON COLUMN pacsstudy.studyuid_enc IS
	'Encrypted version of studyid';
\p\g
COMMENT ON COLUMN pacsstudy.studydate IS
	'The date the study was inserted into the PACS';
\p\g
COMMENT ON COLUMN pacsstudy.aet IS
	'The name of the device which send study to the PACS';
\p\g
COMMENT ON COLUMN pacsstudy.studypk IS
	'The syngo.share study primary key';
\p\g
COMMENT ON COLUMN pacsstudy.modalities IS
	'The modalities syngo.share reports which are in this study';
\p\g
COMMENT ON COLUMN pacsstudy.au IS
	'The authority unit this study is situated';
\p\g
COMMENT ON COLUMN pacsstudy.insert IS
	'Timestamp this was inserted';
\p\g
COMMENT ON COLUMN pacsstudy.update IS
	'Timestamp this was last updated';
\p\g
COMMENT ON COLUMN pacsstudy.status IS
	'Invalid =0, OK=1, Complete=2, Ignore = 3';
\p\g
COMMENT ON COLUMN pacsstudy.patientpk IS
	'The syngo.share patient primary key';
\p\g
COMMENT ON COLUMN pacsstudy.statusenc IS
	'1 if we have not decrypted the encrypted fields, 0 if decrypted. Used by the decryption cron job';
\p\g
COMMENT ON COLUMN pacsstudy.aesvi IS
	'The VI of the AES for this row';
\p\g
COMMENT ON COLUMN pacsstudy.lastalert IS
	'timestamp of when the last email alert was sent';
\p\g

COMMENT ON TABLE staff IS
	'Table to hold the user’s sign on usernames who can access the web site';
\p\g
COMMENT ON COLUMN staff.name IS
	'Sign in name allowed to use the web site';
\p\g
COMMENT ON COLUMN staff.status IS
	'1 if OK, anything else, ignores this user';
\p\g





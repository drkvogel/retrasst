CREATE SEQUENCE log_seq START WITH 1;
\p\g

alter sequence log_seq RESTART WITH 1;
\p\g

CREATE TABLE dicombiobank
(
	patientid				VARCHAR(8)	NOT NULL WITH DEFAULT '',
	studyid					VARCHAR(64) NOT NULL WITH DEFAULT '',
	studyid_enc				VARCHAR(256) NOT NULL WITH DEFAULT '',
	seriesid				VARCHAR(64)	NOT NULL WITH DEFAULT '',
	seriesid_enc			VARCHAR(256)	NOT NULL WITH DEFAULT '',
	studydate				DATE NOT NULL WITH DEFAULT '',
	statusenc				INTEGER NOT NULL WITH DEFAULT 1,
	zipmd5					VARCHAR(32)	NOT NULL,
	filename				VARCHAR(32)	NOT NULL,
	studypk					BIGINT NOT NULL,
	seriespk				BIGINT NOT NULL,
	orginalsize				BIGINT NOT NULL,
	compressedsize			BIGINT NOT NULL,
	importstatus			INTEGER	NOT NULL WITH DEFAULT 0,
	status					INTEGER	NOT NULL WITH DEFAULT 0,
	numImages				INTEGER NOT NULL,
	modality				VARCHAR(10) NOT NULL,	
	aet						VARCHAR(20)	NOT NULL WITH DEFAULT '',
	hostname				VARCHAR(20)	NOT NULL WITH DEFAULT '',
	seriesdescription		VARCHAR(60) NOT NULL,
	aesvi					VARCHAR(32)	NOT NULL WITH DEFAULT ''
)	
\p\g

CREATE TABLE logs
(
	id					INTEGER NOT NULL WITH DEFAULT log_seq.nextval,
	inserttimestamp				DATE NOT NULL WITH DEFAULT 'now',
	loglevel				INTEGER NOT NULL,
	logtype					INTEGER	NOT NULL,
	patientid				VARCHAR(20)	NOT NULL WITH DEFAULT '',
	studydate				DATE NOT NULL WITH DEFAULT '',
	status     				INTEGER NOT NULL WITH DEFAULT 1,
	message					VARCHAR(256)	NOT NULL
)
\p\g

CREATE TABLE dicomdownloader
(
	lastdownload			DATE
)
\p\g

CREATE TABLE staff
(
	status     				INTEGER NOT NULL WITH DEFAULT 1,
	name			VARCHAR(20)	NOT NULL
)
\p\g

CREATE TABLE biobankparticipant 
(
	pid						INTEGER	NOT NULL,
	dicomparticipantid		VARCHAR(8)	NOT NULL,
	firstnameid				VARCHAR(8)	NOT NULL,
	cid						INTEGER NOT NULL,
	trid					INTEGER NOT NULL,
	scandate				DATE NOT NULL
)
\p\g

CREATE TABLE keyword
(
	keywordpk				BIGINT NOT NULL,
	keyword					VARCHAR(200) NOT NULL,
	type					VARCHAR(50) NOT NULL,
	status 					INT NOT NULL WITH DEFAULT 1
)
\p\g

CREATE TABLE dicombiobank_keyword
(
	seriespk				BIGINT NOT NULL,
	keywordpk				BIGINT NOT NULL
)
\p\g

CREATE TABLE cronstatus
(
	name					VARCHAR(10)	NOT NULL,
	started					DATE,
	finished				DATE
)
\p\g

CREATE TABLE pacsstudy
(
	patientid				VARCHAR(11)	NOT NULL WITH DEFAULT '',
	name					VARCHAR(11)	NOT NULL WITH DEFAULT '',
	confirmedpatientid		VARCHAR(8)	NOT NULL WITH DEFAULT '',
	confirmedname			VARCHAR(8)	NOT NULL WITH DEFAULT '',
	studyuid				VARCHAR(60)	NOT NULL WITH DEFAULT '',
	studyuid_enc			VARCHAR(250)	NOT NULL WITH DEFAULT '',
	studydate				DATE,
	studyinsertdate				DATE,
	aet						VARCHAR(20)	NOT NULL WITH DEFAULT '',
	studypk					BIGINT NOT NULL,
	modalities				VARCHAR(20)	NOT NULL WITH DEFAULT '',
	au						VARCHAR(20)	NOT NULL WITH DEFAULT '',
	insert					DATE,
	update					DATE,
	lastalert				DATE,
	status					INT WITH DEFAULT 1,
	patientpk				BIGINT,
	statusenc				INT WITH DEFAULT 1,
	aesvi					VARCHAR(32)	NOT NULL WITH DEFAULT ''
)
\p\g



CREATE SEQUENCE patient_seq START WITH 9;
\p\g

alter sequence  patient_seq restart with 9;
\p\g

CREATE TABLE centre
(
	centre_id			INTEGER	NOT NULL,
	name				VARCHAR(60) NOT NULL,
	nhstrust			VARCHAR(60) NOT NULL,
	assocuniv			VARCHAR(60) NOT NULL,
	pinvestigator_id	INTEGER 	NOT NULL,
	status				INTEGER		NOT NULL
)		
\p\g

CREATE TABLE centre_staff
(
	centre_id			INTEGER	NOT NULL,
	staff_id			INTEGER 	NOT NULL,
	status				INTEGER		NOT NULL
)		
\p\g 

CREATE TABLE staff
(
	staff_id			INTEGER	NOT NULL,
	name				VARCHAR(60) NOT NULL,
	email				VARCHAR(60),
	phone				VARCHAR(40),
	address1			VARCHAR(60),
	address2			VARCHAR(60),
	address3			VARCHAR(60),
	address4			VARCHAR(60),
	address5			VARCHAR(60),
	address6			VARCHAR(60),
	address7			VARCHAR(60),
	address8			VARCHAR(60),
	status				INTEGER		NOT NULL,
	pin					INTEGER,
	role				VARCHAR(6)
)	
\p\g

CREATE TABLE person
(
	person_id			INTEGER	NOT NULL WITH DEFAULT patient_seq.nextval,
	centre_id			INTEGER NOT NULL,
	staffpi_id			INTEGER NOT NULL,
	gender				INTEGER	NOT NULL,
	livewith			VARCHAR(60) NOT NULL,
	caregiversupport	INTEGER NOT NULL,
	riskwandering		INTEGER NOT NULL,
	riskhome			INTEGER NOT NULL,
	informedconsent 	INTEGER	NOT NULL,
	operatorname 		VARCHAR(60) NOT NULL,
	operatortelnum		VARCHAR(40) NOT NULL,
	
	name				VARCHAR(60) NOT NULL WITH DEFAULT '',
	name_enc			VARCHAR(256) NOT NULL WITH DEFAULT '',
	nhsnumber			VARCHAR(12) NOT NULL WITH DEFAULT '',
	nhsnumber_enc		VARCHAR(256) NOT NULL WITH DEFAULT '',
	dob					DATE NOT NULL,
	agegroup			INTEGER NOT NULL,
	hospitalno			VARCHAR(40) NOT NULL WITH DEFAULT '',
	gp					VARCHAR(60) NOT NULL WITH DEFAULT '',
	carersname			VARCHAR(60) NOT NULL WITH DEFAULT '',
	carersname_enc		VARCHAR(256) NOT NULL WITH DEFAULT '',
	carerdob			DATE NOT NULL WITH DEFAULT '1-JAN-1900',
	rssprofessional		VARCHAR(60) NOT NULL,
	rnddate				DATE NOT NULL,
	rndalloc			INTEGER NOT NULL WITH DEFAULT -1,
	diagnosis			INTEGER NOT NULL WITH DEFAULT -1,
	
	tinsert				DATE NOT NULL WITH DEFAULT 'now',
	rec_status			INTEGER	NOT NULL WITH DEFAULT 0,
	
	ctsu_signon			VARCHAR(20) NOT NULL WITH DEFAULT '',
	status_enc			INTEGER NOT NULL WITH DEFAULT 1
)

\p\g

CREATE SEQUENCE application_id_seq START WITH 1;
\p\g

CREATE SEQUENCE user_id_seq START WITH 1;
\p\g

CREATE TABLE application
(
	id					BIGINT NOT NULL,
	applicationid 		VARCHAR(30) NOT NULL WITH DEFAULT ' ',
	pi					VARCHAR(255),
	info				VARCHAR(5012),
	created				DATE NOT NULL WITH DEFAULT 'now',
	lastupdated			DATE NOT NULL WITH DEFAULT 'now',
	status				INT NOT NULL WITH DEFAULT 0, 
	piemail				VARCHAR(30),
	summary				VARCHAR(60),
	useraccountpattern	VARCHAR(10),
	currentusernum		INT NOT NULL WITH DEFAULT 0
)
\p\g

CREATE TABLE attribs
(
	id					BIGINT NOT NULL,
	text				VARCHAR(30) NOT NULL
)
\p\g

CREATE TABLE correspondence
(
	rosetta				LONG VARCHAR NOT NULL,
	application_id		BIGINT  NOT NULL WITH DEFAULT 0
)
\p\g

CREATE TABLE ctsu_user
(
	name				VARCHAR(60) NOT NULL,
	status				INT NOT NULL WITH DEFAULT 1
)
\p\g

CREATE TABLE user
(
	name				VARCHAR(25) NOT NULL,
	type				INTEGER1 NOT NULL,
	id					BIGINT NOT NULL,
	application_id		BIGINT NOT NULL,
	status				INT NOT NULL WITH DEFAULT 0 
)
\p\g

CREATE TABLE user_attribs
(
	user_id				BIGINT NOT NULL,
	attribs_id			BIGINT NOT NULL
)
\p\g



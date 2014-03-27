CREATE SEQUENCE application_id_seq AS INTEGER4 START WITH 1;
\p\g

CREATE SEQUENCE user_id_seq AS INTEGER4 START WITH 1;
\p\g

CREATE TABLE ru_application
(
	id			INTEGER NOT NULL,
	app_id 			INTEGER NOT NULL,
	pi			VARCHAR(255) NOT NULL,
	info			VARCHAR(5012) NOT NULL WITH DEFAULT '',
	status			INT NOT NULL WITH DEFAULT 0, 
	piemail			VARCHAR(30) NOT NULL,
	summary			VARCHAR(60) NOT NULL WITH DEFAULT '',
	useraccountpattern	VARCHAR(10) NOT NULL WITH DEFAULT '',
	currentusernum		INT NOT NULL WITH DEFAULT 0,
	tinsert			DATE NOT NULL WITH DEFAULT 'now',
	tupdate			DATE NOT NULL WITH DEFAULT 'now'
);
\p\g

CREATE TABLE attribs
(
	id			INTEGER NOT NULL,
	text			VARCHAR(30) NOT NULL
);
\p\g

CREATE TABLE correspondence
(
	app_id			INTEGER  NOT NULL WITH DEFAULT 0,
	rosetta			LONG VARCHAR NOT NULL,
	tinsert			DATE NOT NULL WITH DEFAULT 'now'

);
\p\g


CREATE TABLE ru_user
(
	name			VARCHAR(25) NOT NULL,
	type			INTEGER1 NOT NULL,
	id			INTEGER NOT NULL,
	app_id			INTEGER NOT NULL,
	status			INT NOT NULL WITH DEFAULT 0,
	tinsert			DATE NOT NULL WITH DEFAULT 'now'
);
\p\g

CREATE TABLE user_attribs
(
	ru_user_id		INTEGER NOT NULL,
	attribs_id		INTEGER NOT NULL
);
\p\g



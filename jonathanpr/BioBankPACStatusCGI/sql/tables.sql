
CREATE TABLE staff
(
	name				VARCHAR(30) NOT NULL,
	password			VARCHAR(20) NOT NULL,
	pacsuser			VARCHAR(20) NOT NULL,
	pacspassword		VARCHAR(20) NOT NULL,
	status				INTEGER	NOT NULL WITH DEFAULT 1
)		
\p\g


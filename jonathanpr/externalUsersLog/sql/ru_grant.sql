/* sql ace_trex_live < ru_grant.sql */

REVOKE ALL	ON SEQUENCE application_id_seq FROM extract RESTRICT;
\p\g
GRANT NEXT	ON SEQUENCE application_id_seq TO extract;

\p\g

REVOKE ALL	ON SEQUENCE user_id_seq FROM extract RESTRICT;
\p\g
GRANT NEXT	ON SEQUENCE user_id_seq TO extract;

\p\g

DROP PERMIT		ON ru_application ALL;
GRANT SELECT, INSERT	ON ru_application TO extract;
GRANT UPDATE(tupdate)	ON ru_application TO extract;

\p\g

DROP PERMIT		ON attribs ALL;
GRANT SELECT		ON attribs TO extract;

\p\g

DROP PERMIT		ON correspondence ALL;
GRANT SELECT, INSERT	ON correspondence TO extract;

\p\g

DROP PERMIT		ON ru_user ALL;
GRANT SELECT, INSERT	ON ru_user TO extract;
GRANT UPDATE(status)	ON ru_user TO extract;

\p\g

DROP PERMIT		ON user_attribs ALL;
GRANT SELECT, INSERT	ON user_attribs TO extract;

\p\g



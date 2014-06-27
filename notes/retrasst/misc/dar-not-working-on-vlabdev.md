The check status thing is working now, the vnode on arrakis for vlabdev was indeed missing.

I can’t see anything unusual happening on vlabdev though. There’s currently a cbird session in t_ldbc which lists the last statement run as “execute e0”, if that’s of any help.

The audit logs are broken again* and only go up to 05:16 this morning, so I can’t see from those anything which might indicate an earlier problem.

Geraint

*Fairly sure this is a known bug fixed in a later release of Ingres, but vlabdev is deliberately kept on the same Ingres release as vlab.

From: Chris Bird 
Sent: 27 June 2014 12:13
To: Ingres
Subject: DAR status check not working?

Ran my unmodified, it-worked-yesterday program (Storeman.exe) against vlabdev t_ldbc and t_ldb27, and it froze while attempting to save records to one or both of these databases, with no error message, exception or useful stack trace.

Went to DAR to check status of vlabdev and got this:

Connection status check on vlabdev
Generated: 27/06/2014 12:04:04
Unexpected error in DBI connect() to vlabdev::imadb as Reason: E_LQ0001 Failed to connect to DBMS session. E_LC0001 GCA protocol service (GCA_REQUEST) failure. Internal service status E_GC0132 -- User provided a vnode as part of the database name (vnode::dbname), but connection information for that vnode is missing. Enter connection information for the vnode using NETUTIL.. ERROR: exit status 3
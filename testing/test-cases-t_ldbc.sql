/* Here's the SQL for the mirror system.  You'll need to modify the project CID for the test system: */

/* -149662 = t_ldb20 dev_hps2-thrive */

\sql
set autocommit on
\nocontinue
\p\g

insert into c_retrieval_job 
values (-636363, 'Example box retrieval','A box retrieval for Chris', 0,-149662,1,2,'',-31781,0,'10/07/13','', 0);
\p\g

insert into c_retrieval_job 
values (-363636, 'Example cryovial retrieval','A cryovial retrieval for Chris',0,-149662,1,4,'',-31781,0,'10/07/13','',-31782);
\p\g
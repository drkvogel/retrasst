/* Here's the SQL for the mirror system.  You'll need to modify the project CID for the test system: */

/* -149662 = t_ldb20 dev_hps2-thrive */

\sql
set autocommit on
\nocontinue
\p\g

-- update c_retrieval_job set status = 0 where retrieval_cid = -636363;
-- update c_retrieval_job set status = 0 where retrieval_cid = -363636;
-- \p\g
-- \q

-- 2.7.2

-- delete from c_retrieval_job where retrieval_cid = -636363
-- \p\g

insert into c_retrieval_job 
	(retrieval_cid, external_name, description, process_cid, project_cid, 
	status, job_type, claimed_until, reason, 
	primary_aliquot, exercise_cid, start_date, finish_date, 
	secondary_aliquot, box_set, reason_flags)
values (
	-636363, 'Example box retrieval','A box retrieval for Chris', 0, -149662, 
	0, 2, date('now') + date('1 year'), 'For testing',
	-31782, 0, date('now'), date('now') + date('1 year'), 
	-31781, 0, 0);
\p\g

-- enum Status { NEW_JOB, INPROGRESS, DONE, REJECTED, DELETED = 99 };
-- enum JobKind { UNKNOWN, BOX_MOVE, BOX_RETRIEVAL, BOX_DISCARD, SAMPLE_RETRIEVAL, SAMPLE_DISCARD, NUM_TYPES };

-- retrieval_cid,external_name,description,process_cid,project_cid,status,job_type[,claimed_until][,primary_aliquot][,exercise_cid][,start_date][,finish_date][,secondary_aliquot]
-- -363636,     'Example...'  'A cryo... ' 0           -149662     1      4         ''              -31781            0              10/07/13     ''           -31782);

-- 2.7.2

-- delete from c_retrieval_job where retrieval_cid = -363636
-- \p\g

insert into c_retrieval_job
	(retrieval_cid, external_name, description, process_cid, project_cid, 
	status, job_type, claimed_until, reason, 
	primary_aliquot, exercise_cid, start_date, finish_date, 
	secondary_aliquot, box_set, reason_flags) 
values (
	-363636, 'Example cryovial retrieval','A cryovial retrieval for Chris', 0, -149662,
	0, 4, date('now') + date('1 year'), 'For testing', -- INPROGRESS, SAMPLE_RETRIEVAL
	-31782, 0, date('now'), date('now') + date('1 year'), 
	-31781, 0, 0);
\p\g



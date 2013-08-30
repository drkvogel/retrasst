#pragma hdrstop

#include "DiscardDb.h"
#include "DiscardUtil.h"
#include "LCDbObject.h"
#include "LCDbAuditTrail.h"
#include "LPDbCryovial.h"
#include "LPDbCryovialStore.h"

#pragma package(smart_init)


namespace Discard
{

// Db

Db::Db( )
{
}

Db::Db( LIMSDatabase * cdb )
    : m_cdb(cdb)
{
}

void
Db::setCdb( LIMSDatabase * cdb )
{
    m_cdb = cdb;
    return;
}

LIMSDatabase *
Db::getCdb( ) const
{
    return m_cdb;
}

void
Db::setPdb( LIMSDatabase * pdb )
{
	const LCDbProject * pproj =
		LCDbProjects::records().findByName(m_projectName.c_str());
	if (pproj == 0)
	{
		std::string error = "";
		error += "failed to find project ";
		error += m_projectName;
		error += " at ";
		error += HERE;
		throw Exception(error.c_str());
	}

//	DbFamily().changeDbName(pdb, pproj->getDbName());
	m_pproj = pproj;
	m_pdb = pdb;
	return;
}

LIMSDatabase *
Db::getPdb( ) const
{
    return m_pdb;
}    

void
Db::setProjectName( const std::string & name )
{
    m_projectName = name;
    return;
}

const std::string &
Db::getProjectName( ) const
{
    return m_projectName;
}

const LCDbProject *
Db::getPproj( ) const
{
    return m_pproj;
}

std::string
Db::getStudyCode( ) const
{
    std::string code = "";
    if (m_pproj != 0) code = m_pproj->getStudyCode().c_str();
    return code;
}

std::string
Db::getCdbname( ) const
{
	return  m_cdb->getDbName() ;
}

std::string
Db::getPdbname( ) const
{
	return  m_pdb->getDbName() ;
}

void
Db::setCq( LQuery * cq )
{
	do
	{
		cq->setSQL("select dbmsinfo('database') s");
		if (! cq->open())
		{
			std::string error = "";
			error += "suspect query";
			error += " at ";
			error += HERE;
			throw Exception(error.c_str());
		}

		const std::string s = cq->readString("s").c_str();

		m_cq = cq;

	} while (false);

	return;
}

LQuery *
Db::getCq( ) const
{
	return m_cq;
}

void
Db::setPq( LQuery * pq )
{
	do
	{
		pq->setSQL("select dbmsinfo('database') s");
		if (! pq->open())
		{
			std::string error = "";
			error += "suspect query";
			error += " at ";
			error += HERE;
			throw Exception(error.c_str());
		}

		const std::string s = pq->readString("s").c_str();

		m_pq = pq;

	} while (false);

	return;
}

LQuery *
Db::getPq( ) const
{
	return m_pq;
}

std::string
Db::calcPersonFname( ) const
{
	std::string fname = "";

	do
	{
		std::string sql =
			"  SELECT DISTINCT D.specimen_field_name"
			"  FROM DESCRIP D"
			"  JOIN PROFILE_MAP PM"
			"  ON 1=1"
			"  AND D.descriptor_id = PM.group_id"
			"  WHERE 1=1"
			"  AND PM.group_id <> 0"
			"  AND D.specimen_field_name <> ''"
			;

		m_pq->setSQL(sql.c_str());

		for (m_pq->open(); ! m_pq->eof(); m_pq->next())
		{
			if (fname != "")
            {
                std::string error = "";
                error += "multiple person id fields";
                error += " at ";
                error += HERE;
                throw Exception(error.c_str());
			}
			const std::string specimen_field_name =
				m_pq->readString("specimen_field_name").c_str();
			fname = specimen_field_name;
		}

	} while (false);

    return fname;
}

void
Db::addJobnos( IntVec * jobnos, const int dbcrstatus ) const
{

	do
	{
		std::string sql =
			"  SELECT DISTINCT CS.retrieval_cid retrieval_cid"
			"  FROM SPECIMEN S"
			"  JOIN CRYOVIAL C"
			"  ON 1=1"
			"  AND C.sample_id = S.sample_id"
			"  LEFT JOIN CRYOVIAL_STORE CS"
			"  ON 1=1"
			"  AND CS.cryovial_id = C.cryovial_id"
			"  WHERE 1=1"
            ;
        if (dbcrstatus != -1) sql +=
			"  AND CS.status = :dbcrstatus"
            ;
        sql +=
			"  AND CS.retrieval_cid <> 0"
			"  AND S.status <> :deleted"
			"  ORDER BY CS.retrieval_cid DESC"
			;

		m_pq->setSQL(sql.c_str());

        if (dbcrstatus != -1)
            m_pq->setParam("dbcrstatus", dbcrstatus);
		m_pq->setParam("deleted", LPDbCryovial::DELETED);

		for (m_pq->open(); ! m_pq->eof(); m_pq->next())
		{
			const int jobno = m_pq->readInt("retrieval_cid");
            if (getJob(jobno) == 0) continue;
            jobnos->push_back(jobno);
		}

	} while (false);

    return;
}

const LCDbCryoJob *
Db::getJob( const int jobno ) const
{
	const LCDbCryoJob * job = 0;

    do
    {
		const LCDbCryoJob * myjob = LCDbCryoJobs::records().readRecord(*m_cq, jobno);
        if (myjob == 0) break;
        if (! myjob->isActive()) break;

        job = myjob;

    } while (false);

    return job;
}

const LCDbCryoJob *
Db::makeJob( const std::string & prefix,
	const std::string & description, const std::string & reason ) const
{
	LCDbCryoJob newjob(0, LCDbCryoJob::SAMPLE_DISCARD );	/// fixme: may be box disposal
    newjob.setProjectID(getPproj()->getID());
	newjob.setStatus(LCDbCryoJob::INPROGRESS);
	newjob.createName(*m_cq, prefix + " ");
	newjob.setDescription(description);
	newjob.setReason(reason.empty() ? description : reason);
	newjob.saveRecord(*m_cq);
    const int jobno = newjob.getID();

    return getJob(jobno);
}

bool
Db::isJob( const int jobno ) const
{
    return getJob(jobno) != 0;
}

const LCDbOperator *
Db::getUser( const int userid ) const
{
    return LCDbOperators::records().findByID(userid);
}

void
Db::addSamples( SampleVec * samples, const Cryovial & cryovial ) const
{
    const std::string pfname = Person::getFname();
    const std::string pfterm = (pfname == "")
        ? Util::quote("")
        : "S." + pfname;

    std::string sql =
        "  SELECT S.sample_id, S.barcode sample_barcode"
        "  , C.cryovial_id, C.cryovial_barcode, C.aliquot_type_cid"
        "  , CS.status cryovial_store_status, CS.retrieval_cid"
        "  , CS.note_exists note_exists, CS.record_id cryovial_store_id"
        ;
    sql +=
        "  , " + pfterm + " person_id";
    sql +=
        "  FROM SPECIMEN S"
        "  JOIN CRYOVIAL C"
        "  ON 1=1"
        "  AND C.sample_id = S.sample_id"
        "  LEFT JOIN CRYOVIAL_STORE CS"
        "  ON 1=1"
        "  AND CS.cryovial_id = C.cryovial_id"
        "  WHERE 1=1"
        "  AND C.cryovial_barcode = :barcode"
        "  AND S.status <> :deleted"
        "  ORDER BY S.barcode, C.cryovial_barcode, CS.record_id DESC"
        ;

    m_pq->setSQL(sql.c_str());
    m_pq->setParam("barcode", cryovial.getName().c_str());
    m_pq->setParam("deleted", LPDbCryovial::DELETED);

    SampleVec mysamples;
    addSamples(&mysamples);
    setNotesForSamples(&mysamples);
    samples->insert(samples->end(), mysamples.begin(), mysamples.end());

    return;
}

void
Db::addSamples( SampleVec * samples, const Tube & tube ) const
{
    const std::string pfname = Person::getFname();
    const std::string pfterm = (pfname == "")
        ? Util::quote("")
        : "S." + pfname;

    std::string sql =
        "  SELECT S.sample_id, S.barcode sample_barcode"
        "  , C.cryovial_id, C.cryovial_barcode, C.aliquot_type_cid"
        "  , CS.status cryovial_store_status, CS.retrieval_cid"
        "  , CS.note_exists note_exists, CS.record_id cryovial_store_id"
        ;
    sql +=
        "  , " + pfterm + " person_id";
    sql +=
        "  FROM SPECIMEN S"
        "  LEFT JOIN CRYOVIAL C"
        "  ON 1=1"
        "  AND C.sample_id = S.sample_id"
        "  LEFT JOIN CRYOVIAL_STORE CS"
        "  ON 1=1"
        "  AND CS.cryovial_id = C.cryovial_id"
        "  WHERE 1=1"
        "  AND S.barcode = :barcode"
        "  AND S.status <> :deleted"
        "  ORDER BY S.barcode, C.cryovial_barcode, CS.record_id DESC"
        ;

    m_pq->setSQL(sql.c_str());
    m_pq->setParam("barcode", tube.getName().c_str());
    m_pq->setParam("deleted", LPDbCryovial::DELETED);

    SampleVec mysamples;
    addSamples(&mysamples);
    setNotesForSamples(&mysamples);
    samples->insert(samples->end(), mysamples.begin(), mysamples.end());

    return;
}

void
Db::addSamples( SampleVec * samples, const Box & box ) const
{
    const std::string pfname = Person::getFname();
    const std::string pfterm = (pfname == "")
        ? Util::quote("")
        : "S." + pfname;

    std::string sql =
        "  SELECT S.sample_id, S.barcode sample_barcode"
        "  , C.cryovial_id, C.cryovial_barcode, C.aliquot_type_cid"
        "  , CS.status cryovial_store_status, CS.retrieval_cid"
        "  , CS.note_exists note_exists, CS.record_id cryovial_store_id"
        ;
    sql +=
        "  , " + pfterm + " person_id";
    sql +=
        "  FROM BOX_NAME BN"
        "  JOIN CRYOVIAL_STORE CS"
        "  ON 1=1"
        "  AND CS.box_cid = BN.box_cid"
        "  JOIN CRYOVIAL C"
        "  ON 1=1"
        "  AND C.cryovial_id = CS.cryovial_id"
        "  JOIN SPECIMEN S"
        "  ON 1=1"
        "  AND S.sample_id = C.sample_id"
        "  WHERE 1=1"
        "  AND BN.external_name = :name"
        "  AND BN.status <> :deleted"
        "  AND CS.status <> :deleted"
        "  AND S.status <> :deleted"
        "  ORDER BY S.barcode, C.cryovial_barcode, CS.record_id DESC"
        ;


    m_pq->setSQL(sql.c_str());
    m_pq->setParam("name", box.getName().c_str());
    m_pq->setParam("deleted", LPDbCryovial::DELETED);

    SampleVec mysamples;
    addSamples(&mysamples);
    setNotesForSamples(&mysamples);
    samples->insert(samples->end(), mysamples.begin(), mysamples.end());

    return;
}

void
Db::addSamples( SampleVec * samples, const Person & person ) const
{
    const std::string pfname = Person::getFname();
    const std::string pfterm = (pfname == "")
        ? Util::quote("")
        : "S." + pfname;

    std::string sql =
        "  SELECT S.sample_id, S.barcode sample_barcode"
        "  , C.cryovial_id, C.cryovial_barcode, C.aliquot_type_cid"
        "  , CS.status cryovial_store_status, CS.retrieval_cid"
        "  , CS.note_exists note_exists, CS.record_id cryovial_store_id"
        ;
    sql +=
        "  , " + pfterm + " person_id";
    sql +=
        "  FROM SPECIMEN S"
        "  LEFT JOIN CRYOVIAL C"
        "  ON 1=1"
        "  AND C.sample_id = S.sample_id"
        "  LEFT JOIN CRYOVIAL_STORE CS"
        "  ON 1=1"
        "  AND CS.cryovial_id = C.cryovial_id"
        "  WHERE 1=1";
    sql +=
        "  AND " + pfterm + " = :id";
    sql +=
        "  AND S.status <> :deleted"
        "  ORDER BY S.barcode, C.cryovial_barcode, CS.record_id DESC"
        ;

    m_pq->setSQL(sql.c_str());
    m_pq->setParam("id", person.getName().c_str());
    m_pq->setParam("deleted", LPDbCryovial::DELETED);

    SampleVec mysamples;
    addSamples(&mysamples);
    setNotesForSamples(&mysamples);
    samples->insert(samples->end(), mysamples.begin(), mysamples.end());

	return;
}

void
Db::addSamples( SampleVec * samples, const Job & job ) const
{
    const std::string pfname = Person::getFname();
    const std::string pfterm = (pfname == "")
        ? Util::quote("")
        : "S." + pfname;

    std::string sql =
        "  SELECT S.sample_id, S.barcode sample_barcode"
        "  , C.cryovial_id, C.cryovial_barcode, C.aliquot_type_cid"
        "  , CS.status cryovial_store_status, CS.retrieval_cid"
        "  , CS.note_exists note_exists, CS.record_id cryovial_store_id"
        ;
    sql +=
        "  , " + pfterm + " person_id";
    sql +=
        "  FROM CRYOVIAL_STORE CS"
        "  JOIN CRYOVIAL C"
        "  ON 1=1"
        "  AND C.cryovial_id = CS.cryovial_id"
        "  JOIN SPECIMEN S"
        "  ON 1=1"
        "  AND S.sample_id = C.sample_id"
        "  WHERE 1=1"
        "  AND CS.retrieval_cid = :id"
        "  AND CS.status <> :deleted"
        "  AND S.status <> :deleted"
        "  ORDER BY S.barcode, C.cryovial_barcode, CS.record_id DESC"
        ;

    m_pq->setSQL(sql.c_str());
    m_pq->setParam("id", job.getNumber());
    m_pq->setParam("deleted", LPDbCryovial::DELETED);

    SampleVec mysamples;
    addSamples(&mysamples);
    setNotesForSamples(&mysamples);
    samples->insert(samples->end(), mysamples.begin(), mysamples.end());

    return;
}

void
Db::addSamples( SampleVec * samples ) const
{
    IntPairSet seen;

    for (m_pq->open(); ! m_pq->eof(); m_pq->next())
    {
        const int sample_id =
            m_pq->readInt("sample_id");
        const std::string sample_barcode =
            m_pq->readString("sample_barcode").c_str();
        const std::string person_id =
            m_pq->readString("person_id").c_str();
        const int cryovial_id =
            m_pq->readInt("cryovial_id");
        const std::string cryovial_barcode =
            m_pq->readString("cryovial_barcode").c_str();
        const int aliquot_type_cid =
            m_pq->readInt("aliquot_type_cid");
        const int cryovial_store_status =
            m_pq->readInt("cryovial_store_status");
        const int retrieval_cid =
            m_pq->readInt("retrieval_cid");
        const int note_exists =
            m_pq->readInt("note_exists");
        const int cryovial_store_id =
            m_pq->readInt("cryovial_store_id");

        const IntPair key(sample_id, cryovial_id);

        if (seen.count(key) > 0) continue; // old record

        Sample sample(sample_id);
        sample.setBarcode(sample_barcode);
        sample.setCryovialId(cryovial_id);
        sample.setCryovialBarcode(cryovial_barcode);
        sample.setPersonId(person_id);
        sample.setAliquotId(aliquot_type_cid);
        sample.setCryovialStatus(cryovial_store_status);
        sample.setJobno(retrieval_cid);
        sample.setNoteFlag(note_exists);
        sample.setCryovialStoreId(cryovial_store_id);

        samples->push_back(sample);

        seen.insert(key);
    }

    return;
}

void
Db::setNotesForSamples( SampleVec * samples ) const
{
    std::string sql =
        "  SELECT N.contents, N.operator_cid, N.time_stamp"
        "  FROM NOTE N"
        "  JOIN CRYOVIAL_STORE CS"
        "  ON 1=1"
        "  AND CS.record_id = N.object_id"
        "  WHERE 1=1"
        "  AND CS.note_exists > 0"
        "  AND CS.record_id = :csid"
        "  ORDER BY N.time_stamp"
        ;

//    m_pq->setSQL(sql.c_str()); FIXME

    for (SampleVec::iterator it = samples->begin();
        it != samples->end(); it++)
    {
        Sample & sample = *it;
        const int csid = sample.getCryovialStoreId();
        const int noteFlag = sample.getNoteFlag();
        if (noteFlag == 0) continue;
        if (noteFlag < 0)
        {
            std::string error = "";
            error += "bad note_exists for cryovial csid ";
            error += csid;
            error += " at ";
            error += HERE;
            throw Exception(error.c_str());
        }

        m_pq->setSQL(sql.c_str()); // FIXME

        m_pq->setParam("csid", csid);
        std::string text = "";
        for (m_pq->open(); ! m_pq->eof(); m_pq->next())
        {
            const std::string contents = m_pq->readString("contents").c_str();
			const int userid = m_pq->readInt("operator_cid");
			AnsiString date = m_pq->readDateTime("time_stamp").DateString();
			const std::string when = date.c_str( );
            const LCDbOperator * creator = getUser(userid);
            const std::string who = (creator == 0)
                ? ("user" + Util::asString(userid)).c_str()
                : creator->getDescription().c_str();
            text += contents;
            text += " - ";
            text += "by " + who;
            text += " on " + when;
            text += "\n";
        }
        sample.setNote(text);
    }

    return;
}

std::string
Db::updateSamples( const std::map<int,IntSet> & jobCsids, const int dbcrstatus,
    const std::string & jobName, const std::string & jobDescription,
    const IntToStringMap & sampleNote ) const
{
	std::string error = "";

	do
	{
/*		if (m_pdb->InTransaction)
		{
			error = "unable to start transaction";
			break;
		}
		m_pdb->StartTransaction();
*/
        error = updateSamplesStatus(jobCsids,
            dbcrstatus, jobName, jobDescription);

		if (error != "")
		{
//			m_pdb->Rollback();
            break;
		}

        for (std::map<int,IntSet>::const_iterator it = jobCsids.begin();
            it != jobCsids.end(); it++)
        {
            const int jobno = it->first;
            error = closeJob(jobno);
            if (error != "") break;
        }

        if (error != "")
        {
//            m_pdb->Rollback();
            break;
        }

        error = updateSamplesNote(sampleNote);

		if (error != "")
		{
//			m_pdb->Rollback();
            break;
		}

//        m_pdb->Commit();

	} while (false);

	return error;
}

std::string
Db::updateSamplesStatus( const std::map<int,IntSet> & jobCsids,
    const int dbcrstatus,
    const std::string & jobName, const std::string & jobDescription ) const
{
    std::string error = "";

    {
        std::string sql =
            "  UPDATE CRYOVIAL_STORE CS"
            "  SET status = :dbcrstatus"
            "  , retrieval_cid = :newjobno"
            "  , process_cid = :pid"
            "  WHERE 1=1"
            "  AND CS.status <> :deleted"
            "  AND CS.record_id = :csid"
            "  AND CS.retrieval_cid = :jobno"
            ;

        m_pq->setSQL(sql.c_str());
    }

    m_pq->setParam("deleted", LPDbCryovial::DELETED);
    m_pq->setParam("dbcrstatus", dbcrstatus);
    const int pid = LCDbAuditTrail::getCurrent().getProcessID();
    m_pq->setParam("pid", pid);

    for (std::map<int,IntSet>::const_iterator it1 = jobCsids.begin();
        it1 != jobCsids.end(); it1++)
    {
        const int jobno = it1->first;
        const IntSet csids = it1->second;

        const bool isNewJob = (jobno == 0);
		const LCDbCryoJob * pjob = isNewJob
            ? makeJob(jobName, jobDescription)
            : getJob(jobno);
        if (pjob == 0)
        {
            error = (jobno == 0)
                ? std::string("failed to create job")
                : "failed to find job " + Util::asString(jobno)
                ;
            break;
        }
		LCDbCryoJob job = *pjob;
        if (! job.claim(*m_cq, isNewJob))
        {
            error = "unable to claim job";
            break;
        }
        const int newjobno = job.getID();

        m_pq->setParam("jobno", jobno);
        m_pq->setParam("newjobno", newjobno);

        for (IntSet::const_iterator it2 = csids.begin();
            it2 != csids.end(); it2++)
        {
            const int csid = *it2;
            m_pq->setParam("csid", csid);
			if (!m_pq->execSQL())
            {
                error = "failed to update cryovial csid " +
                    Util::asString(csid);
                break;
            }
        }

        if (! job.release(*m_cq, false))
        {
            error = "unable to release job";
            break;
        }

        if (error != "") break;
    }

    return error;
}

std::string
Db::resetSamples( const std::map<int,IntSet> & jobCsids ) const
{
	std::string error = "";

	do
	{
/*		if (m_pdb->InTransaction)
		{
			error = "unable to start transaction";
			break;
		}
		m_pdb->StartTransaction();
*/
        error = resetSamplesStatus(jobCsids);

		if (error != "")
		{
//			m_pdb->Rollback();
            break;
		}

        for (std::map<int,IntSet>::const_iterator it = jobCsids.begin();
            it != jobCsids.end(); it++)
        {
            const int jobno = it->first;
            error = closeJob(jobno);
            if (error != "") break;
        }

        if (error != "")
        {
//            m_pdb->Rollback();
            break;
        }

//        m_pdb->Commit();

	} while (false);

	return error;
}

std::string
Db::resetSamplesStatus( const std::map<int,IntSet> & jobCsids ) const
{
    std::string error = "";

    {
        std::string sql =
            "  UPDATE CRYOVIAL_STORE CS"
            "  SET status = :dbcrstatus"
            "  , retrieval_cid = :newjobno"
            "  , process_cid = :pid"
            "  WHERE 1=1"
            "  AND CS.status <> :deleted"
            "  AND CS.record_id = :csid"
            "  AND CS.retrieval_cid = :jobno"
            ;

        m_pq->setSQL(sql.c_str());
    }

	m_pq->setParam("deleted", LPDbCryovialStore::DELETED);
	m_pq->setParam("dbcrstatus", LPDbCryovialStore::CONFIRMED);
	const int pid = LCDbAuditTrail::getCurrent().getProcessID();
    m_pq->setParam("pid", pid);

    for (std::map<int,IntSet>::const_iterator it1 = jobCsids.begin();
        it1 != jobCsids.end(); it1++)
    {
        const int jobno = it1->first;
        const IntSet csids = it1->second;

		const LCDbCryoJob * pjob = getJob(jobno);
        if (pjob == 0)
        {
            error = "failed to find job " + Util::asString(jobno);
            break;
        }
        LCDbCryoJob job = *pjob;
        if (! job.claim(*m_cq, false))
        {
            error = "unable to claim job";
            break;
        }
        const int newjobno = job.getID();

        m_pq->setParam("jobno", jobno);
        m_pq->setParam("newjobno", 0);

        for (IntSet::const_iterator it2 = csids.begin();
            it2 != csids.end(); it2++)
        {
            const int csid = *it2;
            m_pq->setParam("csid", csid);
			if (!m_pq->execSQL())
            {
                error = "failed to update cryovial csid " +
                    Util::asString(csid);
                break;
            }
        }

        if (! job.release(*m_cq, false))
        {
            error = "unable to release job";
            break;
        }

        if (error != "") break;
    }
	return error;
}

std::string
Db::calcJobCounts( const int jobno, IntToIntMap * counts ) const
{
    std::string error = "";

    do
    {
        IntToIntMap mycounts;

        if (jobno == 0) break;

        {
            std::string sql =
                "  SELECT status, COUNT(*) n"
                "  FROM CRYOVIAL_STORE CS"
                "  WHERE 1=1"
                "  AND CS.retrieval_cid = :jobno"
                "  GROUP BY CS.status"
                ;

            m_pq->setSQL(sql.c_str());
        }

        m_pq->setParam("jobno", jobno);
        
        for (m_pq->open(); ! m_pq->eof(); m_pq->next())
        {
            const int status = m_pq->readInt("status");
            const int nsamples = m_pq->readInt("n");
            if (nsamples < 0)
            {
                error = "problem checking job " + Util::asString(jobno);
                break;
            }
            if (mycounts.count(status) != 0)
            {
                error = "problem checking job " + Util::asString(jobno);
                break;
            }
            mycounts[status] = nsamples;
        }

        if (error != "") break;

        *counts = mycounts;

    } while (false);

    return error;
}

std::string
Db::closeJob( const int jobno ) const
{
    std::string error = "";

    do
    {
        if (jobno == 0) break;

        IntToIntMap counts;

        error = calcJobCounts(jobno, &counts);
        if (error != "") break;

        int nremaining = 0;
        for (IntToIntMap::const_iterator it = counts.begin();
            it != counts.end(); it++)
        {
            const int status = it->first;
            const int nsamples = it->second;
            if (status == LPDbCryovial::DELETED) continue;
            if (status == LPDbCryovial::DESTROYED) continue;
            nremaining += nsamples;
        }

        if (error != "") break;

        if (nremaining != 0) break;

        const LCDbCryoJob * pjob = getJob(jobno);
        if (pjob == 0)
        {
            error = "failed to find job " + Util::asString(jobno);
            break;
        }

        LCDbCryoJob job = *pjob;
        if (! job.claim(*m_cq, false))
        {
            error = "failed to claim job " + Util::asString(jobno);
            break;
        }

        if (! job.release(*m_cq, true))
        {
            error = "failed to release job " + Util::asString(jobno);
            break;
        }

    } while (false);

    return error;
}

std::string
Db::updateSamplesNote( const IntToStringMap & sampleNote ) const
{
    std::string error = "";

    for (IntToStringMap::const_iterator it = sampleNote.begin();
        it != sampleNote.end(); it++)
    {
        const int csid = it->first;
        const std::string noteText = it->second;
        LPDbNote note(noteText.c_str(), csid);
        note.saveRecord(*m_pq);
    }

    {
        std::string sql =
            "  UPDATE CRYOVIAL_STORE CS"
            "  SET note_exists = note_exists + 1"
            "  , process_cid = :pid"
            "  WHERE 1=1"
            "  AND CS.status <> :deleted"
            "  AND CS.record_id = :csid"
            "  AND CS.note_exists >= 0"
            ;

        m_pq->setSQL(sql.c_str());

        m_pq->setParam("deleted", LPDbCryovial::DELETED);
		const int pid = LCDbAuditTrail::getCurrent().getProcessID();
        m_pq->setParam("pid", pid);

        for (IntToStringMap::const_iterator it = sampleNote.begin();
            it != sampleNote.end(); it++)
        {
            const int csid = it->first;

            m_pq->setParam("csid", csid);
            if (!m_pq->execSQL())
            {
                error = "failed to update cryovial csid " +
                    Util::asString(csid);
                break;
            }
        }
    }
    
    return error;
}

std::string
Db::getJobOwner( const int jobno ) const
{
    std::string owner = "";

    do
    {
        const LCDbCryoJob * job = getJob(jobno);
        if (job == 0) break;

        const int userid = job->getUserID();

        const LCDbOperator * myoperator = getUser(userid);
        if (myoperator == 0) break;

        owner = myoperator->getDescription().c_str();

    } while (false);

    return owner;
}

bool
Db::canAbort( const int jobno ) const
{
    std::string diagnosis = "";

    do
    {
        const LCDbCryoJob * job = getJob(jobno);
        if (job == 0)
        {
            diagnosis = "job " + Util::asString(jobno) + " is not valid";
            break;
        }

        std::string sql =
            "  SELECT CS.record_id csid, CS.status csst"
            "  FROM CRYOVIAL_STORE CS"
            "  JOIN CRYOVIAL C"
            "  ON 1=1"
            "  AND C.cryovial_id = CS.cryovial_id"
            "  JOIN SPECIMEN S"
            "  ON 1=1"
            "  AND S.sample_id = C.sample_id"
            "  WHERE 1=1"
            "  AND CS.retrieval_cid = :id"
            "  AND CS.status <> :deleted"
            "  AND S.status <> :deleted"
            ;

        m_pq->setSQL(sql.c_str());
        m_pq->setParam("id", jobno);
        m_pq->setParam("deleted", LPDbCryovial::DELETED);

        for (m_pq->open(); ! m_pq->eof(); m_pq->next())
        {
            const int csid = m_pq->readInt("csid");
            const int csst = m_pq->readInt("csst");
			if (csst != LPDbCryovialStore::CONFIRMED)
            {
                diagnosis =
                    "cryovial_store entry " + Util::asString(csid) +
                 " has status " + Util::asString(csst)
                 ;
                 break;
            }
        }

    } while (false);

    return diagnosis == "";
}

std::string
Db::getJobName( const int jobno ) const
{
    std::string description = "";

    do
    {
        const LCDbCryoJob * job = getJob(jobno);
        if (job == 0) break;

        description = job->getName().c_str();

    } while (false);

    return description;
}

std::string
Db::getJobDescription( const int jobno ) const
{
    std::string description = "";

    do
    {
        const LCDbCryoJob * job = getJob(jobno);
        if (job == 0) break;

        description = job->getDescription().c_str();

    } while (false);

    return description;
}

time_t
Db::getJobCreationUtime( const int jobno ) const
{
    time_t utime = -1;

    do
    {
        const LCDbCryoJob * job = getJob(jobno);
        if (job == 0) break;

        const TDateTime epoch(1970, 1, 1);
        const int spd = 60 * 60 * 24;
        const TDateTime jdt = job->getTimeStamp();
        utime = (time_t) (0.5 + ((double) jdt - (double) epoch) * spd);

    } while (false);

    return utime;
}

const LCDbOperator *
Db::getCurrentUser( ) const
{
    const int userid = LCDbAuditTrail::getCurrent().findUserID(
        LCDbAuditTrail::getCurrent().getProcessID() );

    return getUser(userid);
}

std::string
Db::getCurrentUserName( ) const
{
    std::string name = "";

    do
    {
        const LCDbOperator * user = getCurrentUser();
        if (user == 0) break;
        name = user->getName().c_str();

    } while (false);

    return name;
}

StringVec
Db::getProjectNames( ) const
{
    StringVec names;
	for (Range<LCDbProject> pl = LCDbProjects::records(); pl.isValid(); ++pl) {
		if( pl->isValid() && pl->isActive() && !pl->isCentral() ) {
			names.push_back(pl->getName().c_str());
        }
	}
    return names;
}

int
Db::allocCids( const size_t count ) const
{
    LCDbID myLCDbID;
    myLCDbID.claimNextID(*m_cq, count);
    return myLCDbID.getID();
}

bool
Db::addAuditEntry( const std::string & message ) const
{
    bool ok = false;

    do
    {
        LCDbAuditTrail & trail = LCDbAuditTrail::getCurrent();
        if (! trail.addRecord(message.c_str(), LCDbAuditTrail::INFORMATION))
        {
            break;
        }

        ok = true;

    } while (false);

    return ok;
}

// AliquotInfo

AliquotInfo::AliquotInfo( )
{
}

void
AliquotInfo::populate( )
{
	LCDbObjects& objs = LCDbObjects::records();
	for (Range<LCDbObject> ul = objs; ul.isValid(); ++ul )
	{
		if (! ul->isActive()) continue;
		if (ul->getObjectType() !=
			LCDbObject::Category(LCDbObject::ALIQUOT_TYPE)) continue;
		int id = ul->getID();
		std::string name = ul->getName().c_str();
		set(id, name);
	}

	return;
}

// CrstatusInfo

CrstatusInfo::CrstatusInfo( )
{
}

void
CrstatusInfo::populate( )
{
	set(Cryovial::EXPECTED,    LPDbCryovialStore::ALLOCATED);
	set(Cryovial::CONFIRMED,   LPDbCryovialStore::CONFIRMED);
	set(Cryovial::REMOVED,     LPDbCryovialStore::ANALYSED);
	set(Cryovial::DESTROYED,   LPDbCryovialStore::DESTROYED);
    set(Cryovial::NINETYNINED, LPDbCryovialStore::DELETED);
    return;
}


} // Discard

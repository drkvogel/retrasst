#pragma hdrstop

#include "DiscardContext.h"

#pragma package(smart_init)


namespace Discard {

// SampleHandler

SampleHandler::SampleHandler( ) { }

int SampleHandler::operator()( SamplePile * samples, const int sampleno ) const {
	int nchanged = 0;
	return nchanged;
}

bool SampleHandler::isMarkable( SamplePile * samples, const int sampleno ) const {
    const Sample * sample = samples->getSample(sampleno);
    return (sample != 0) && (samples->isSampleMarkable(*sample));
}


// MarkSampleHandler

MarkSampleHandler::MarkSampleHandler( ) { }

int MarkSampleHandler::operator()( SamplePile * samples, const int sampleno ) const {
	int nchanged = 0;
    do {
        if (! isMarkable(samples, sampleno)) break;

        nchanged += samples->setMarked(sampleno);

	} while (false);
    return nchanged;
}


// UnmarkSampleHandler

UnmarkSampleHandler::UnmarkSampleHandler( ) { }

int UnmarkSampleHandler::operator()( SamplePile * samples, const int sampleno ) const {
	int nchanged = 0;
    do {
        if (! isMarkable(samples, sampleno)) break;

        nchanged += samples->clearMarked(sampleno);

	} while (false);
    return nchanged;
}


// ToggleMarkSampleHandler

ToggleMarkSampleHandler::ToggleMarkSampleHandler( ) { }

int ToggleMarkSampleHandler::operator()( SamplePile * samples, const int sampleno ) const {
	int nchanged = 0;
	do {
		if (! isMarkable(samples, sampleno)) break;

		nchanged += samples->toggleMarked(sampleno);
	} while (false);
	return nchanged;
}


// NoteSampleHandler

NoteSampleHandler::NoteSampleHandler( ) { }

int NoteSampleHandler::operator()( SamplePile * samples, const int sampleno ) const {
	int nchanged = 0;
	do {
//        if (! isMarkable(samples, sampleno)) break;

		nchanged += samples->setNoteFromContext(sampleno);

	} while (false);
	return nchanged;
}


// SCComparator

SCComparator::SCComparator( const int sortcolno )
	: m_sortcolnos()
	, m_scalidToSampleno()
	, m_alnoToAlid()
	, m_samples(0) {
	m_sortcolnos.push_front(sortcolno);
}

void SCComparator::insertSortcolno( const int abssortcolno, const int direction ) {
	const int lastsortcolno = m_sortcolnos.front();
	int sortcolno = abssortcolno;
	switch (direction) {
	case 1:
		break;
	case -1:
		sortcolno = -sortcolno;
		break;
	case 0:
	default:
		if (abssortcolno == std::abs(lastsortcolno)) {
			sortcolno = -lastsortcolno;
		}
		break;
	}
	if (sortcolno != lastsortcolno) {
		m_sortcolnos.push_front(sortcolno);
	}
	return;
}

int SCComparator::getSortPosition( const int abssortcolno ) const {
	int position = 0;
	int index = 0;
	for (std::list<int>::const_iterator it=m_sortcolnos.begin();
			it !=m_sortcolnos.end(); it++) {
		const int sortcolno = *it;
		++index;
		if (sortcolno == abssortcolno) {
			position = index;
			break;
		}
		if (sortcolno == -abssortcolno) {
			position = -index;
			break;
		}
	}
	return position;
}

void SCComparator::setScalidToSampleno(
	const std::map<IntPair,int> & scalidToSampleno ) {
	m_scalidToSampleno = scalidToSampleno;
	return;
}

void SCComparator::setAlnoToAlid( const IntToIntMap & alnoToAlid ) {
	m_alnoToAlid = alnoToAlid;
	return;
}

void SCComparator::setSamples( const SamplePile * samples ) {
	m_samples = samples;
	return;
}

bool SCComparator::operator()( const int & left, const int & right ) const {
	int sigma = 0;

	for (std::list<int>::const_iterator it = m_sortcolnos.begin(); it != m_sortcolnos.end(); it++) {
		const int sortcolno = *it;
		const int abssortcolno = std::abs(sortcolno);

		do {
			if (abssortcolno == RESULTNO) {
				sigma = Util::sigma(left - right);
				break;
			}

			const int alid =
				Util::at(m_alnoToAlid, abssortcolno - ALIQUOT0, 0);

			const Sample * leftSample = getSample(left, alid);
			const Sample * rightSample = getSample(right, alid);

			switch (abssortcolno) {
			case SBARCODE:
			case CBARCODE:
			case PERSONID:
				if ((leftSample == 0) || (rightSample == 0)) {
					std::string message = "";
					message += "null sample";
					message += " at ";
					message += HERE;
					throw Exception(message.c_str());
				}
				break;
			}

			if (abssortcolno == SBARCODE) {
				sigma = leftSample->getBarcode().compare(
							rightSample->getBarcode());
				break;
			}

			if (abssortcolno == CBARCODE) {
				sigma = leftSample->getCryovialBarcode().compare(rightSample->getCryovialBarcode());
				break;
			}

			if (abssortcolno == PERSONID) {
				sigma = leftSample->getPersonId().compare(rightSample->getPersonId());
				break;
			}

			if (leftSample == 0) {
				sigma = (rightSample == 0) ? 0 : 1;
			} else {
				sigma = (rightSample == 0)
						? -1
						: Util::sigma(leftSample->getCryovialStoreStatus() -
									  rightSample->getCryovialStoreStatus());
			}

		} while (false);

		if (sigma != 0) {
			if (sortcolno != abssortcolno) {
				sigma = -sigma;
			}
			break;
		}
	}

	return sigma < 0;
}

const Sample * SCComparator::getSample( const int scid, const int alid ) const {
	int sampleno = -1;
	do {
		const IntPair scalid(scid, alid);
		if (m_scalidToSampleno.count(scalid) != 1) break;
		sampleno = m_scalidToSampleno.find(scalid)->second;

	} while (false);

    return m_samples->getSample(sampleno);
}


// Context

MarkSampleHandler Context::s_marker;
UnmarkSampleHandler Context::s_unmarker;
ToggleMarkSampleHandler Context::s_toggleMarker;
NoteSampleHandler Context::s_noter;

Context::Context( Db * db )
    : m_db(db)
    , m_jobno(0) {
    configDbCrstatusMap();
    m_crstatus.init();
    m_alinfo.init();
}

void Context::setProjectName( const std::string & name ) {
	m_db->setProjectName(name);
	return;
}

const std::string & Context::getProjectName( ) const {
	return m_db->getProjectName();
}

std::string Context::getStudyCode( ) const {
	return m_db->getStudyCode();
}

void Context::configCrstatus( const int index ) {
    switch (index) {
    case 0:
		m_crstatus.setCurrentId(Cryovial::STORED);
		m_crstatus.setNextId(Cryovial::MARKED);
		break;
    case 1:
		m_crstatus.setCurrentId(Cryovial::MARKED);
		m_crstatus.setNextId(Cryovial::REMOVED);
        break;
    case 2:
        m_crstatus.setCurrentId(Cryovial::REMOVED);
        m_crstatus.setNextId(Cryovial::DESTROYED);
        break;
    default:
        break;
    }

    return;
}

int Context::getCurrentCrstatus( ) const {
	return m_crstatus.getCurrentId();
}

int Context::getNextCrstatus( ) const {
	return m_crstatus.getNextId();
}

int Context::getCurrentDbCrstatus( ) const {
	return calcDbCrstatus(getCurrentCrstatus());
}

int Context::getNextDbCrstatus( ) const {
	return calcDbCrstatus(getNextCrstatus());
}

std::string Context::getCurrentCrstatusName( ) const {
	return m_crstatus.getName(m_crstatus.getCurrentId());
}

std::string Context::getNextCrstatusName( ) const {
	return m_crstatus.getName(m_crstatus.getNextId());
}

bool Context::isCreateJobStage( ) const {
	return m_crstatus.getCurrentId() == Cryovial::STORED;
}

bool Context::isSelectJobStage( ) const {
	return m_crstatus.getNextId() == Cryovial::DESTROYED;
}

void Context::setReason( const std::string & reason ) {
	m_reason = reason;
	return;
}

const std::string & Context::getReason( ) const {
	return m_reason;
}

void Context::setDescription( const std::string & description ) {
	m_description = description;
	return;
}

const std::string & Context::getDescription( ) const {
	return m_description;
}

void Context::setMethod( const std::string & method ) {
	m_method = method;
	return;
}

const std::string & Context::getMethod( ) const {
	return m_method;
}

void Context::setNote( const std::string & note ) {
	m_note = note;
	return;
}

const std::string & Context::getNote( ) const {
	return m_note;
}

const MarkSampleHandler & Context::getMarker( ) {
	return s_marker;
}

const UnmarkSampleHandler & Context::getUnmarker( ) {
	return s_unmarker;
}

const ToggleMarkSampleHandler & Context::getToggleMarker( ) {
	return s_toggleMarker;
}

const NoteSampleHandler & Context::getNoter( ) {
	return s_noter;
}

IntVec Context::getAliquotIds( ) const {
	return m_alinfo.getIds();
}

std::string Context::getAliquotName( const int aliquotId ) const {
	return m_alinfo.getName(aliquotId);
}

void Context::configPersonFname( ) {
	Person::setFname(m_db->calcPersonFname());
	return;
}

std::string Context::getPersonFname( ) const {
	return m_personFname;
}

IntVec Context::getJobnos( ) const {
	IntVec jobnos;
//    m_db->addJobnos(&jobnos, getCurrentDbCrstatus());
	m_db->addJobnos(&jobnos, -1);
	return jobnos;
}

std::string Context::calcJobPrompt( int jobno ) const {
	if (jobno == 0) jobno = m_jobno;

	std::string prompt = Util::asString(jobno);

    do {
        if (! m_db->isJob(jobno)) break;

        const std::string who = getJobOwner(jobno);
        const std::string when = Util::ddmmyyyy(getJobCreationUtime(jobno));
        const std::string what =
            getJobDescription(jobno) + " / " + getJobReason(jobno);

        prompt = who + " - " + when + " - " + what;

    } while (false);

    return prompt;
}

std::string Context::calcJobDescription( int jobno ) const {
    if (jobno == 0) jobno = m_jobno;

    std::string description = Util::asString(jobno);

    do {
        if (! m_db->isJob(jobno)) break;

        const std::string what =
            getJobDescription(jobno) + " / " + getJobReason(jobno);
        const std::string who = getJobOwner(jobno);
        const std::string when = Util::ddmmyyyy(getJobCreationUtime(jobno));

        description = what;
        description += " - ";
        description += " by " + who;
        description += " on " + when;
        description += "\n";

    } while (false);

    return description;
}

std::string Context::calcJobStatus( int jobno ) const {
    if (jobno == 0) jobno = m_jobno;

    std::string text = "";

    do {
        if (! m_db->isJob(jobno)) break;

        IntToIntMap counts;
        if (m_db->calcJobCounts(jobno, &counts) != "") break;

        IntVec statuses;
        for (IntToIntMap::const_iterator it = counts.begin();
                it != counts.end(); it++) {
            statuses.push_back(it->first);
        }
        std::sort(statuses.begin(), statuses.end());

        for (IntVec::const_iterator it = statuses.begin();
                it != statuses.end(); it++) {
            const int status = *it;
            const int nsamples = Util::at(counts, status, 0);
            text +=
                m_crstatus.getName(status) +
                ": " + Util::asString(nsamples) +
                "\n";
        }

    } while (false);

    return text;
}

bool Context::canAbort( int jobno ) const {
    if (jobno == 0) jobno = m_jobno;

    return m_db->canAbort(jobno);
}

std::string Context::getJobOwner( int jobno ) const {
    if (jobno == 0) jobno = m_jobno;

    return m_db->getJobOwner(jobno);
}

std::string Context::getJobDescription( int jobno ) const {
    if (jobno == 0) jobno = m_jobno;

    return m_db->getJobName(jobno); // sic
}

std::string Context::getJobReason( int jobno ) const {
    if (jobno == 0) jobno = m_jobno;

    return m_db->getJobDescription(jobno); // sic
}

time_t Context::getJobCreationUtime( int jobno ) const {
    if (jobno == 0) jobno = m_jobno;

    return m_db->getJobCreationUtime(jobno);
}

void Context::setSearchType( const std::string & searchType ) {
    m_searchType = searchType;
    return;
}

const std::string & Context::getSearchType( ) const {
    return m_searchType;
}

void Context::setSearchSource( const std::string & searchSource ) {
    m_searchSource = searchSource;
    return;
}

const std::string &
Context::getSearchSource( ) const {
    return m_searchSource;
}

void Context::configSearchTexts( const std::string & searchText ) {
    do {
        m_searchTexts.clear();

        if (m_searchSource == "File") {
            const std::string filename = searchText;
            std::ifstream ifs;
            ifs.open(filename.c_str());
            if (ifs) {
                std::string line = "";
                while (getline(ifs, line)) {
                    if (Util::trim(line) == "") continue;
                    const std::string text = line;
                    m_searchTexts.insert(text);
                }
                ifs.close();
            }
            break;
        }

		if (m_searchSource == "Keyboard") {
            m_searchTexts.insert(searchText);
            break;
        }

        {
            std::string message = "";
            message += "bad search source";
            message += " at ";
            message += HERE;
            throw Exception(message.c_str());
        }

    } while (false);

    return;
}

const StringSet &
Context::getSearchTexts( ) const {
    return m_searchTexts;
}

int Context::calcDbCrstatus( const int crstatus ) const {
	int dbCrstatus = -1;

	do {
		IntToIntMap::const_iterator it = m_dbCrstatusMap.find(crstatus);
		if (it == m_dbCrstatusMap.end()) break;
		dbCrstatus = it->second;

	} while (false);

	return dbCrstatus;
}

int Context::calcCrstatus( const int dbcrstatus ) const {
	int crstatus = -1;

	do {
		IntToIntMap::const_iterator it = m_crstatusMap.find(dbcrstatus);
		if (it == m_crstatusMap.end()) break;
		crstatus = it->second;

	} while (false);

	return crstatus;
}

void Context::configDbCrstatusMap( ) {
	CrstatusInfo crstatusInfo;
	crstatusInfo.init();
	for( auto it : crstatusInfo.getMapping() ) {
		const int crstatus = it.second;
		const int dbcrstatus = it.first;
		m_dbCrstatusMap[crstatus] = dbcrstatus;
		m_crstatusMap[dbcrstatus] = crstatus;
	}
	return;
}

Db * Context::getDb( ) {
	return m_db;
}

void Context::setSelectMode( const int mode ) {
	m_selectMode = mode;
}

int Context::getSelectMode( ) const {
	return m_selectMode;
}

bool Context::isMarking( ) const {
	return m_selectMode == MARK;
}

bool Context::isNoting( ) const {
	return m_selectMode == NOTE;
}

void Context::setSaved( const bool isSaved ) {
	m_isSaved = isSaved;
	return;
}

bool Context::isSaved( ) const {
	return m_isSaved;
}

int Context::allocCids( const size_t count ) const {
	return m_db->allocCids(count);
}

std::string Context::getCurrentUserName( ) const {
	return m_db->getCurrentUserName();
}

bool Context::addAuditEntry( const std::string & message ) const {
	return m_db->addAuditEntry(message);
}

void Context::setJobno( const int jobno ) {
	m_jobno = jobno;
	return;
}

int Context::getJobno( ) const {
	return m_jobno;
}

// SamplePile

SamplePile::SamplePile( ) {
	clear();
}

void SamplePile::init( Discard::Context * context ) {
	if (context == 0) {
		std::string message = "";
		message += "null context";
		message += " at ";
		message += HERE;
		throw Exception(message.c_str());
	}

	m_context = context;
	clear();
	return;
}

IntPair SamplePile::add( const Tube & tube ) {
	const int oldnsamples = count();
	int nfound = 0;

	do {
		SampleVec samples;
		m_context->getDb()->addSamples(&samples, tube);
		nfound = samples.size();

		for (std::vector<Sample>::const_iterator it = samples.begin();
				it != samples.end(); it++) {
			const Sample sample = *it;
			add(sample);
			m_jobnos.insert(sample.getJobno());
		}

	} while (false);

	const int nadded = count() - oldnsamples;

	return std::make_pair(nfound, nadded);
}

IntPair SamplePile::add( const Cryovial & cryovial ) {
	const int oldnsamples = count();
	int nfound = 0;

	do {
		SampleVec samples;
		m_context->getDb()->addSamples(&samples, cryovial);
		nfound = samples.size();

		for (std::vector<Sample>::const_iterator it = samples.begin();
				it != samples.end(); it++) {
			const Sample sample = *it;
			add(sample);
			m_jobnos.insert(sample.getJobno());
		}

	} while (false);

	const int nadded = count() - oldnsamples;

	return std::make_pair(nfound, nadded);
}

IntPair SamplePile::add( const Box & box ) {
	const int oldnsamples = count();
	int nfound = 0;

	do {
		SampleVec samples;
		m_context->getDb()->addSamples(&samples, box);
		nfound = samples.size();

		for (std::vector<Sample>::const_iterator it = samples.begin();
				it != samples.end(); it++) {
			const Sample sample = *it;
			add(sample);
			m_jobnos.insert(sample.getJobno());
		}

	} while (false);

	const int nadded = count() - oldnsamples;

	return std::make_pair(nfound, nadded);
}

IntPair SamplePile::add( const Person & person ) {
	const int oldnsamples = count();
	int nfound = 0;

	do {
		SampleVec samples;
		m_context->getDb()->addSamples(&samples, person);
		nfound = samples.size();

		for (std::vector<Sample>::const_iterator it = samples.begin();
				it != samples.end(); it++) {
			const Sample sample = *it;
			add(sample);
			m_jobnos.insert(sample.getJobno());
		}

	} while (false);

	const int nadded = count() - oldnsamples;

	return std::make_pair(nfound, nadded);
}

IntPair SamplePile::add( const Job & job ) {
	const int oldnsamples = count();
	int nfound = 0;

	do {
		SampleVec samples;
		m_context->getDb()->addSamples(&samples, job);
		nfound = samples.size();

		for (std::vector<Sample>::const_iterator it = samples.begin();
				it != samples.end(); it++) {
			const Sample sample = *it;
			add(sample);
			m_jobnos.insert(sample.getJobno());
		}

	} while (false);

	const int nadded = count() - oldnsamples;
	return std::make_pair(nfound, nadded);
}

void SamplePile::add( const Sample & sample ) {
	do {
		const int crid = sample.getCryovialId();
		const int n = m_crids.count(crid);

		if (n == 1) break;

		if (n != 0) {
			std::string message = "";
			message += "suspect map";
			message += " at ";
			message += HERE;
			throw Exception(message.c_str());
		}

		m_samples.push_back(Sample(sample));
		m_crids.insert(crid);

		if (! isSampleMarkable(sample)) break;

		const int sampleno = count() - 1;
		this->setMarked(sampleno);


	} while (false);

	return;
}

void SamplePile::clear( ) {
	m_samples.clear();
	m_crids.clear();
	m_ismarked.clear();
	m_jobnos.clear();
	m_note.clear();
    return;
}

int SamplePile::count( ) const {
	return m_samples.size();
}

std::string SamplePile::asString( ) const {
	std::string text = "";
	std::string line = "";
	text += line + "\n";
	for (std::vector<Sample>::const_iterator it = m_samples.begin(); it != m_samples.end(); it++) {
		const Sample sample = *it;
		line = "";
		line += " sample_id=" + Util::asString(sample.getId());
		line += " sample_barcode=" + sample.getBarcode();
		line += " cryovial_id=" + Util::asString(sample.getCryovialId());
		line += " cryovial_barcode=" + sample.getCryovialBarcode();
		const int aliquotId = sample.getAliquotId();
		line += " aliquot_id=" + Util::asString(aliquotId);
		const std::string aliquotName = m_context->getAliquotName(aliquotId);
		line += " aliquot_name=" + aliquotName;
		text += line + "\n";
	}
	return text;
}

const Sample * SamplePile::getSample( const int sampleno ) const {
	return ((sampleno >= 0) && (sampleno < (int) (m_samples.size())))
		   ? &(m_samples[sampleno])
		   : 0;
}

std::string SamplePile::update( const int dbcrstatus ) {
//, const std::string & description, const std::string & reason ) {
	std::string error = "";

	do {
		std::map<int,IntSet> jobCsids;
		IntToStringMap sampleNote;

		const int nsamples = count();
		for (int sampleno=0; sampleno<nsamples; sampleno++) {
			const Sample * sample = getSample(sampleno);
			if (sample == 0) continue;
			if (! isMarked(sampleno)) continue;
			const int csid = sample->getCryovialStoreId();
			const std::string note = getNote(sampleno);
			if (note != "") sampleNote[csid] = note;
			const int jobno = sample->getJobno();
			if (m_jobnos.count(jobno) != 1) continue;
			jobCsids[jobno].insert(csid);
		}

		error = m_context->getDb()->updateSamples(jobCsids,
				dbcrstatus, /* jobName, jobDescription, */ sampleNote);
		if (error != "") break;

	} while (false);

	return error;
}

std::string SamplePile::reset( ) {
	std::string error = "";

	do {
		std::map<int,IntSet> jobCsids;

		const int nsamples = count();
		for (int sampleno=0; sampleno<nsamples; sampleno++) {
			const Sample * sample = getSample(sampleno);
			if (sample == 0) continue;
			const int csid = sample->getCryovialStoreId();
			const int jobno = sample->getJobno();
			if (m_jobnos.count(jobno) != 1) continue;
			jobCsids[jobno].insert(csid);
		}

		error = m_context->getDb()->resetSamples(jobCsids);
		if (error != "") break;

	} while (false);

	return error;
}

int SamplePile::getNCryovialsRemaining( const int jobno ) const {
	int nRemaining = -1;

	do {
		int nExcused = 0;
        int nExpected = 0;
        int nConfirmed = 0;
        int nRemoved = 0;
        int nDestroyed = 0;
        int n99d = 0;
        int nUnexpected = 0;

        Job job(jobno);
        SampleVec samples;
        m_context->getDb()->addSamples(&samples, job);

        for (std::vector<Sample>::const_iterator it = samples.begin();
                it != samples.end(); it++) {
            const Sample sample = *it;
            const bool isExcused = false; // FIXME 57
            if (isExcused) {
                ++nExcused;
                continue;
            }
			const int dbcrstatus = sample.getCryovialStoreStatus();
			const int crstatus = m_context->calcCrstatus(dbcrstatus);
		  if (crstatus == Cryovial::MARKED) {
				++nExpected;
				continue;
			}
		  if (crstatus == Cryovial::STORED) {
                ++nConfirmed;
                continue;
            }
            if (crstatus == Cryovial::REMOVED) {
                ++nRemoved;
                continue;
            }
            if (crstatus == Cryovial::DESTROYED) {
                ++nDestroyed;
                continue;
            }
            if (crstatus == Cryovial::NINETYNINED) {
                ++n99d;
                continue;
            }
            ++nUnexpected;
        }

        if (nExpected > 0) {
            std::string message = "";
            message += "suspect sample";
            message += " at ";
            message += HERE;
			throw Exception(message.c_str());
        }
        if (nUnexpected > 0) {
            std::string message = "";
            message += "suspect sample";
            message += " at ";
            message += HERE;
            throw Exception(message.c_str());
        }

        nRemaining = nConfirmed + nRemoved;

    } while (false);

    return nRemaining;
}

bool SamplePile::isSampleMarkable( const Sample & sample ) const {
	bool isMarkable = false;

	do {
		const int dbcrstatus = sample.getCryovialStoreStatus();
		const int crstatus = m_context->calcCrstatus(dbcrstatus);
		if (crstatus != m_context->getCurrentCrstatus()) break;

		const int jobno = sample.getJobno();
		const bool isCreateJobStage = m_context->isCreateJobStage();
		if (isCreateJobStage && (jobno != 0)) break;
		const bool isSelectJobStage = m_context->isSelectJobStage();
		if (isSelectJobStage && (jobno == 0)) break;

		isMarkable = true;

	} while (false);

	return isMarkable;
}

int SamplePile::setMarked( const int sampleno ) {
	int nchanged = 0;
	if (isMarked(sampleno) == 0) {
		m_ismarked.insert(sampleno);
		++nchanged;
	}
	return nchanged;
}

int SamplePile::clearMarked( const int sampleno ) {
	int nchanged = 0;
	if (isMarked(sampleno) == 1) {
		m_ismarked.erase(sampleno);
		++nchanged;
	}
	return nchanged;
}

int SamplePile::toggleMarked( const int sampleno ) {
    int nchanged = 0;
    do {
        const int myIsMarked = isMarked(sampleno);
        if (myIsMarked == 0) {
            nchanged += setMarked(sampleno);
            break;
        }
        if (myIsMarked == 1) {
            nchanged += clearMarked(sampleno);
            break;
        }

    } while (false);

    return nchanged;
}

int SamplePile::isMarked( const int sampleno ) {
    const int n = m_ismarked.count(sampleno);
    return ((n == 0) || (n == 1)) ? n : -1;
}

int SamplePile::getNMarked( ) const {
    return m_ismarked.size();
}

IntSet::const_iterator SamplePile::getMarkedBegin( ) const {
	return m_ismarked.begin();
}

IntSet::const_iterator SamplePile::getMarkedEnd( ) const {
	return m_ismarked.end();
}

int SamplePile::setNoteFromContext( const int sampleno ) {
	return setNote(sampleno, m_context->getNote());
}

int SamplePile::setNote( const int sampleno, const std::string & note ) {
	int nchanged = 0;
	do {
		std::string oldNote = getNote(sampleno);
		if (note == oldNote) break;
		if (note == "") {
			m_note.erase(sampleno);
		} else {
			m_note[sampleno] = note;
		}
		++nchanged;

	} while (false);

	return nchanged;
}

std::string SamplePile::getNote( const int sampleno ) const {
	return Util::at(m_note, sampleno);
}

int SamplePile::getNNoted( ) const {
	return m_note.size();
}

IntVec SamplePile::getAliquotIds( ) const {
	return m_context->getAliquotIds();
}

SCComparator * SamplePile::getSCComparator( ) {
	return &m_scc;
}

std::string SamplePile::getAliquotName( const int alid ) const {
    return m_context->getAliquotName(alid);
}


// GridStuff

GridStuff::GridStuff( const SamplePile & samples, const std::string & utext, const int usampleno )
    : m_samples(samples)
    , m_utext(utext)
    , m_usampleno(usampleno)
    , m_ncols(-1)
    , m_nrows(-1) {
    init();
}

int GridStuff::getNcols( ) const {
    return m_ncols;
}

int GridStuff::getNrows( ) const {
    return m_nrows;
}

void GridStuff::setText( const Cell & cell, const std::string & text ) {
    m_text[cell] = text;
    return;
}

const std::string & GridStuff::getText( const Cell & cell ) {
    std::map<Cell,std::string>::const_iterator it = m_text.find(cell);
    return (it == m_text.end())
           ? m_utext
           : it->second;
}

std::map<Cell,std::string>::const_iterator
GridStuff::getTextBegin( ) const {
    return m_text.begin();
}

std::map<Cell,std::string>::const_iterator
GridStuff::getTextEnd( ) const {
    return m_text.end();
}

int GridStuff::getFirstSampleColno( ) const {
    return m_firstSampleColno;
}

int GridStuff::getFirstSampleRowno( ) const {
    return m_firstSampleRowno;
}

void GridStuff::setHeader( const Cell & cell ) {
    m_header.insert(cell);
    return;
}

bool GridStuff::isHeader( const Cell & cell ) const {
    return m_header.find(cell) != m_header.end();
}

std::set<Cell>::const_iterator
GridStuff::getHeaderBegin( ) const {
    return m_header.begin();
}

std::set<Cell>::const_iterator
GridStuff::getHeaderEnd( ) const {
    return m_header.end();
}

void GridStuff::setSampleno( const Cell & cell, const int sampleno ) {
    m_sampleno[cell] = sampleno;
    return;
}

const int GridStuff::getSampleno( const Cell & cell ) {
    std::map<Cell,int>::const_iterator it = m_sampleno.find(cell);
    return (it == m_sampleno.end())
           ? m_usampleno
           : it->second;
}

std::map<Cell,int>::const_iterator
GridStuff::getCellSamplenoBegin( ) const {
    return m_sampleno.begin();
}

std::map<Cell,int>::const_iterator
GridStuff::getCellSamplenoEnd( ) const {
    return m_sampleno.end();
}

void GridStuff::init( ) {
    m_firstSampleColno = -1;
    m_firstSampleRowno = -1;

    m_text.clear();
    m_header.clear();
    m_sampleno.clear();

    m_scidToScno.clear();
    m_alidToAlno.clear();
    m_alnoToAlid.clear();
    m_scalidToSampleno.clear();
    m_samplenoToScid.clear();

    return;
}

void GridStuff::analyze( SCComparator * scc ) {
    std::map<StringPair,int> scbarcodeToScid;
    IntVec scids;

    {
        IntSet seenAlids;
        const int nsamples = m_samples.count();
        for (int sampleno=0; sampleno<nsamples; sampleno++) {
            const Sample * sample = m_samples.getSample(sampleno);
            if (sample == 0) {
                std::string message = "";
                message += "null sample";
                message += " at ";
                message += HERE;
                throw Exception(message.c_str());
            }

            const int alid = sample->getAliquotId();
            seenAlids.insert(alid);

            const StringPair scbarcode(sample->getBarcode(),
                                       sample->getCryovialBarcode());
            const size_t n = scbarcodeToScid.count(scbarcode);
            const bool isNew = (n == 0);
            const bool isOld = (n == 1);
            if ((! isNew) && (! isOld)) {
                std::string message = "";
                message += "internal error";
                message += " at ";
                message += HERE;
                throw Exception(message.c_str());
            }

            int scid = -1;
            if (isNew) {
                scid = 1 + scbarcodeToScid.size();
                scids.push_back(scid);
                scbarcodeToScid[scbarcode] = scid;
            }
            scid = scbarcodeToScid[scbarcode];
            m_samplenoToScid[sampleno] = scid;
            {
                const IntPair scalid(scid, alid);
                m_scalidToSampleno[scalid] = sampleno;
            }
            {
                const IntPair scalid(scid, 0);
                if (m_scalidToSampleno.count(scalid) == 0) {
                    m_scalidToSampleno[scalid] = sampleno;
                }
            }
        }

        IntVec alids;
        IntVec allAlids = m_samples.getAliquotIds();
        for (IntVec::const_iterator it = allAlids.begin();
                it != allAlids.end(); it++) {
            int alid = *it;
            const int nseen = seenAlids.count(alid);
            if (nseen == 0) continue;
            if (nseen != 1) {
                std::string message = "";
                message += "internal error";
                message += " at ";
                message += HERE;
                throw Exception(message.c_str());
            }
            alids.push_back(alid);
        }

        for (size_t i=0; i<alids.size(); i++) {
            const int alid = Util::at(alids, i);
            const int alno = i;
            m_alidToAlno[alid] = alno;
            m_alnoToAlid[alno] = alid;
        }
    }

    {
        scc->setScalidToSampleno(m_scalidToSampleno);
        scc->setAlnoToAlid(m_alnoToAlid);
        std::sort(scids.begin(), scids.end(), *scc);
    }

    {
        int scno = 0;
        for (IntVec::const_iterator it = scids.begin(); it != scids.end(); it++) {
            const int scid = *it;
            m_scidToScno[scid] = scno;
            ++scno;
        }
    }

    m_ncols = 1 + SCComparator::PERSONID + m_alidToAlno.size();
    m_nrows = 1 + m_scidToScno.size();

    return;
}

void GridStuff::setup( ) {
    specifyGridColumns();
    specifyGridRows();
    specifyGridCells();

    return;
}

void GridStuff::specifyGridColumns( ) {
    const int rowno = 0;

    {
        Cell cell(SCComparator::RESULTNO, rowno);
        setText(cell, "No");
        setHeader(cell);
    }

    {
        Cell cell(SCComparator::SBARCODE, rowno);
		setText(cell, "Sample");
        setHeader(cell);
    }

    {
        Cell cell(SCComparator::CBARCODE, rowno);
        setText(cell, "Cryovial");
        setHeader(cell);
    }

    {
        Cell cell(SCComparator::PERSONID, rowno);
		setText(cell, "Source");
        setHeader(cell);
    }

    m_firstSampleColno = SCComparator::PERSONID + 1;
    m_firstSampleRowno = rowno+1;

    for (IntToIntMap::const_iterator it = m_alidToAlno.begin();
            it != m_alidToAlno.end(); it++) {
        const int alid = it->first;
        const int alno = it->second;
        const std::string name = m_samples.getAliquotName(alid);
        const int colno = alnoToColno(alno);
        Cell cell(colno, rowno);
        setText(cell, name);
        setHeader(cell);
    }

    return;
}

void GridStuff::specifyGridRows( ) {
    for (IntToIntMap::const_iterator it = m_scidToScno.begin();
            it != m_scidToScno.end(); it++) {
        const int scid = it->first;
        const int scno = it->second;
        const IntPair scalid(scid, 0);
        const int sampleno = (m_scalidToSampleno.count(scalid) == 1)
                             ? m_scalidToSampleno.find(scalid)->second
                             : -1;
        const Sample * sample = m_samples.getSample(sampleno);
        const std::string sbarcode = sample->getBarcode();
        const std::string cbarcode = sample->getCryovialBarcode();
        const std::string personid = sample->getPersonId();
        const int rowno = scnoToRowno(scno);
        {
            Cell cell(SCComparator::RESULTNO, rowno);
            setText(cell, Util::asString(scid));
            setHeader(cell);
        }
        {
            Cell cell(SCComparator::SBARCODE, rowno);
            setText(cell, sbarcode);
            setHeader(cell);
        }
        {
            Cell cell(SCComparator::CBARCODE, rowno);
            setText(cell, cbarcode);
            setHeader(cell);
        }
        {
			Cell cell(SCComparator::PERSONID, rowno);
            setText(cell, personid);
            setHeader(cell);
        }
    }

    return;
}

void GridStuff::specifyGridCells( ) {
    const int nsamples = m_samples.count();
    for (int sampleno=0; sampleno<nsamples; sampleno++) {
        const int scid = Util::at(m_samplenoToScid, sampleno);
        if (scid < 0) {
            std::string message = "";
            message += "suspect sampleno";
            message += Util::asString(sampleno);
            message += " at ";
            message += HERE;
            throw Exception(message.c_str());
        }
        const Sample * sample = m_samples.getSample(sampleno);
        const int alid = sample->getAliquotId();
        if (alid == 0) continue;
        const int scno = Util::at(m_scidToScno, scid);
        if (scno < 0) {
            std::string message = "";
            message += "suspect scid";
            message += " at ";
            message += HERE;
            throw Exception(message.c_str());
        }
        const int rowno = scnoToRowno(scno);
        const int alno = Util::at(m_alidToAlno, alid);
        if (alno < 0) {
            std::string message = "";
            message += "suspect alno";
            message += " at ";
            message += HERE;
            throw Exception(message.c_str());
        }
        const int colno = alnoToColno(alno);

        const Cell cell(colno, rowno);
        setSampleno(cell, sampleno);
        setText(cell, AnsiString(sampleno+1).c_str());
    }

    return;
}

int GridStuff::scnoToRowno( const int scno ) {
    return 1 + scno;
}

int GridStuff::alnoToColno( const int alno ) {
    return SCComparator::ALIQUOT0 + alno;
}


} // Discard

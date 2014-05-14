#pragma hdrstop

#include "DiscardLogic.h"

#pragma package(smart_init)

namespace Discard {

// Cell

Cell::Cell( const int colno, const int rowno )
    : m_colno(colno)
    , m_rowno(rowno) {
}

std::string Cell::asString( ) const {
    return Util::asString(m_colno) + "," + Util::asString(m_rowno);
}

int Cell::getColno( ) const {
    return m_colno;
}

int Cell::getRowno( ) const {
    return m_rowno;
}

bool Cell::operator== ( const Cell & that ) const {
    return (this->m_colno == that.m_colno) && (this->m_rowno == that.m_rowno);
}

bool Cell::operator< ( const Cell & that ) const {
    return (this->m_colno < that.m_colno)
           || ((this->m_colno == that.m_colno)
               && (this->m_rowno < that.m_rowno));
}


// Tube

Tube::Tube( const std::string & barcode )
    : ANamedThing(barcode) {
}


// Cryovial

Cryovial::Cryovial( const std::string & barcode )
    : ANamedThing(barcode) {
}


// Box

Box::Box( const std::string & name )
    : ANamedThing(name) {
}


// Person

std::string Person::s_fname;

Person::Person( const std::string & id )
    : ANamedThing(id) {
}

void Person::setFname( const std::string & fname ) {
    s_fname = fname;
    return;
}

const std::string & Person::getFname( ) {
    return s_fname;
}

bool Person::canSearch( ) {
    return s_fname != "";
}


// Job

Job::Job( const int id ) : ANumberedThing(id) { }


// Sample

Sample::Sample( const int id ) : ANumberedThing(id) { }

int Sample::getId( ) const {
    return getNumber();
}

void Sample::setBarcode( const std::string & barcode ) {
    setString("sbar", barcode);
    return;
}

std::string Sample::getBarcode( ) const {
    return getString("sbar");
}

void Sample::setCryovialBarcode( const std::string & barcode ) {
    setString("cbar", barcode);
    return;
}

std::string Sample::getCryovialBarcode( ) const {
    return getString("cbar");
}

void Sample::setPersonId( const std::string & id ) {
    setString("pid", id);
    return;
}

std::string Sample::getPersonId( ) const {
    return getString("pid");
}

void Sample::setAliquotId( const int id ) {
    setInt("aid", id);
    return;
}

int Sample::getAliquotId( ) const {
    return getInt("aid");
}

int Sample::getCryovialId( ) const {
    return getInt("crid");
}

void Sample::setCryovialId( const int id ) {
    setInt("crid", id);
    return;
}

int Sample::getCryovialStoreStatus( ) const {
    return getInt("crst");
}

void Sample::setCryovialStoreStatus( const int status ) {
    setInt("crst", status);
    return;
}

int Sample::getJobno( ) const {
    return getInt("jobno");
}

void Sample::setJobno( const int no ) {
    setInt("jobno", no);
    return;
}

int Sample::getNoteFlag( ) const {
    return getInt("noteflag");
}

void Sample::setNoteFlag( const int count ) {
    setInt("noteflag", count);
    return;
}

int Sample::getCryovialStoreId( ) const {
    return getInt("csid");
}

void Sample::setCryovialStoreId( const int id ) {
    setInt("csid", id);
    return;
}

// FIXME 6 begin
void Sample::setNote( const std::string & note ) {
    setString("note", note);
    return;
}

std::string Sample::getNote( ) const {
    return getString("note");
}
// FIXME 6 end


// CryovialStatus

CryovialStatus::CryovialStatus( ) : m_currentIndex(-1) , m_nextIndex(-1) { }

void CryovialStatus::populate( ) {
	set(Cryovial::STORED,   	"Stored");
	set(Cryovial::MARKED,   	"Marked");
	set(Cryovial::REMOVED,     	"Removed");
	set(Cryovial::DESTROYED,   	"Destroyed");
	set(Cryovial::NINETYNINED, 	"99d");
    return;
}

void CryovialStatus::close( ) {
    m_ids.clear();
    m_names.clear();

	m_ids.push_back(Cryovial::STORED);
	m_ids.push_back(Cryovial::MARKED);
	m_ids.push_back(Cryovial::REMOVED);
	m_ids.push_back(Cryovial::DESTROYED);

    m_nids = m_ids.size();

    for (int i=0; i<m_nids; i++) {
        const int id = m_ids[i];
        const std::string name = m_name[id];
        m_names.push_back(m_name[id]);
        m_index[id] = i;
    }

    return;
}

int CryovialStatus::getCurrentId( ) const {
    int currentId = -1;
    if ((m_currentIndex >= 0) && (m_currentIndex < m_nids)) {
        currentId = m_ids[m_currentIndex];
    }
    return currentId;
}

bool CryovialStatus::setCurrentId( const int id ) {
    bool ok = false;

    do {
        if (m_index.count(id) != 1) break;
        m_currentIndex = m_index[id];
        ok = true;

    } while (false);

    return ok;
}

bool CryovialStatus::setCurrentId( const std::string & name ) {
    bool ok = false;

    do {
        const int id = getId(name);
        if (id == 0) break;
        ok = setCurrentId(id);

    } while (false);

    return ok;
}

int CryovialStatus::getNextId( ) const {
    int nextId = -1;
    if ((m_nextIndex >= 0) && (m_nextIndex < m_nids)) {
        nextId = m_ids[m_nextIndex];
    }
    return nextId;
}

bool CryovialStatus::setNextId( const int id ) {
    bool ok = false;

    do {
        if (m_index.count(id) != 1) break;
        m_nextIndex = m_index[id];
        ok = true;

    } while (false);

    return ok;
}

bool CryovialStatus::setNextId( const std::string & name ) {
    bool ok = false;

    do {
        const int id = getId(name);
        if (id == 0) break;
        ok = setNextId(id);

    } while (false);

    return ok;
}


// Cells

Cells::Cells( ) { }

void Cells::reset( ) {
    m_isheader.clear();
    m_sampleno.clear();

    return;
}

int Cells::setHeader( const Cell & cell ) {
    int nchanged = 0;
    if (isHeader(cell) == 0) {
        m_isheader.insert(cell);
        ++nchanged;
    }
    return nchanged;
}

int Cells::clearHeader( const Cell & cell ) {
    int nchanged = 0;
    if (isHeader(cell) == 1) {
        m_isheader.erase(cell);
        ++nchanged;
    }
    return nchanged;
}

int Cells::toggleHeader( const Cell & cell ) {
    int nchanged = 0;
    do {
        const int myIsHeader = isHeader(cell);
        if (myIsHeader == -1) break;
        if (myIsHeader == 0) {
            m_isheader.insert(cell);
            ++nchanged;
            break;
        }
        if (myIsHeader == 1) {
            m_isheader.erase(cell);
            ++nchanged;
            break;
        }

    } while (false);

    return nchanged;
}

int Cells::isHeader( const Cell & cell ) {
    const int n = m_isheader.count(cell);
    return ((n == 0) || (n == 1)) ? n : -1;
}

int Cells::getNHeader( ) {
    return m_isheader.size();
}

std::set<Cell>::const_iterator Cells::getHeaderBegin( ) const {
    return m_isheader.begin();
}

std::set<Cell>::const_iterator Cells::getHeaderEnd( ) const {
    return m_isheader.end();
}

void Cells::setSampleno( const Cell & cell, const int sampleno ) {
    m_sampleno[cell] = sampleno;
    return;
}

int Cells::getSampleno( const Cell & cell ) const {
    int sampleno = -1;

    do {
        std::map<Cell,int>::const_iterator it = m_sampleno.find(cell);
        if (it == m_sampleno.end()) break;
        sampleno = it->second;

    } while (false);

    return sampleno;
}


} // Discard


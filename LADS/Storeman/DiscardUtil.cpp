#pragma hdrstop

#include <algorithm>
#include <iomanip>
#include <sstream>
#include <stdio.h>

#include "DiscardUtil.h"

#pragma package(smart_init)

namespace Discard {

// MyRosetta

const int MyRosetta::NO_IVALUE = -1;
const std::string MyRosetta::NO_SVALUE = "";

MyRosetta::MyRosetta( ) {
}

int
MyRosetta::getInt( const std::string & name ) const {
    const StringToIntMap::const_iterator it = m_ivalues.find(name);
    return (it == m_ivalues.end()) ? NO_IVALUE : it->second;
}

void
MyRosetta::setInt( const std::string & name, const int value ) {
    m_svalues.erase(name);
    m_ivalues[name] = value;
    return;
}

const std::string &
MyRosetta::getString( const std::string & name ) const {
    const StringToStringMap::const_iterator it = m_svalues.find(name);
    return (it == m_svalues.end()) ? NO_SVALUE : it->second;
}

void
MyRosetta::setString( const std::string & name, const std::string & value ) {
    m_ivalues.erase(name);
    m_svalues[name] = value;
    return;
}


// ANamedThing

ANamedThing::ANamedThing( ) {
}

ANamedThing::ANamedThing( const std::string & name ) {
    this->setString("name", name);
}

std::string
ANamedThing::getName( ) const {
    return this->getString("name");
}


// ANumberedThing

ANumberedThing::ANumberedThing( ) {
}

ANumberedThing::ANumberedThing( const int number ) {
    this->setInt("number", number);
}

int
ANumberedThing::getNumber( ) const {
    return this->getInt("number");
}

// NameTable

NameTable::NameTable( ) {
}

void
NameTable::clear( ) {
    m_id.clear();
    m_name.clear();
    return;
}

void
NameTable::init( ) {
    clear();
    populate();
    close();
}

void
NameTable::populate( ) {
    return;
}

void
NameTable::close( ) {
    return;
}

bool
NameTable::set( const int id, const std::string & name ) {
    bool isReplacing = m_name.count(id) == 1;
    m_name[id] = name;
    m_id[name] = id;
    return isReplacing;
}

std::string
NameTable::getName( const int id ) const {
    std::string name = "";

    do {
        IntToStringMap::const_iterator it = m_name.find(id);
        if (it == m_name.end()) break;
        name = it->second;

    } while (false);

    return name;
}

int
NameTable::getId( const std::string & name ) const {
    int id = 0;

    do {
        StringToIntMap::const_iterator it = m_id.find(name);
        if (it == m_id.end()) break;
        id = it->second;

    } while (false);

    return id;
}

// NumberTable

NumberTable::NumberTable( ) {
}

void
NumberTable::clear( ) {
    m_id.clear();
    m_number.clear();
    return;
}

void
NumberTable::init( ) {
    clear();
    populate();
    close();
}

void
NumberTable::populate( ) {
    return;
}

void
NumberTable::close( ) {
    return;
}

bool
NumberTable::set( const int id, const int number ) {
    bool isReplacing = m_number.count(id) == 1;
    m_number[id] = number;
    m_id[number] = id;
    return isReplacing;
}

int
NumberTable::getNumber( const int id ) const {
    int number = -1;

    do {
        IntToIntMap::const_iterator it = m_number.find(id);
        if (it == m_number.end()) break;
        number = it->second;

    } while (false);

    return number;
}

int
NumberTable::getId( const int number ) const {
    int id = 0;

    do {
        IntToIntMap::const_iterator it = m_id.find(number);
        if (it == m_id.end()) break;
        id = it->second;

    } while (false);

    return id;
}

// OrderedNameTable

OrderedNameTable::OrderedNameTable( ) {
}

void
OrderedNameTable::close( ) {
    m_ids.clear();
    m_names.clear();

    for (StringToIntMap::const_iterator it = m_id.begin();
            it != m_id.end(); it++) {
        m_names.push_back(it->first);
    }
    std::sort(m_names.begin(), m_names.end());

    const int nnames = m_names.size();
    for (int i=0; i<nnames; i++) {
        const std::string name = m_names[i];
        const int id = m_id[name];
        m_ids.push_back(id);
        m_index[id] = i;
    }

    m_nids = m_ids.size();

    return;
}

IntVec
OrderedNameTable::getIds( ) const {
    return m_ids;
}

StringVec
OrderedNameTable::getNames( ) const {
    return m_names;
}

// OrderedNumberTable
/*
OrderedNumberTable::OrderedNumberTable( ) {
}

void
OrderedNumberTable::close( ) {
	m_ids.clear();
	m_numbers.clear();

	for (IntToIntMap::const_iterator it = m_id.begin();
			it != m_id.end(); it++) {
		m_numbers.push_back(it->first);
	}
	std::sort(m_numbers.begin(), m_numbers.end());

	const int nnumbers = m_numbers.size();
	for (int i=0; i<nnumbers; i++) {
		const int number = m_numbers[i];
		const int id = m_id[number];
		m_ids.push_back(id);
		m_index[id] = i;
	}

	m_nids = m_ids.size();

	return;
}

IntVec
OrderedNumberTable::getIds( ) const {
	return m_ids;
}

IntVec
OrderedNumberTable::getNumbers( ) const {
	return m_numbers;
}
*/

// MyException

MyException::MyException( const std::string & text )
	: std::runtime_error(text) {
}


// Util

namespace Util {

std::string
quote( const std::string & s, const std::string & q1, const std::string & q2 ) {
	return q1 + s + (q2 == "" ? q1 : q2);
}

std::string
bracket( const std::string & s ) {
    return quote(s, "(", ")");
}

StringVec
split( const std::string & sep, const std::string & in ) {
    StringVec out;

    size_t lenstr = in.length();
    size_t index1 = 0;

    while (index1 <= lenstr) {
        size_t index2 = in.find(sep, index1);
        if (index2 == std::string::npos) index2 = in.length();
        out.push_back(in.substr(index1, index2-index1));
        index1 = index2 + sep.length();
    }

    return out;
}

std::string
asString( const int n ) {
    char buff[256];
    std::sprintf(buff, "%d", n);
    return std::string(buff);
}

std::string
substitute( const std::string & in,
            const std::string & from, const std::string & to ) {
    std::string out = in;
    const size_t fromlen = from.size();
    const size_t tolen = to.size();
    size_t icurrent = 0;
    size_t ifound = 0;
    while ((ifound = out.find(from, icurrent)) != std::string::npos) {
        out.replace(ifound, fromlen, to);
        icurrent = ifound + tolen;
    }
    return out;
}

std::string
trim( const std::string & text ) {
    return ltrim(rtrim(text));
}

std::string
ltrim( const std::string & text ) {
    const size_t index = text.find_first_not_of(" ");
    return (index == std::string::npos) ? text : text.substr(index);
}

std::string
rtrim( const std::string & text ) {
    const size_t index = text.find_last_not_of(" ");
    return (index == std::string::npos) ? text : text.substr(0, 1+index);
}

std::string
dtg( time_t utime ) {
    char buff[256];
    if (utime == -1) utime = time(0);
    strftime(buff, sizeof(buff)-1, "%Y%m%dT%H%M%S", gmtime(&utime));
    return std::string(buff);
}

std::string
ddmmyyyy( time_t utime ) {
    char buff[256];
    if (utime == -1) utime = time(0);
    strftime(buff, sizeof(buff)-1, "%d/%m/%Y", gmtime(&utime));
    return std::string(buff);
}

int
sigma( const int n ) {
    return (n < 0)
           ? -1
           : ((n > 0)
              ? 1
              : 0)
           ;
}

int
at( const IntToIntMap & map, const int name, const int defaultValue ) {
    return (map.count(name) == 1)
           ? map.find(name)->second
           : defaultValue;
}

int
at( const IntVec & vec, const int index, const int defaultValue ) {
    return ((index >= 0) && (index < (int) (vec.size())))
           ? vec[index]
           : defaultValue;
}

std::string
at( const IntToStringMap & map, const int name,
    const std::string & defaultValue ) {
    return (map.count(name) == 1)
           ? map.find(name)->second
           : defaultValue;
}

std::string
at( const StringVec & vec, const int index, const std::string & defaultValue ) {
    return ((index >= 0) && (index < (int) (vec.size())))
           ? vec[index]
           : defaultValue;
}

unsigned int
blend( const unsigned int i1, const size_t w1,
       const unsigned int i2, const size_t w2 ) {
    return (i1 * w1 + i2 * w2) / (w1 + w2);
}

std::string
pad( const std::string & text, const size_t width, const char padding ) {
    std::ostringstream ss;
    ss << std::setw(width) << std::setfill(padding) << text;
    return ss.str();
}

bool
splat( const std::string & path, const std::string & gob,
       const std::string mode ) {
    bool st = false;

    FILE * fp = 0;

    do {
        fp = fopen(path.c_str(), mode.c_str());

        if (fp == 0) break;

        if (fwrite(gob.c_str(), gob.size(), 1, fp) != 1) break;

        st = true;

    } while (false);

    if (fp != 0) {
        (void) fclose(fp);
        fp = 0;
    }

    return st;
}


} // Util

} // Discard

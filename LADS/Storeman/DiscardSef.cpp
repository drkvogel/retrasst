#pragma hdrstop

#include <algorithm>

#include "DiscardSef.h"
#include "DiscardUtil.h"

#pragma package(smart_init)


namespace Discard {

// Sef

Sef::Sef( const std::string & studyName, const std::string & studyCode,
          const std::string & description, const std::string & reason,
          const std::string & owner,
          const time_t created, const std::string username,
          const std::string & key )
    : m_studyName(studyName)
    , m_studyCode(studyCode)
    , m_reason(reason)
    , m_description(description)
    , m_owner(owner)
    , m_created(created)
    , m_username(username)
    , m_key(key)
    , m_isOpen(true)
    , m_filename("")
    , m_method("") {
}

Sef::Sef( ) {
    std::string error = "";
    error += "attempt to call no-arg constructor for Sef";
    error += " at ";
    error += HERE;
    throw MyException(error);
}

void
Sef::addSample( const std::string & sbarcode, const std::string & stype,
                const std::string & cbarcode ) {
    do {
        if (! m_isOpen) break;

        m_sbarcodes.push_back(sbarcode);
        m_stypes.push_back(stype);
        m_cbarcodes.push_back(cbarcode);

    } while (false);

    return;
}

void
Sef::close( const int serial, const std::string & method ) {
    do {
        if (! m_isOpen) break;

        m_ddmmyyyy = Util::ddmmyyyy();

        m_serial = serial;
        m_serialString = Util::pad(Util::asString(m_serial), 10, '0');

        m_method = method;

        m_filename = "";
        {
            StringVec words;
            words.push_back("AUTO_SEF");
            words.push_back(m_studyCode);
            words.push_back(m_serialString);
            words.push_back(m_key);
            m_reference = Util::join("-", words.begin(), words.end());
            m_filename = m_reference + ".txt";
        }

        m_isOpen = false;

    } while (false);

    return;
}

std::string Sef::asString() const {
    std::string text = "";

    do {
        if (m_isOpen) break;

        StringVec lines;

        lines.push_back("SAMPLE EVENT FORM (SEF)");
        lines.push_back(m_studyName + " " + Util::bracket(m_studyCode));
        lines.push_back("");
        lines.push_back("");
        lines.push_back("Study: " + m_studyName);
        lines.push_back("");
        lines.push_back("Reference Number: " + m_reference);
        lines.push_back("");
        lines.push_back("Date: " + Util::ddmmyyyy(m_created));
        lines.push_back("");

        bool isCryovialBarcodeDifferent = false;
        const int nsamples = m_sbarcodes.size();
        for (int sampleno=0; sampleno<nsamples; sampleno++) {
            if (! isCryovialBarcodeDifferent) {
                if (m_sbarcodes[sampleno] != m_cbarcodes[sampleno]) {
                    isCryovialBarcodeDifferent = true;
                }
            }
            lines.push_back("Sample: "+m_sbarcodes[sampleno]+" "+Util::bracket(m_stypes[sampleno]));
            lines.push_back("");
        }
        lines.push_back("");
        lines.push_back("Event Details");
        lines.push_back(m_description);
        lines.push_back("");
        lines.push_back("");
        lines.push_back("Missing Vacutainers");
        lines.push_back("");
        lines.push_back("The following vacutainers were missing: "+std::string("N/A"));
        lines.push_back("");
        lines.push_back("");
        lines.push_back("Missing Results");
        lines.push_back("");
        lines.push_back("The following test results will be missing: "+std::string("N/A"));
        lines.push_back("");
        lines.push_back("Reason: "+std::string("N/A"));
        lines.push_back("");
        lines.push_back("");
        lines.push_back("Sample Disposal");
        lines.push_back("");
        lines.push_back("Reason for Disposal: "+m_reason);
        lines.push_back("");

        if (isCryovialBarcodeDifferent) {
            for (int sampleno=0; sampleno<nsamples; sampleno++) {
                lines.push_back("Barcode ID: " + m_cbarcodes[sampleno]);
                lines.push_back("");
            }
        } else {
            lines.push_back("Barcode IDs are the same as sample IDs above.");
            lines.push_back("");
        }
        lines.push_back("");

        lines.push_back("Date of Disposal: "+m_ddmmyyyy);
        lines.push_back("");
        lines.push_back("Method of Disposal: "+m_method);
        lines.push_back("");
        lines.push_back("Initials: "+m_username);
        lines.push_back("");
        lines.push_back("");
        lines.push_back("Other");
        lines.push_back("");
        lines.push_back("");

        text += " serial: " + m_serialString;
        text += " key: " + m_key;
        text += " date: " + m_ddmmyyyy;
        text += " filename: " + m_filename;
        text += " samples: "+Util::join(",", m_sbarcodes.begin(), m_sbarcodes.end());

        lines.push_back("");

        text = Util::join("\n", lines.begin(), lines.end());

    } while (false);

    return text;
}

const std::string & Sef::getFilename( ) const { return m_filename; }


// SefBatch

SefBatch::SefBatch( const Context * context ) : m_context(context) { }

void SefBatch::addSample( const Sample * s ) {
    do {
        if (s == 0) break;

        const std::string personid = s->getPersonId();
        const std::string key = (personid == "") ? s->getCryovialBarcode() : personid;

        const int jobno = s->getJobno();

        const Context * c = m_context;
        if (c == 0) break;

        const std::string description = c->getJobDescription(jobno);
        const std::string reason = c->getJobReason(jobno);
        Sef sef(c->getProjectName(), c->getStudyCode(), description, reason,
                c->getJobOwner(jobno), c->getJobCreationUtime(jobno),
                c->getCurrentUserName(), key);

        if (m_sefs.count(key) == 0) {
            m_sefs.insert(std::make_pair(key, sef));
        }

        if (m_sefs.count(key) != 1) {
            std::string error = "";
            error += "not one sef for ";
            error += key;
            error += " at ";
            error += HERE;
            throw MyException(error);
        }

        const std::string sbarcode = s->getBarcode();
        const std::string stype = c->getAliquotName(s->getAliquotId());
        const std::string cbarcode = s->getCryovialBarcode();
        m_sefs[key].addSample(sbarcode, stype, cbarcode);

    } while (false);

    return;
}

size_t SefBatch::size( ) const { return m_sefs.size(); }

void SefBatch::publish( int studySerial ) {
    for (StringToSefMap::iterator it = m_sefs.begin();
            it != m_sefs.end(); it++) {
        const std::string key = it->first;
        Sef & sef = it->second;
        sef.close(studySerial++, m_context->getMethod());
    }

    for (StringToSefMap::const_iterator it = m_sefs.begin();
            it != m_sefs.end(); it++) {
        const std::string key = it->first;
        Sef sef = it->second;
        const std::string filename = sef.getFilename();
        const std::string filepath = filename; // FIXME 81
        const std::string content = sef.asString();
        if (! Util::splat(filepath, content)) {
            std::string error = "";
            error += "failed to write file ";
            error += filepath;
            error += " at ";
            error += HERE;
            throw MyException(error);
        }
        if (! m_context->addAuditEntry(content)) {
            std::string error = "";
            error += "failed to add audit entry";
            error += " at ";
            error += HERE;
            throw MyException(error);
        }
    }

    return;
}

} // Discard


#ifndef DiscardSefH
#define DiscardSefH

#include <string>
#include <map>

#include "DiscardContext.h"
#include "DiscardLogic.h"


namespace Discard
{

class Sef
{
public:
	Sef( const std::string & studyName, const std::string & studyCode,
        const std::string & reason, const std::string & description,
        const std::string & owner,
        const time_t created, const std::string username,
        const std::string & key );
    Sef( );
    void addSample( const std::string & sbarcode, const std::string & stype,
        const std::string & cbarcode );
    void close( const int serial, const std::string & metho );
    std::string asString( ) const;
    const std::string & getFilename( ) const;
protected:
private:
    std::string m_studyName;
    std::string m_studyCode;
    std::string m_reason;
    std::string m_description;
    std::string m_owner;
    time_t m_created;
    unsigned int m_serial;
    std::string m_serialString;
    std::string m_username;
    std::string m_key;
    StringVec m_sbarcodes;
    StringVec m_stypes;
    StringVec m_cbarcodes;
    bool m_isOpen;
    std::string m_reference;
    std::string m_filename;
    std::string m_ddmmyyyy;
    std::string m_method;
};

class SefBatch
{
public:
    typedef std::map<std::string,Sef> StringToSefMap;
    SefBatch( const Context * context );
    void addSample( const Sample * s );
    size_t size( ) const;
    void publish( int studySerial );
protected:
private:
    const Context * m_context;
    StringToSefMap m_sefs;
};


} // Discard


#endif


#ifndef SourceH
#define SourceH

#include <XMLDoc.hpp>
#include <XSLProd.hpp>

namespace xslt
{

class XMLSource
{
public:
    virtual ~XMLSource() {}
    virtual void bind( TXMLDocument* xml ) = 0;
};

String transform( XMLSource& xmlSource, XMLSource& xslSource );

class XMLSourceFile : public XMLSource
{
public:
    XMLSourceFile( const String& fileName );
    void bind( TXMLDocument* xml );

private:
    String m_fileName;
};

class XMLSourceString : public XMLSource
{
public:
    XMLSourceString( const String& xmlData );
    void bind( TXMLDocument* xml );

private:
    String m_xml;
};

};

#endif


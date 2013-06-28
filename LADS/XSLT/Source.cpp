#include "Hidden_XSLTForm.h"
#include "Source.h"

#include <memory>

namespace xslt
{

class XSLTrans
{
public:
    XSLTrans( XMLSource& xmlSource, XMLSource& xslSource )
        : m_form( new THiddenXSLTForm(0) )
    {
        m_form->setXmlSource(xmlSource);
        m_form->setXslSource(xslSource);
    }
    
    String doTransform() { return m_form->transform(); }

    struct InitCom
    {
        InitCom()  { CoInitialize(0);  }
        ~InitCom() { CoUninitialize(); }
    };

private:
    static InitCom initCom;
    
    std::auto_ptr<THiddenXSLTForm> m_form;
};

XSLTrans::InitCom initCom;

String transform( XMLSource& xmlSource, XMLSource& xslSource )
{
    XSLTrans t( xmlSource, xslSource );
    return t.doTransform();
}

XMLSourceFile::XMLSourceFile( const String& fileName )
    : m_fileName( fileName )
{}

void XMLSourceFile::bind( TXMLDocument* xml )
{
    xml->Active = false;
    xml->FileName = m_fileName;
    xml->Active = true;
}

XMLSourceString::XMLSourceString( const String& xmlData )
    : m_xml(xmlData)
{}

void XMLSourceString::bind( TXMLDocument* xml )
{
    xml->Active = false;
    xml->LoadFromXML( m_xml );
    xml->Active = true;
}

}


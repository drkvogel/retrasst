//---------------------------------------------------------------------------

#ifndef Hidden_XSLTFormH
#define Hidden_XSLTFormH
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
#include <msxmldom.hpp>
#include <XMLDoc.hpp>
#include <xmldom.hpp>
#include <XMLIntf.hpp>
#include <XSLProd.hpp>

#include "Source.h"

//---------------------------------------------------------------------------
class THiddenXSLTForm : public TForm
{
__published:	// IDE-managed Components
    TXSLPageProducer *transformer;
    TXMLDocument *xml;
private:	// User declarations
public:		// User declarations
    __fastcall THiddenXSLTForm(TComponent* Owner);
    void setXmlSource( xslt::XMLSource& xmlSource );
    void setXslSource( xslt::XMLSource& xslSource );
    String transform();
};
#endif

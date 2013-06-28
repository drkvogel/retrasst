//---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop

#include "Hidden_XSLTForm.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"
//---------------------------------------------------------------------------
__fastcall THiddenXSLTForm::THiddenXSLTForm(TComponent* Owner)
    : TForm(Owner)
{
}
//---------------------------------------------------------------------------
void THiddenXSLTForm::setXmlSource( xslt::XMLSource& xmlSource )
{
    xmlSource.bind(xml);
}

void THiddenXSLTForm::setXslSource( xslt::XMLSource& xslSource )
{
    xslSource.bind(transformer);
}

String THiddenXSLTForm::transform()
{
    transformer->XMLData = xml;
    transformer->Active = true;
    return transformer->Content();
}
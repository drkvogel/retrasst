//---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop

#include "Source.h"
#include <iostream>

#pragma argsused
int main(int argc, char* argv[])
{
    xslt::XMLSourceString xml("<a/>");
    xslt::XMLSourceString xsl(
        "<?xml version=\"1.0\" ?>"
        "<xsl:stylesheet version=\"1.0\" "
        "xmlns:xsl=\"http://www.w3.org/1999/XSL/Transform\">"
        "<xsl:output method=\"text\"/>"
        "<xsl:template match=\"/\">OK!</xsl:template>"
        "</xsl:stylesheet>"
    );
    String output = xslt::transform( xml, xsl );
    std::cout << output.c_str() << std::endl;
    std::cout << "[Enter] to quit..." << std::endl;
    char c;
    std::cin >> std::noskipws >> c;
    return 0;
}
//---------------------------------------------------------------------------

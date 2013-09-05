#include <vcl.h>
#include <windows.h>

#pragma hdrstop
#pragma argsused

#include <tchar.h>
#include <stdio.h>

#include <boost/shared_ptr.hpp>
#include <exception>
#include <fstream>
#include <iostream>
#include <stdexcept>
#include <string>

#include <System.hpp>

#include "MSXML2_TLB.h"

std::string USAGE =
"2 arguments are required, both pathnames, the 1st for the xml, "
"the 2nd for xsl.\n"
;

bool fileExists( const AnsiString& filename )
{
    std::ifstream file( filename.c_str() );
    return file;
}

void printError( const AnsiString& msg )
{
    std::cerr << msg.c_str() << "\n";
}

void require( bool condition, const std::string& errorMsg )
{
    if ( ! condition )
    {
        throw std::runtime_error( errorMsg.c_str() );
    }
}

void freeString( BSTR str )
{
    if ( str )
        SysFreeString( str );
}

void release( IErrorInfo* ptr )
{
    if (ptr)
        ptr->Release();
}

void load( TCOMIXMLDOMDocument3& xml, _TCHAR* filename )
{
    BSTR filenameCopy = SysAllocString(filename);

    require( filenameCopy, "SysAllocString returned NULL" );

    boost::shared_ptr<void> freeStringOnExit( filenameCopy, freeString );

    VARIANT_BOOL ok = VARIANT_FALSE;
    VARIANT v;
    v.vt = VT_BSTR;
    v.bstrVal = filenameCopy;

    xml->load( v, &ok );

    if ( ok != VARIANT_TRUE )
    {
        Msxml2_tlb::IXMLDOMParseErrorPtr parserError = xml->parseError;
        WideString parserErrorMsg( parserError->reason );
        throw Exception( parserErrorMsg );
    }
}

int _tmain(int argc, _TCHAR* argv[])
{
    try
    {
        require( argc == 3, USAGE );
        require( fileExists( argv[1] ), "File not found" );
        require( fileExists( argv[2] ), "File not found" );

        TCOMIXMLDOMDocument3 xml = Msxml2_tlb::CoDOMDocument60::Create();
        TCOMIXMLDOMDocument3 xsl = Msxml2_tlb::CoDOMDocument60::Create();
        xsl->resolveExternals = VARIANT_TRUE;

        load( xml, argv[1] );
        load( xsl, argv[2] );

        BSTR result = NULL;
        Msxml2_tlb::IXMLDOMNodePtr pXSLDOM = xsl->documentElement;

        HRESULT hr = xml->transformNode( pXSLDOM, &result );

        if (FAILED(hr))
        {
            WideString errMsg = L"Failure code obtained from transformNode. ";

            // Try to get extended error info...
            IErrorInfo* errorInfo = NULL;

            if ( SUCCEEDED( GetErrorInfo( 0, &errorInfo ) ) )
            {
                boost::shared_ptr<void> releaseOnExit( errorInfo, release );

                BSTR reason = NULL;

                if ( SUCCEEDED( errorInfo->GetDescription( &reason ) ) )
                {
                    boost::shared_ptr<void> freeStringOnExit( reason, freeString );

                    if ( reason )
                    {
                        errMsg += WideString(reason);
                    }
                }
            }

            throw Exception( UnicodeString( errMsg ) );
        }

        boost::shared_ptr<void> freeStringOnExit( result, freeString );

        WideString ws(result);
        std::wcout << (wchar_t*)ws;

        return 0;
    }
    catch( Exception& e )
    {
        printError( e.Message );
    }
    catch( const std::exception& e )
    {
        printError( e.what() );
    }
    catch( ... )
    {
        printError( "Unspecified exception" );
    }
	return 1;
}


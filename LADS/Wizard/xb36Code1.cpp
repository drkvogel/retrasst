//---------------------------------------------------------------------------

#error	This is an old version of xb36 - do not use

#include <vcl.h>
#pragma hdrstop

#include "xb36Code1.h"
//#include "modLog.h"

//---------------------------------------------------------------------------

#pragma package(smart_init)
static const int LEN_ORDER = 36;
static const char order[LEN_ORDER] =
    "0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZ";
CharIntMap xb36Code::mapEncode;
IntCharMap xb36Code::mapDecode;
//---------------------------------------------------------------------------
xb36Code::xb36Code()
{
}
//---------------------------------------------------------------------------
__fastcall xb36Code::~xb36Code()
{
}
//---------------------------------------------------------------------------
void xb36Code::fillMap()
{
    for ( int pos=0; pos<LEN_ORDER; pos++ )
    {
        mapEncode.insert(CharIntMap::value_type(order[pos],pos));
        mapDecode.insert(IntCharMap::value_type(pos,order[pos]));
    }
}
//---------------------------------------------------------------------------

unsigned __int64 xb36Code::xb36Encode( AnsiString alpha )
{
    unsigned __int64 code = 0;
    CharIntMap::iterator itChars;

    for ( int pos=1; pos<=alpha.Length(); pos++ )
    {
        itChars = mapEncode.find( alpha[pos] );
        if ( itChars == mapEncode.end() )
        {
            AnsiString alphaPos = alpha[pos];
            AnsiString strMessage =
                 "Character <" + alphaPos + "> of Read code <" + alpha +
                "> not found in map";
            Application->MessageBox(
                strMessage.c_str(),
                "xb36Code", MB_OK );
            return code;
        }
        code = code * 36;
        code += itChars->second;
    }
    return code;
}
//---------------------------------------------------------------------------
AnsiString xb36Code::xb36Decode( unsigned __int64 code, int len )
{
    AnsiString output = "";
    IntCharMap::iterator itChars;
    unsigned __int64 intCode = code;

    for ( int pos=0; pos<len; pos++ )
    {
        int intOneChar = intCode % 36;
        itChars = mapDecode.find( intOneChar );
        if ( itChars == mapDecode.end() )
        {
            AnsiString strChar = IntToStr( intOneChar );
            AnsiString strMessage = "Code <" + strChar + "> not found in map";
            Application->MessageBox(
                strMessage.c_str(),
                "xb36Code Error", MB_OK );
            return "";
        }
        AnsiString strChar = itChars->second;
        output = strChar + output;
        intCode = intCode / 36;
    }
    if ( intCode != 0 )
    {
        Application->MessageBox(
            "Problem with decoding during value checking",
            "xb36Code Error", MB_OK );
    }
    return output;
}
//---------------------------------------------------------------------------


//---------------------------------------------------------------------------

#ifndef xuni64H
#define xuni64H

#include <wchar.h>

//---------------------------------------------------------------------------

String xuniGetLocaleId( LCID locale );
int xuniInitLocale( String localeID = "" );

//---------------------------------------------------------------------------

int multibyte_to_uni64(unsigned const char* szMBCSIn, unsigned char** szUCB64Out);
int multibyte_to_uni64(unsigned const char* szMBCSIn, unsigned char** szUCB64Out);
int multibyte_to_wide(unsigned const char* szMBCSIn, wchar_t** lpUCS2Out);
int wide_to_uni64(const wchar_t* lpUCS2In, unsigned char** szUCB64Out);
int uni64_to_multibyte(unsigned char* szUCB64In, unsigned char** szMBCSOut);
int uni64_to_wide(unsigned char* szUCB64In, wchar_t** lpUCS2Out);
int wide_to_multibyte(const wchar_t* lpUCS2In, unsigned char** szMBCSOut);

#ifdef _Windows
int wide_to_utf8(const wchar_t* lpWideIn, unsigned char** szUTF8Out);
#endif

//---------------------------------------------------------------------------

#endif


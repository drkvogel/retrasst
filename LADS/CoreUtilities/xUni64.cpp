/*
 *	xUni64 - converts between multi-byte, Unicode and base64
 *
 *	16 August 2005 - incorporated Alex's routines to check and set locale
 *  23 September - modified locale routines to allow other than default
 */

#ifdef _Windows
#ifdef __BORLANDC__

#include <vcl.h>
#pragma hdrstop

#else

#include <windows.h>

#endif
#endif

#include "xuni64.h"

#ifdef __STDC__
#include "xb64.c"
#else
#include "xb64.h"
#endif

#ifdef __BORLANDC__
#pragma package(smart_init)
#endif

#include <stdlib.h>
#include <wchar.h>

//---------------------------------------------------------------------------

/*
   form a string from the given locale suitable for passing to setlocale();
   format is similar to "English_United Kingdom.1252"
*/
String xuniGetLocaleId( LCID locale )
{
	char buf[128];
				   // GET LANGUAGE NAME (IN ENGLISH)
	GetLocaleInfo( locale, LOCALE_SENGLANGUAGE, buf, 127 );
	String result = buf;
	memset(buf, 0, sizeof(buf));
				   // GET REGION/COUNTRY NAME (IN ENGLISH)
	GetLocaleInfo( locale, LOCALE_SENGCOUNTRY, buf, 127 );
	result += "_";
	result += buf;
	memset(buf, 0, sizeof(buf));
					   // GET CODE PAGE
	if( ( GetLocaleInfo( locale, LOCALE_IDEFAULTANSICODEPAGE, buf, 127 )
		|| GetLocaleInfo( locale, LOCALE_IDEFAULTCODEPAGE, buf, 127) )
			&&  *buf )
		{
		result += ".";
		result += buf;
		}
	return( result );
}

/*
	initialize the locale for multibyte conversion; default to the current
   (user) locale; returns 1 on success and zero on failure (locale has not
   been changed)
*/
int xuniInitLocale( String localeID )
{
	if( localeID.IsEmpty() )
		localeID = xuniGetLocaleId( LOCALE_USER_DEFAULT );

	if( NULL == setlocale( LC_ALL, localeID.c_str() ) )
		return 0;
	else
		return 1;
}

//---------------------------------------------------------------------------

/* return number of bytes, excluding null terminator, of base64 string output,
   or -1 if the conversion failed */

int multibyte_to_uni64(unsigned const char* szMBCSIn, unsigned char** szUCB64Out)
{
	wchar_t* wszTemp = NULL;
	int result = multibyte_to_wide(szMBCSIn, &wszTemp);

	if (result > 0) {
		result = wide_to_uni64(wszTemp, szUCB64Out);
	}

	if (wszTemp) free(wszTemp);

	return result;
}

/* return number of characters converted, or -1 if an invalid
   multibyte character is encountered */

int multibyte_to_wide(unsigned const char* szMBCSIn, wchar_t** lpWideOut) {
	int cchWideChar;
	int result;

	/* to avoid mbstowcs crashing on NULL szMBCSIn */
	if (NULL == szMBCSIn) return -1;

	cchWideChar = mbstowcs(NULL, szMBCSIn, 0);

	if (cchWideChar < 0) return cchWideChar;

	*lpWideOut = (wchar_t *)malloc((cchWideChar + 1) * sizeof(wchar_t));

	mbstowcs(*lpWideOut, szMBCSIn, cchWideChar);

	(*lpWideOut)[cchWideChar] = L'\0';

	return cchWideChar;
}

/* return number of bytes, excluding null terminator, of base64 string output,
   or -1 if the conversion failed */

int wide_to_uni64(const wchar_t* pwszWideIn, unsigned char** szUCB64Out)
{
	int result;
	int cchWideChar;
	int cchUCB64Out;
	unsigned char *temp;
	unsigned char *pUCS2;
	wchar_t* pwszWideInTemp;
	int i;

	if (NULL == pwszWideIn) return -1;

	cchWideChar = wcslen(pwszWideIn);

	if (2 == sizeof(wchar_t)) {
		pUCS2 = (unsigned char *)(pwszWideIn);
	} else if (4 == sizeof(wchar_t)) {
		pwszWideInTemp = (wchar_t *)pwszWideIn;

		temp = (char *)malloc((2 * (cchWideChar +1)) * sizeof(char));

		pUCS2 = temp;

		for (i=0; i<cchWideChar; i++) {
			*temp++ = (unsigned char)( ((int)(*pwszWideInTemp)) & 0xFF);
			*temp++ = (unsigned char)((((int)(*pwszWideInTemp)) & 0xFF00) >> 8);
			pwszWideInTemp++;
		}
		*temp++ = '\0';
		*temp   = '\0';
	} else {
		// unsupported wchar_t size
		return -1;
	}

	if (0 != xBase64_encode(pUCS2, cchWideChar * 2, szUCB64Out, &cchUCB64Out)) {
		result = cchUCB64Out;
	} else {
		result = -1;
	}

	if (4 == sizeof(wchar_t))
		if (pUCS2) free(pUCS2);

	return result;
}

/* return number of bytes written to multibyte string, not including null terminator */

int uni64_to_multibyte(unsigned char* szUCB64In, unsigned char** szMBCSOut)
{
	wchar_t* wszTemp = NULL;
	int result;

	result = uni64_to_wide(szUCB64In, &wszTemp);

	if (result != -1) {
		result = wide_to_multibyte(wszTemp, szMBCSOut);
	}

	if (wszTemp) free(wszTemp);

	return result;
}

/* return number of wide chars written, excluding null terminator,
   or -1 if the number of UCS-2 bytes is uneven */

int uni64_to_wide(unsigned char* szUCB64In, wchar_t** lpWideOut)
{
	int cbUCS2Out;
	unsigned char *rgbUCS2Out;
	int result;
	int i;

	if (NULL == szUCB64In) return -1;

	result = xBase64_decode(szUCB64In, strlen(szUCB64In), &rgbUCS2Out, &cbUCS2Out);

	if ((cbUCS2Out % 2) != 0 || 0 == result)
		return -1; // uneven no of bytes, error

	*lpWideOut = (wchar_t *)malloc((cbUCS2Out/2 + 1) * sizeof(wchar_t));

	i=0;
	do {
		(*lpWideOut)[i/2] = (wchar_t)(rgbUCS2Out[i] | (rgbUCS2Out[i+1] << 8));

		i += 2;
	} while (i<cbUCS2Out);

	if (rgbUCS2Out) free(rgbUCS2Out);

	(*lpWideOut)[cbUCS2Out/2] = L'\0';

	return (cbUCS2Out/2);
}

/* return number of bytes written to multibyte string, excluding null terminator,
or -1 if the conversion could not be performed */

int wide_to_multibyte(const wchar_t* lpWideIn, unsigned char** szMBCSOut)
{
	int cbMultiByte;

	if (NULL == lpWideIn) return -1;

	cbMultiByte = wcstombs(NULL, lpWideIn, 0);

	if (cbMultiByte < 0) return cbMultiByte;

	*szMBCSOut = (unsigned char *)malloc((cbMultiByte + 1) * sizeof(char));

	wcstombs(*szMBCSOut, lpWideIn, cbMultiByte);

	(*szMBCSOut)[cbMultiByte] = '\0';

	return cbMultiByte;
}

#ifdef _Windows
int wide_to_utf8(const wchar_t* lpWideIn, unsigned char** szUTF8Out)
{
	DWORD dwFlags = 0;
	int cbMultiByte;

	cbMultiByte = WideCharToMultiByte(CP_UTF8, dwFlags, lpWideIn, -1, NULL, 0, NULL, NULL);

	*szUTF8Out = new unsigned char[cbMultiByte];
	WideCharToMultiByte(CP_UTF8, dwFlags, lpWideIn, -1, *szUTF8Out, cbMultiByte, NULL, NULL);

	return cbMultiByte;
}
#endif


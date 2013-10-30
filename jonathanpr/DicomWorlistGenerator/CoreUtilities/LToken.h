//---------------------------------------------------------------------------

#ifndef LTokenH
#define LTokenH

#include <deque>
// $Id: LToken.h,v 1.1 2013/09/13 12:46:06 jprice Exp $
// Supercides  Token.cpp and Token.h
using namespace std;
typedef deque< String > DequeString;

class LToken
{
	public:
		LToken( const AnsiString& scanFor ) ;
		void getTokens( DequeString& tokenList  );
		void setLine(String &in );
		bool nonScanForChars( const String & in );
		const String & fileID( void );
	private:
		String gSplitStr;
		String gTokenLine;
} ;
//---------------------------------------------------------------------------
#endif

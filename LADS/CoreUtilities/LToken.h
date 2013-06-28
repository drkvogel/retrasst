//---------------------------------------------------------------------------

#ifndef LTokenH
#define LTokenH

#include <deque>
// $Id: LToken.h,v 1.1 2008/07/02 13:37:15 cp Exp $
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

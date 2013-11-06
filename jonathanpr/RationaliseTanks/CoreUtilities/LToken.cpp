//---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop

#include "LToken.h"

// Load up a String of individual characters to tokenise a line with
LToken::LToken( const AnsiString&  scanFor )
{
		gSplitStr=scanFor;
}
const String & LToken::fileID( void )
{
	static String ver;
	ver=String("$ID$");
	return ver;
}

// Load up the line to Scan
void LToken::setLine(String &in )
{
		gTokenLine=in;
}
// Scan the line putting in tokens into the tokenList.
void LToken::getTokens( DequeString& tokenList  )
{
	bool testCharFound;
	String tokenBuild;
	tokenBuild="";
	tokenList.clear();
	 for( int j=1;j<=gTokenLine.Length();j++)
	{
		testCharFound=false;
// Search for a separator character, testing each separator char against the input line
		for( int k=1; !testCharFound && k<=gSplitStr.Length(); k++)
		{
				testCharFound=( gTokenLine[j]==gSplitStr[k]);
		}
		if(testCharFound)
		{
				if(!tokenBuild.IsEmpty())
						tokenList.push_back(tokenBuild);
// Push back the separator
//				tokenList.push_back(gLTokenLine[j]);
				tokenBuild="";
		}else
		{
				tokenBuild+=gTokenLine[j];
		}

	}
	if( !tokenBuild.IsEmpty())
	{
				tokenList.push_back(tokenBuild);
	}
}

bool LToken::nonScanForChars( const String & in )
{
		bool nonScanChar,scanForCharFound;
		nonScanChar=false;

		for(  int j=1;!nonScanChar && j<in.Length() ;j++)
		{
				scanForCharFound=false;
				for(  int i=1;!scanForCharFound&&i<gSplitStr.Length();i++)
				{
					  if( gSplitStr[i] == in[j] )
					  {
						 scanForCharFound=true;
					  }
				}
				if(!scanForCharFound)
				{
					 nonScanChar=true;
				}
		}
		return nonScanChar;
}
//---------------------------------------------------------------------------

#pragma package(smart_init)

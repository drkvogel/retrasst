//---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop

#include "pssymbol.h"
int bar2of5[10][6]={{0,0,0,1,1,0},
                    {1,1,0,0,0,1},
                    {2,0,1,0,0,1},
                    {3,1,1,0,0,0},
                    {4,0,0,1,0,1},
                    {5,1,0,1,0,0},
                    {6,0,1,1,0,0},
                    {7,0,0,0,1,1},
                    {8,1,0,0,1,0},
                    {9,0,1,0,1,0}};

Code128set code128set[107]={
{0," "," ","00",2,1,2,2,2,2},
{1,"!","!","01",2,2,2,1,2,2},
{2,"\"","\"","02",2,2,2,2,2,1},
{3,"#","#","03",1,2,1,2,2,3},
{4,"$","$","04",1,2,1,3,2,2},
{5,"%","%","05",1,3,1,2,2,2},
{6,"&","&","06",1,2,2,2,1,3},
{7,"'","'","07",1,2,2,3,1,2},
{8,"(","(","08",1,3,2,2,1,2},
{9,")",")","09",2,2,1,2,1,3},
{10,"*","*","10",2,2,1,3,1,2},
{11,"+","+","11",2,3,1,2,1,2},
{12,",",",","12",1,1,2,2,3,2},
{13,"-","-","13",1,2,2,1,3,2},
{14,".",".","14",1,2,2,2,3,1},
{15,"/","/","15",1,1,3,2,2,2},
{16,"0","0","16",1,2,3,1,2,2},
{17,"1","1","17",1,2,3,2,2,1},
{18,"2","2","18",2,2,3,2,1,1},
{19,"3","3","19",2,2,1,1,3,2},
{20,"4","4","20",2,2,1,2,3,1},
{21,"5","5","21",2,1,3,2,1,2},
{22,"6","6","22",2,2,3,1,1,2},
{23,"7","7","23",3,1,2,1,3,1},
{24,"8","8","24",3,1,1,2,2,2},
{25,"9","9","25",3,2,1,1,2,2},
{26,":",":","26",3,2,1,2,2,1},
{27,";",";","27",3,1,2,2,1,2},
{28,"<","<","28",3,2,2,1,1,2},
{29,"=","=","29",3,2,2,2,1,1},
{30,">",">","30",2,1,2,1,2,3},
{31,"?","?","31",2,1,2,3,2,1},
{32,"@","@","32",2,3,2,1,2,1},
{33,"A","A","33",1,1,1,3,2,3},
{34,"B","B","34",1,3,1,1,2,3},
{35,"C","C","35",1,3,1,3,2,1},
{36,"D","D","36",1,1,2,3,1,3},
{37,"E","E","37",1,3,2,1,1,3},
{38,"F","F","38",1,3,2,3,1,1},
{39,"G","G","39",2,1,1,3,1,3},
{40,"H","H","40",2,3,1,1,1,3},
{41,"I","I","41",2,3,1,3,1,1},
{42,"J","J","42",1,1,2,1,3,3},
{43,"K","K","43",1,1,2,3,3,1},
{44,"L","L","44",1,3,2,1,3,1},
{45,"M","M","45",1,1,3,1,2,3},
{46,"N","N","46",1,1,3,3,2,1},
{47,"O","O","47",1,3,3,1,2,1},
{48,"P","P","48",3,1,3,1,2,1},
{49,"Q","Q","49",2,1,1,3,3,1},
{50,"R","R","50",2,3,1,1,3,1},
{51,"S","S","51",2,1,3,1,1,3},
{52,"T","T","52",2,1,3,3,1,1},
{53,"U","U","53",2,1,3,1,3,1},
{54,"V","V","54",3,1,1,1,2,3},
{55,"W","W","55",3,1,1,3,2,1},
{56,"X","X","56",3,3,1,1,2,1},
{57,"Y","Y","57",3,1,2,1,1,3},
{58,"Z","Z","58",3,1,2,3,1,1},
{59,"[","[","59",3,3,2,1,1,1},
{60,"\\","\\","60",3,1,4,1,1,1},
{61,"]","]","61",2,2,1,4,1,1},
{62,"^","^","62",4,3,1,1,1,1},
{63,"_","_","63",1,1,1,2,2,4},
{64,"NUL","`","64",1,1,1,4,2,2},
{65,"SOH","a","65",1,2,1,1,2,4},
{66,"STX","b","66",1,2,1,4,2,1},
{67,"ETX","c","67",1,4,1,1,2,2},
{68,"EOT","d","68",1,4,1,2,2,1},
{69,"ENQ","e","69",1,1,2,2,1,4},
{70,"ACK","f","70",1,1,2,4,1,2},
{71,"BEL","g","71",1,2,2,1,1,4},
{72,"BS","h","72",1,2,2,4,1,1},
{73,"HT","i","73",1,4,2,1,1,2},
{74,"LF","j","74",1,4,2,2,1,1},
{75,"VT","k","75",2,4,1,2,1,1},
{76,"FF","I","76",2,2,1,1,1,4},
{77,"CR","m","77",4,1,3,1,1,1},
{78,"SO","n","78",2,4,1,1,1,2},
{79,"SI","o","79",1,3,4,1,1,1},
{80,"DLE","p","80",1,1,1,2,4,2},
{81,"DC1","q","81",1,2,1,1,4,2},
{82,"DC2","r","82",1,2,1,2,4,1},
{83,"DC3","s","83",1,1,4,2,1,2},
{84,"DC4","t","84",1,2,4,1,1,2},
{85,"NAK","u","85",1,2,4,2,1,1},
{86,"SYN","v","86",4,1,1,2,1,2},
{87,"ETB","w","87",4,2,1,1,1,2},
{88,"CAN","x","88",4,2,1,2,1,1},
{89,"EM","y","89",2,1,2,1,4,1},
{90,"SUB","z","90",2,1,4,1,2,1},
{91,"ESC","{","91",4,1,2,1,2,1},
{92,"FS","|","92",1,1,1,1,4,3},
{93,"GS","}","93",1,1,1,3,4,1},
{94,"RS","~","94",1,3,1,1,4,1},
{95,"US","DEL","95",1,1,4,1,1,3},
{96,"FNC 3","FNC 3","96",1,1,4,3,1,1},
{97,"FNC 2","FNC 2","97",4,1,1,1,1,3},
{98,"SHIFT","SHIFT","98",4,1,1,3,1,1},
{99,"CODE C","CODE C","99",1,1,3,1,4,1},
{100,"CODE B","FNC 4","CODE B",1,1,4,1,3,1},
{101,"FNC 4","CODE A","CODE A",3,1,1,1,4,1},
{102,"FNC 1","FNC 1","FNC 1",4,1,1,1,3,1},
{103,"Start A","Start A","Start A",2,1,1,4,1,2},
{104,"Start B","Start B","Start B",2,1,1,2,1,4},
{105,"Start C","Start C","Start C",2,1,1,2,3,2},
{106,"Stop","Stop","Stop",2,3,3,1,1,1}
};

//BarcodeSymbology::*BarcodeSymbology BarcodeSymbology::bs;
BarcodeSymbology * BarcodeSymbology::bs;
bool BarcodeSymbology::isBarcodeCheckDigit;
bool BarcodeSymbology::barcodeOrientation;
String BarcodeSymbology::lastBarcodeAndCheckDigit;
String BarcodeSymbology::lastTBarcodeAndCheckDigit;
String BarcodeSymbology::checkDigitSeparator;
//-------------------------------------------------------------
//   BarcodeSymbology
//-------------------------------------------------------------

BarcodeSymbology::BarcodeSymbology( )
{
    setSymboliser( this );
    isBarcodeCheckDigit=false;
//  barcodeOrientation=false signifies a horizontal barcode;
    barcodeOrientation=false;
    lastBarcodeAndCheckDigit="";
    lastTBarcodeAndCheckDigit="";
}
 void BarcodeSymbology::barWidths(int maxChar, float labelIncX, float labelIncY)
{
}
void BarcodeSymbology::scaleBarCode( VecBar& aa, float maxUnitBars, float barHeight, float xRange,float yRange )
{

// When the barcode has been processed here the total co-ordinates (xstart,ystart)
// and (xend,yend ) should be in the range of a maximum of 0 to 1. this is modified by
// the values in xRange and yRange.

//        float x1,x2,y1,y2;
        if( !getBarcodeOrientation() )
        {
// Horizontal Barcode
		for( unsigned int i=0;i< aa.size();i++)
        {
// lines below left in for testing
//               x1  = xRange*(aa[i].xStart/maxUnitBars);
//               y1  = yRange*(aa[i].yStart);
//               x2  = xRange*(aa[i].xEnd/maxUnitBars);
//               y2  = yRange*(aa[i].yEnd );

			   aa[i].xStart= xRange*(aa[i].xStart/maxUnitBars);
               aa[i].yStart= yRange*(aa[i].yStart);
               aa[i].xEnd  = xRange*(aa[i].xEnd/maxUnitBars);
               aa[i].yEnd  = yRange*(aa[i].yEnd );

        }
        }else
        {
// Vertical Barcode
            for( unsigned int i=0;i< aa.size();i++)
            {
// lines below left in for testing
//               x1  = xRange*(aa[i].xStart);
//               y1  = yRange*(aa[i].yStart/maxUnitBars);
//               x2  = xRange*(aa[i].xEnd);
//               y2  = yRange*(aa[i].yEnd/maxUnitBars );

               aa[i].xStart= xRange*(aa[i].xStart);
               aa[i].yStart= yRange*(aa[i].yStart/maxUnitBars);
               aa[i].xEnd  = xRange*(aa[i].xEnd);
               aa[i].yEnd  = yRange*(aa[i].yEnd/maxUnitBars );
            }
		}
}

//-------------------------------------------------------------
//   BarcodeSymbologyCode39
//-------------------------------------------------------------

BarcodeSymbologyCode39::BarcodeSymbologyCode39 ( )
{
    setSymboliser( this );
}
bool BarcodeSymbologyCode39::encode(const String & pCode, VecBar& bars, const float barWidth, const float barHeight, const float xRange, const float yRange)
{ return true;
}

void BarcodeSymbologyCode39::barWidths(int maxChar, float labelIncX, float labelIncY)
{
        maxUnitBars=( 10 +11+(11*maxChar) +13 +10 );
        widthOfBar=labelIncX/maxUnitBars;

        barWidth=labelIncX;
        barHeight=labelIncY;
}

//-------------------------------------------------------------
//   BarcodeSymbology2of5
//-------------------------------------------------------------

BarcodeSymbology2of5::BarcodeSymbology2of5 ( )
{
    setSymboliser( this );
}

 void BarcodeSymbology2of5::barWidths(int maxChar, float labelIncX, float labelIncY)
{
         int pairs=(maxChar)/2;
         if(pairs*2 != maxChar )
         {
// no of  maxChars is odd no of Chars therefore round up.
                pairs++;
         }
// Width is per char in units of bar widths ( 2.5*2 + 3)
// Times two for each pair
// 2*10  for the quiet zones
// 4 for the stop
// 4.5 for the start
		maxUnitBars=((8*2*(float)pairs)+(20+4+4.5));
		if(getBarcodeOrientation())
		{
				widthOfBar=labelIncX/maxUnitBars;
		}else
		{
				widthOfBar=labelIncY/maxUnitBars;
		}

		barWidth=labelIncX;
		barHeight=labelIncY;
}
bool BarcodeSymbology2of5::encode(const String & pCode, VecBar& bars, const float barWidth, const float barHeight, const float xRange,const float yRange)
{
		rawEncode( pCode,bars,barWidth,barHeight, xRange, yRange );
		scaleBarCode(  bars, maxUnitBars,  barHeight, xRange,yRange ) ;
		return true;
}
int BarcodeSymbology2of5::calcCheckDigit( const String & pBarcode  ) const
{

	int evenDigit,oddDigit,firstDigit,secondDigit,totalSum,rem, barLen;

	evenDigit=0;
	oddDigit=0;
	barLen=pBarcode.Length();
	for( int i=1; i<= barLen; i=i+2)
	{
		firstDigit=pBarcode[i]-'0';
		if(firstDigit>9 || firstDigit<0 )
		{
		} else
		{
			 evenDigit+=firstDigit;
		}
		if( (i+1 ) < barLen )
		{
				secondDigit=pBarcode[i+1]-'0';
				if(secondDigit>9 || secondDigit<0 )
				{
				}else
				{
						oddDigit+=secondDigit;
				}
		}
	}
	totalSum= ( oddDigit + (3 * evenDigit ));
	rem= totalSum%10 ;
	if ( rem == 0 ) {return 0; }
	return (10- rem );


}
void BarcodeSymbology2of5::addCheckDigit( const String &pCode, String & newBarcode )
{
	int internalCheckDigit;
	String code;

	code="";
	internalCheckDigit=0;
	newBarcode="";
// extract numbers starting from 0

	int i= pCode.Length();
	int j= i%2;
	if( j == 0 )
	{
// Even Number of digits
		if(! BarcodeSymbology::isBarcodeCheckDigit )
		{
				code=pCode;
		}else
		{
// Check digit required Even Length - therefore add a leading zero
// and a trailing checkdigit to barcode

				code=String("0")+pCode;
				internalCheckDigit=calcCheckDigit(code);
				lastTBarcodeAndCheckDigit=code+checkDigitSeparator+String( internalCheckDigit );
				code+=String( internalCheckDigit );
		}

	}else
	{
// Odd Number of digits
		if(! BarcodeSymbology::isBarcodeCheckDigit )
		{
				code="0"+pCode;
		}else
		{
// trailing checkdigit added to barcode
				code=pCode;
				internalCheckDigit=calcCheckDigit(pCode);
				lastTBarcodeAndCheckDigit=code+checkDigitSeparator+String( internalCheckDigit );
				code+=String( internalCheckDigit );
		}

	}
	lastBarcodeAndCheckDigit=code;
	newBarcode=code;

}
bool BarcodeSymbology2of5::rawEncode(const String & pCode, VecBar& bars, const float barWidth, const float barHeight, const float xRange,const float yRange)
{
// Assume in format of sets of numeric pairs
// barcode generated
	barDetails b;
//    int internalCheckDigit;
	bars.clear();
	String code,newBarcode;
	bool valid;
	valid=true;
//    internalCheckDigit=0;


   addCheckDigit( pCode, code );


// Quiet zone
	b.barWide=10;
	b.inkOn=0;
	bars.push_back(b);
// Add the start bars
	b.barWide=0;
// Thin black
	b.inkOn=1;
	bars.push_back(b);
// Thin White
	b.inkOn=0;
	bars.push_back(b);
// Thin Black
	b.inkOn=1;
	bars.push_back(b);
// Thin White
	b.inkOn=0;
	bars.push_back(b);

	int firstDigit,secondDigit;
	for( int i=1; i< code.Length(); i=i+2)
	{
		firstDigit=code[i]-'0';
		if(firstDigit>9 || firstDigit<0 )
		{
				bars.clear();
				valid=false;
				return valid;
		}
		secondDigit=code[i+1]-'0';
		if(secondDigit>9 || secondDigit<0 )
		{
				bars.clear();
				valid=false;
				return valid;
		}
		for ( int j=1;j<6;j++ )
		{
				b.barWide=bar2of5[firstDigit][j];
				b.inkOn=1;
				bars.push_back(b);
				b.barWide=bar2of5[secondDigit][j];
				b.inkOn=0;
				bars.push_back(b);
		}

	}
// Add the stop bars
	b.barWide=1;
	b.inkOn=1;
	bars.push_back(b);
	b.barWide=0;
	b.inkOn=0;
	bars.push_back(b);
	b.inkOn=1;
	bars.push_back(b);
// Quiet zone
	b.barWide=10;
	b.inkOn=0;
	bars.push_back(b);

	float xStartPos=0.0;
	float xEndPos=0.0;
	float yStartPos=0.0;
	if( !getBarcodeOrientation() )
	{
// Horizontal Barcode
		for( unsigned int i=0; i< bars.size(); i++ )
		{

				bars[i].xStart=xStartPos;
				bars[i].yStart=yStartPos;
				switch(bars[i].barWide)
				{
						case 0:
						  xEndPos++;
						break;
						case 1:
						  xEndPos+=2.5;
						break;
						case 10:
						  xEndPos+=10;
						break;
				}
				bars[i].xEnd=xEndPos;
				bars[i].yEnd=1.00;//barHeight;
				xStartPos=xEndPos;

		}
	}else
	{
// Vertical Barcode
		float yEndPos=0;
		for( unsigned int i=0; i< bars.size(); i++ )
		{

				bars[i].xStart=xStartPos;
				bars[i].yStart=yStartPos;
				switch(bars[i].barWide)
				{
						case 0:
						  yEndPos++;
						break;
						case 1:
						  yEndPos+=2.5;
						break;
						case 10:
						  yEndPos+=10;
						break;
				}
				bars[i].yEnd=yEndPos;
				bars[i].xEnd=1.00;//barHeight;
				yStartPos=yEndPos;
		}
	}
	return valid;
}

//-------------------------------------------------------------
//   BarcodeSymbologyCode128
//-------------------------------------------------------------

BarcodeSymbologyCode128::BarcodeSymbologyCode128 ( )
{
	setSymboliser( this );
}
void BarcodeSymbologyCode128::barWidths(int maxChar, float labelIncX, float labelIncY)
{
// bars consist of a
// quiet zone = 10 white units
// start char = 11
// 11 bars for each char encoded
//  stop char = 13
// quiet zone = 10 white units

        maxUnitBars=( 10 +11+(11*maxChar)+11 +13 +10 );
        widthOfBar=labelIncX/maxUnitBars;

        barWidth=labelIncX;
        barHeight=labelIncY;
}
bool BarcodeSymbologyCode128::encode(const String & pCode, VecBar& bars, const float pBarWidth, const float pBarHeight, const float xRange, const float yRange)
{
        rawEncode(pCode, bars, pBarWidth, pBarHeight, xRange,yRange);
		scaleBarCode(  bars, maxUnitBars, pBarHeight,xRange,yRange);

        return true;
}
bool BarcodeSymbologyCode128::rawEncode(const String & pCode, VecBar& bars, const float pBarWidth, const float pBarHeight, const float xRange, const float yRange)
{
// Assume in format of sets of numeric pairs
// barcode generated
    barDetails b;
    bars.clear();
    String code;
    bool valid;
    valid=true;

    code=pCode;
// Quiet zone
    b.barWide=10;
    b.inkOn=0;
    b.isValue=false;
    b.valueCode128=0;
    bars.push_back(b);
// Add the start bars
    loadCharBars("Start B",bars );
    char itemChar[2];
    itemChar[1]='\0';

    for( int i=1; i<= code.Length(); i++)
    {
        itemChar[0]=code[i];
        loadCharBars(itemChar,bars );

    }
// Calculating Check Digit
    int digitPos=0;
    int CheckTotal=0;// Value of start code B
    for ( unsigned int k =0;k< bars.size();k++)
    {
        if (bars[k].isValue)
        {
             if(digitPos==0)
             {
                CheckTotal=CheckTotal+bars[k].valueCode128;
                digitPos++;
             }else
             {
                CheckTotal=CheckTotal+(bars[k].valueCode128 * digitPos );
                digitPos++;
             }
        }

    }
    int rem=(CheckTotal%103);
    loadValueBars(rem,bars);
// Add the stop bars
    loadCharBars("Stop",bars);
// Quiet zone
    b.barWide=10;
    b.inkOn=0;
    bars.push_back(b);

    int totalBars=0;
    float xStartPos=0.0;
    float xEndPos=0.0;
    float yStartPos=0.0;
//    float yEndPos=1;
    for( unsigned int i=0; i< bars.size(); i++ )
    {

        bars[i].xStart=xStartPos;
        bars[i].yStart=yStartPos;
        totalBars+=bars[i].barWide;
        switch(bars[i].barWide)
        {
                case 1:
                case 2:
                case 3:
                case 4:
//                        xEndPos=(barWidth * bars[i].barWide)+xEndPos ;
//                        xEndPos=(pBarWidth *totalBars) ;
                          xEndPos=totalBars;
                break;
                case 10:
//                        xEndPos=( pBarWidth * totalBars);
                          xEndPos=totalBars;
                break;
        }
        bars[i].xEnd=xEndPos;
        bars[i].yEnd=1;
        xStartPos=xEndPos;

    }

    return valid;
}

int BarcodeSymbologyCode128::ifindCode( const char * s )
{
   for (int i=0;i<107  ; i++ )
   {
        if(strcmp(code128set[i].codeB,s)== 0 )
        {
                return i;
        }
   }
   return NOT_FOUND_CODE128;
}
bool BarcodeSymbologyCode128::loadCharBars(const char * s, VecBar& bars)
{
    int i=ifindCode(  s);
    if ( i == NOT_FOUND_CODE128 )
    {
         return false;
    }
    return loadValueBars(i,bars);
}
bool BarcodeSymbologyCode128::loadValueBars( const int  i,VecBar &bars )
{
    barDetails b;
    if(i== NOT_FOUND_CODE128)
    {
        return false;
    }
    b.barWide=code128set[i].bar1;
    b.inkOn=1;
// Mark digits/bars which are going to be added into check digit
// The Stop digit isn't all others are.
    if( i==STOP_DIGIT)
    {
        b.valueCode128=0;
        b.isValue=false;
    }else{
        b.valueCode128=code128set[i].value;
        b.isValue=true;
    }
    bars.push_back(b);

    b.barWide=code128set[i].space1;
    b.inkOn=0;

// Set Value to Zero so only the first bar is of interest.
    b.valueCode128=0;
    b.isValue=false;
// Do not move the statement above.

    bars.push_back(b);

    b.barWide=code128set[i].bar2;
    b.inkOn=1;
    bars.push_back(b);

    b.barWide=code128set[i].space2;
    b.inkOn=0;
    bars.push_back(b);

    b.barWide=code128set[i].bar3;
    b.inkOn=1;
    bars.push_back(b);

    b.barWide=code128set[i].space3;
    b.inkOn=0;
    bars.push_back(b);

// char is a stop code add on extra black line 2 units wide.
    if( i==STOP_DIGIT )
    {
            b.barWide=2;
            b.inkOn=1;
            bars.push_back(b);
    }
    return true;
}

//---------------------------------------------------------------------------

#pragma package(smart_init)

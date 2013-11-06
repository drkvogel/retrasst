//---------------------------------------------------------------------------

#ifndef PSSymbolH
#define PSSymbolH

/*
Author: Andrew Munday. initial relased into CVS 22-01-2009.

pssymbol.cpp,.h hold the code to generate barcode in two formats at the moment.
These formats being interleave 2of5 and code 128.
Code 39 hasn't been implemented yet but a class has been defined.

BarcodeSymbology is the base class for the rest of the other format types
and must be used as a template for futher implementions of barcode formats.

$id$

*/

#include <deque.h>
const int NOT_FOUND_CODE128=-1;
const int STOP_DIGIT=106;

struct barDetails
{
        int barWide;
		int inkOn;
        float xStart;
        float yStart;
        float xEnd;
        float yEnd;
        int valueCode128;
        bool isValue;
};

struct Code128set{
int value;
char * codeA;
char * codeB;
char * codeC;
int bar1;
int space1;
int bar2;
int space2;
int bar3;
int space3;
};

typedef std::deque< barDetails > VecBar;
class BarcodeSymbology
{
        public:
                BarcodeSymbology();
                virtual ~BarcodeSymbology(){};
                static  BarcodeSymbology* getSymboliser( void )
                {        return bs;};

                static const void  setSymboliser( BarcodeSymbology *pbs )
                {        bs=pbs;  };

                virtual bool encode( const String & pCode
                         ,VecBar& bars
                         ,const float barWidth
                         ,const float barHeight
                         ,const float xRange
                         ,const float yRange ){return false;};

                virtual bool rawEncode( const String & pCode
                         ,VecBar& bars
                         ,const float barWidth
                         ,const float barHeight
                         ,const float xRange
                         ,const float yRange ){return false;};

        static String & getLastBarcodeWithCheck( void ) {return lastBarcodeAndCheckDigit;}
        static const String & getLastTBarcodeWithCheck( void ){return lastTBarcodeAndCheckDigit;}
        virtual void barWidths( int maxChar, float labelIncX, float labelIncY) ;
        virtual void setBarcodeCheckDigit( bool x ) const {isBarcodeCheckDigit=x; };
        virtual const bool getBarcodeCheckDigit( void  ){return isBarcodeCheckDigit; };
        virtual void setBarcodeOrientation( bool x ) const {barcodeOrientation=x; };
        virtual const bool getBarcodeOrientation( void  ){return barcodeOrientation; };
        virtual const void setCheckDigitSeparator( String & x ){checkDigitSeparator=x; };

        virtual int calcCheckDigit( const String & pBarcode ) const { return 0;};

        protected:

        virtual void addCheckDigit( const String &pCode, String & newBarcode ){};

		virtual void scaleBarCode( VecBar& aa, float maxUnitBars, float barHeight, float xRange,float yRange  );
               static BarcodeSymbology* bs;
               float widthOfBar;
			   float  barWidth;
               float  barHeight;
			   float maxUnitBars;
        static bool isBarcodeCheckDigit;
        static bool barcodeOrientation;
        static String lastBarcodeAndCheckDigit;
        static String lastTBarcodeAndCheckDigit;
        static String checkDigitSeparator;
        private:


};
class BarcodeSymbology2of5: public BarcodeSymbology
{
		public:
                BarcodeSymbology2of5();
                virtual ~BarcodeSymbology2of5(){};
     virtual int calcCheckDigit( const String & pBarcode  ) const;

     virtual void barWidths( int maxChar, float labelIncX, float labelIncY) ;
     virtual bool encode( const String & pCode
                         ,VecBar& bars
                         ,const float barWidth
                         ,const float barHeight
                         ,const float xRange
                         ,const float yRange );
     virtual bool rawEncode( const String & pCode
                         ,VecBar& bars
                         ,const float barWidth
                         ,const float barHeight
                         ,const float xRange
                         ,const float yRange );

     virtual void addCheckDigit( const String &pCode, String & newBarcode );

};
class BarcodeSymbologyCode128: public BarcodeSymbology
{
        public:
                BarcodeSymbologyCode128();
                virtual ~BarcodeSymbologyCode128(){};
     virtual void barWidths( int maxChar, float labelIncX, float labelIncY) ;
     virtual bool encode( const String & pCode
                         ,VecBar& bars
                         ,const float barWidth
                         ,const float barHeight
                         ,const float xRange
                         ,const float yRange );
     virtual bool rawEncode( const String & pCode
                         ,VecBar& bars
                         ,const float barWidth
                         ,const float barHeight
                         ,const float xRange
                         ,const float yRange );


		protected:

     virtual int ifindCode( const char * s );
     virtual bool loadCharBars(const char * s, VecBar& bars);
     virtual bool loadValueBars( const int  i,VecBar &bars );
};
class BarcodeSymbologyCode39: public BarcodeSymbology
{
        public:
                BarcodeSymbologyCode39();
                virtual ~BarcodeSymbologyCode39();
     virtual bool encode( const String & pCode
                         ,VecBar& bars
                         ,const float barWidth
						 ,const float barHeight
                         ,const float xRange
                         ,const float yRange );

     virtual bool rawEncode( const String & pCode
                         ,VecBar& bars
                         ,const float barWidth
                         ,const float barHeight
                         ,const float xRange
                         ,const float yRange );

     virtual void barWidths( int maxChar, float labelIncX, float labelIncY) ;
        protected:
};
class BarcodeSymbologyPlainText: public BarcodeSymbology
{
};

//---------------------------------------------------------------------------
#endif

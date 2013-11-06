//---------------------------------------------------------------------------

#ifndef psitemH
#define psitemH
/*
Author: Andrew Munday. initial relased into CVS 22-01-2009.

PSItems attempts to handle standard printed items through a standard interface.
This enables all the items to be printed with a one stamp command after all the
data has been set up for the items.

$id$

*/

#include "PSRect.h"
class PSPrinter;
class PSItemInterface: public PSRect
{
		protected:
				virtual void draw()=0;
				virtual void setPrinter(PSPrinter *ppr)=0;
				virtual void setCheckDigitVisible( bool checkFlag ) =0;
				virtual void setContent( const String & pStr )=0;
				virtual void setFontsize( int pFontSize )=0;
				virtual void setBitmap( Graphics::TBitmap* pBm)=0;
				virtual void setLabelPi( PSRect* pLabelPi )=0;
				virtual void setPixelOffset( PSSize* pPo )=0;

};
class PSItemBase:public PSItemInterface
{
		public:
				virtual void setContent( const String & pStr ){content=pStr;}
				virtual void setPrinter(PSPrinter *ppr){pr=ppr;}
				virtual void draw(){};
				virtual void setLabelPi( PSRect* pLabelPi );
				virtual void setBitmap( Graphics::TBitmap* pBm){bm=pBm;}
				virtual void setPixelOffset( PSSize* pPo ){pixelOffset=pPo;};
				virtual void setFontsize( int pFontSize ){};
				virtual void setCheckDigitVisible( bool checkFlag ){};
				virtual ~PSItemBase();

		protected:

				bool checkFlag;
				PSPrinter* pr;
				TPrinter* p;
				int fontSize;
				String content;
				PSRect* labelPi;
				PSSize* pixelOffset;
				Graphics::TBitmap* bm;
				PSItemBase();

};
class PSItemText:public PSItemBase
{
		public:
				virtual void draw();
				virtual void setFontsize( int pFontSize ){fontSize=pFontSize;}

};

class PSItemBarcode:public PSItemBase
{
		public:
				virtual void setCheckDigitVisible( bool pCheckFlag ){checkFlag=pCheckFlag;}
				virtual void draw();
				virtual void setFontsize( int pFontSize ){fontSize=pFontSize;}
};
class PSItemBitmap:public PSItemBase
{
		public:
				virtual void draw();
};
class PSItemRect: public PSItemBase
{
		public:
				virtual void draw();
};
class PSItemFrameRect: public PSItemBase
{
		public:
				virtual void draw();
};
//---------------------------------------------------------------------------
#endif

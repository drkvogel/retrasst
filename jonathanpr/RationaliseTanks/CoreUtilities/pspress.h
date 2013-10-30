//---------------------------------------------------------------------------

#ifndef PSPressH
#define PSPressH

/*
Author: Andrew Munday. initial relased into CVS 22-01-2009.

pspress.cpp,.h

PSPress is a class which pulls together the different objects to enable printing.
The class is a relatively small light weight class put is still a usefull class.

In a conceptual model this class might be regarded as the printing press.

$id$

*/

#include "psprintr.h"
#include "pspage.h"
#include "psrect.h"
#include "pssize.h"
#include "psstamp.h"

class BarcodeSymbology;

class PSPressInterface
{
	protected:
			virtual void intialize(void)=0;
			virtual PSPageBase* getPg( void )=0;
			virtual void newPage( void)=0;
			virtual void endDoc( void)=0;
};
class PSPressBase : public PSPressInterface
{
		public:
			PSPressBase(const String& printerName,const String& symbology, const int barcodeLength);
			virtual void intialize(void);
			virtual PSPageBase* getPg( void );
			virtual void newPage( void);
			virtual void setBarcodeCheckDigit( bool isCheckDigit ) const;
			virtual void endDoc( void);
			virtual ~PSPressBase(void);
		protected:
				PSPageBase* pg;
				PSPrinter*      p;
				TPrinter*       pr;
				PSStamper *stamper;
				static BarcodeSymbology * barSym;
				static int maxChar;

		virtual void selectSymbolizer( String & Symbolizer,const int pMaxChar );

};
class PSPress : public PSPressBase
{
		public:
			PSPress(const String& printerName,const String& symbology, const int barcodeLength);
			~PSPress();
};
class PSMSPress : public PSPressBase
{
		public:
			PSMSPress(const String& printerName,const String& symbology, const int barcodeLength);
			~PSMSPress();

};
//---------------------------------------------------------------------------
#endif

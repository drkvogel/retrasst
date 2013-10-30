//---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop

#include "PSPress.h"
#include "psrect.h"
#include "pssize.h"
#include "psstamp.h"
#include "psitem.h"
#include "pssymbol.h"

#pragma package(smart_init)

BarcodeSymbology* PSPressBase::barSym;

int PSPressBase::maxChar;
//---------------------------------------------------------------------------
PSPressBase::PSPressBase(const String& printerName,const String& symbology, const int barcodeLength)
{
		p=new PSPrinter(printerName);
		pr=p->getCurrPrinter();

		selectSymbolizer( String(symbology),barcodeLength);
		setBarcodeCheckDigit(false);
		maxChar=barcodeLength;

		p->beginDoc();

};
PSPressBase::~PSPressBase(void)
{
	   delete p;
}

void PSPressBase::selectSymbolizer( String & Symbolizer ,const int pMaxChar)
{

	 if( Symbolizer == "2of5" )
	 {
		barSym=new BarcodeSymbology2of5();

	 }else if ( Symbolizer == "code128" )
	 {
		barSym=new BarcodeSymbologyCode128();
	 }else if ( Symbolizer == "code39" )
	 {
//        barSym=new BarcodeSymbologyCode39();

	 }
}
void  PSPressBase::setBarcodeCheckDigit( bool isCheckDigit ) const
{
		barSym->setBarcodeCheckDigit(isCheckDigit );
}
void PSPressBase::intialize(void)
{
		pg->getPageSize()->setSizeX(p->getPageSize()->getSizeX());
		pg->getPageSize()->setSizeY(p->getPageSize()->getSizeY());
//		pg->calcLabelSize();
		pg->calcLayout();

		pg->nextLabel();

		float labelX,labelY;
		PSRect* labelArea;
		labelArea=pg->getLabelArea();

		labelX=labelArea->getEndXmm()- labelArea->getStartXmm();
		labelY=labelArea->getEndYmm()- labelArea->getStartYmm();
		barSym->barWidths(maxChar,labelX,labelY);

		stamper->copy(pg->getLabelArea());

}
PSPageBase* PSPressBase::getPg( void )
{
		return pg;
}
void PSPressBase::newPage( void)
{
		p->newPage();pg->clear();
}

void PSPressBase::endDoc( void)
{
		pr->EndDoc();
}


PSPress::PSPress(const String& printerName,const String& symbology, const int barcodeLength):PSPressBase(printerName,symbology,barcodeLength)
{


		pg=new PSPage();
		pg->clear();

// get a pointer to the pages unprintable area
		PSRect* unprintablePA;
		unprintablePA=pg->getUnprintable();
// copy over the printers unprintable area to pages.
		unprintablePA->copy(p->getUnprintable() );

		stamper= pg->getStamper();
		stamper->setPrinter( p);
		stamper->setDpcm( p->getDpcm());
		stamper->setDpi( p->getDpi());
		stamper->setPixelOffset( p->getPixelOffset());

}

PSPress::~PSPress(void)
{
	   delete pg;
	   delete barSym;
}

PSMSPress::PSMSPress(const String& printerName,const String& symbology, const int barcodeLength):PSPressBase(printerName,symbology,barcodeLength)
{
		pg=new PSMSPage();
		pg->clear();

// get a pointer to the pages unprintable area
		PSRect* unprintablePA;
		unprintablePA=pg->getUnprintable();
// copy over the printers unprintable area to pages.
		unprintablePA->copy(p->getUnprintable() );

		stamper= pg->getStamper();
		stamper->setPrinter( p);
		stamper->setDpcm( p->getDpcm());
		stamper->setDpi( p->getDpi());
		stamper->setPixelOffset( p->getPixelOffset());

}
PSMSPress::~PSMSPress(void)
{
	   delete pg;
	   delete barSym;
}



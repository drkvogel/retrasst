//---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop

#include "psprintr.h"
#include "psrect.h"
#include "pssize.h"

#pragma package(smart_init)

//---------------------------------------------------------------------------

PSPrinter::PSPrinter(const String& printerName )
{
		pr= Printer();
		printerIndex=selectPrinter(printerName );
}

//---------------------------------------------------------------------------

bool PSPrinter::beginDoc()
{
	try
	{
			pr->BeginDoc();
	}
	catch(Exception &e )
	{
		return (false );
	}
	return (true);
}

//---------------------------------------------------------------------------

bool PSPrinter::newPage()
{
    try
    {
            pr->NewPage();
    }
    catch(Exception &e )
	{
        return (false );
    }
    return (true);
}

//---------------------------------------------------------------------------

bool PSPrinter::endPage()
{;
	try
	{
		pr->EndDoc();
	}
	catch(Exception &e )
	{
		return (false );
	}
	return (true);
}

//---------------------------------------------------------------------------

int PSPrinter::selectPrinter(const String& printerName )
{
		int prIndex=-2;
		for (int i=0;i<pr->Printers->Count;i++)
		{
				if(pr->Printers->Strings[i].Pos(printerName)!=0)
				{
					prIndex=i;
					break;
				}
		}

		if( prIndex!=-2)
		{
				pr->PrinterIndex=prIndex;
		}

		pageSize=LASERA4;
		if( printerName.Pos("Zebra LP2844") !=0 || printerName.Pos("Eltron TLP2742") != 0 )
		{
				pageSize= ELTRON;
		}

		unprintable = new PSRect();
		pagesizes   = new PSSize();
		pixelOffset = new PSSize();

		 int printerPageSizeX;
		 int printerPageSizeY;

		switch( pageSize )
		{
			case LASERA4:
			{
				// This is the physical page size, in mm, of an A4 page
				printerPageSizeX = 210;
				printerPageSizeY = 297;

				// Windows uses its own units to measure the page size
				int pxWidth = GetDeviceCaps( pr->Handle, PHYSICALWIDTH );
				int pxHeight = GetDeviceCaps( pr->Handle, PHYSICALHEIGHT );
				int offsetX = GetDeviceCaps( pr->Handle, PHYSICALOFFSETX );
				int offsetY = GetDeviceCaps( pr->Handle, PHYSICALOFFSETY );

				 // calculate average dpi, rounded to nearest 10
				dpmmX = float( pxWidth ) / printerPageSizeX;
				dpmmY = float( pxHeight ) / printerPageSizeY;
				dpcm = (dpmmX + dpmmY) * 5;
				dpi = int( dpcm * 0.254 + 0.5) * 10;

				// calculate right and bottom unprintable margins
				int upmRight = pxWidth - pr->PageWidth - offsetX;
				int upmBottom = pxHeight - pr->PageHeight - offsetY;

				unprintable->setStartXmm( offsetX / dpmmX );
				unprintable->setStartYmm( offsetY / dpmmY );
				unprintable->setEndXmm( upmRight / dpmmX );
				unprintable->setEndYmm( upmBottom / dpmmY );

				pixelOffset->setSizeX(0);
				pixelOffset->setSizeY(0);
			}
			break;

			case ELTRON:

				 printerPageSizeX=60;  // Allow for a total boarder of 9mm 2*4.5mm
				 printerPageSizeY=20;  // Allow for a total boarder of 7mm 2*3.5mm

				 unprintable->setStartXmm(0.0);
				 unprintable->setStartYmm(0.0);
				 unprintable->setEndXmm(0.0);
				 unprintable->setEndYmm(0.0);

				 pixelOffset->setSizeX(175);
				 pixelOffset->setSizeY(-7);

				 dpi=200;
				 dpcm=dpi/2.54;
				 dpmmX=dpcm/10;
				 dpmmY=dpmmX;
		}
		pagesizes->setSizeX(printerPageSizeX);
		pagesizes->setSizeY(printerPageSizeY);
		return prIndex;
}

PSPrinter::~PSPrinter(void )
{
		delete unprintable;
		delete pagesizes;
		delete pixelOffset;
}

PSSize* PSPrinter::getPageSize()
{
        return pagesizes;
}

PSSize* PSPrinter::getPixelOffset(void )
{
        return pixelOffset;
}

TPrinter* PSPrinter::getCurrPrinter(void)
{
        return pr;
}

PSRect* PSPrinter::getUnprintable(void)
{
        return unprintable;
}



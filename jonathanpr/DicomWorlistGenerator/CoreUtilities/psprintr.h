//---------------------------------------------------------------------------

#ifndef psprintrH
#define psprintrH
/*
Author: Andrew Munday. initial relased into CVS 22-01-2009.

psprintr.cpp,.h holds details of the physical characteristics of the printer
and handles to the printer it self. at the moment only an A4 printer and eltron
printer are defined.

$id$

*/
typedef enum
{
        LASERA4,
        ELTRON
}ePagesize;
#include <stdio.h>
#include <math.h>
#include <printers.hpp>

#include "pspage.h"

class PSPrinter
{
        public:
                PSPrinter(const String& printerName);
                ~PSPrinter(void );

                PSSize* getPageSize();
                TPrinter* getCurrPrinter(void);
                PSRect *getUnprintable(void);
                PSSize* getPixelOffset( );
                int getDpcm(void){return dpcm;}
                int getDpi(void){return dpi;}
                bool beginDoc();
                bool newPage(void);
                bool endPage(void);

        private:

                int dpi;
                PSPage *pg;
                PSRect *unprintable;
                PSSize *pixelOffset;
                int selectPrinter(const String& printerName );
                int printerIndex;
                TPrinter* pr;
                ePagesize pageSize;
                PSSize* pagesizes;

                float dpmmX;
                float dpInch;
                float dpmmY;
                float dpcm;
};
//---------------------------------------------------------------------------
#endif
 
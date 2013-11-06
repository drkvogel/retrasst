//---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop

#include "psstamp.h"

//---------------------------------------------------------------------------
void PSStamper::addItem( PSItemBase * anItem )
{
        allItems.push_back(anItem );
}
void PSStamper::clearItem()
{
        allItems.clear();
}
void PSStamper::setPrinter( PSPrinter *ppr )
{
        pr=ppr;
}
void PSStamper::setDpi( int pDpi )
{
        dpi=pDpi;
}
void PSStamper::setDpcm( int pDpcm )
{
        dpcm=pDpcm;
}

void PSStamper::setPixelOffset( PSSize* ppo)
{
        pixelOffset=ppo;
}

void PSStamper::stamp( void )
{
// Work out the label boundaries in pixels

        startX = (int)((dpcm * startXmm)/10);
        endX   = (int)((dpcm * endXmm)/10);
        startY = (int)((dpcm * startYmm)/10);
        endY   = (int)((dpcm * endYmm)/10);


    for( unsigned int i=0;i<allItems.size();i++)
    {
        PSItemBase *anItem= allItems[i];
        anItem->setPrinter(pr);
        anItem->setLabelPi( this );
        anItem->setPixelOffset(pixelOffset);
        anItem->draw();
    }
}

//--------------------------------------------------------------------


#pragma package(smart_init)

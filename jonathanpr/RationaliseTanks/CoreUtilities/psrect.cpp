//---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop

#include "PSRect.h"
PSRect::PSRect( void )
{
	startX	=-1;
	startY	=-1;
	endX  	=-1;
	endY  	=-1;

	startXmm	=(float)-1.0;
	startYmm	=(float)-1.0;
	endXmm		=(float)-1.0;
	endYmm		=(float)-1.0;

	startXratio	=(float)-1.0;
	startYratio	=(float)-1.0;
	endXratio	=(float)-1.0;
	endYratio	=(float)-1.0;

}
void PSRect::setStartX( int pStartX )
{
        startX=pStartX;
}
void PSRect::setStartY( int pStartY )
{
        startY=pStartY;
}
void PSRect::setEndX( int pEndX )
{
        endX=pEndX;
}
void PSRect::setEndY( int pEndY )
{
        endY=pEndY;
}

int  PSRect::getStartX( void )
{
        return startX;
}
int  PSRect::getStartY( void )
{
        return startY;

}
int  PSRect::getEndX( void )
{
        return endX;
}
int  PSRect::getEndY( void )
{
        return endY;
}



void PSRect::setStartXmm( float pStartXmm )
{
        startXmm=pStartXmm;
}
void PSRect::setStartYmm( float pStartYmm )
{
        startYmm=pStartYmm;
}
void PSRect::setEndXmm( float pEndXmm )
{
        endXmm=pEndXmm;
}

void PSRect::setEndYmm( float pEndYmm )
{
        endYmm=pEndYmm;
}
float  PSRect::getStartXmm( void )
{
        return startXmm;
}
float  PSRect::getStartYmm( void )
{
        return startYmm;

}
float  PSRect::getEndXmm( void )
{
        return endXmm;
}
float  PSRect::getEndYmm( void )
{
        return endYmm;
}
void PSRect::setDpcm( int pDpcm )
{
        dpcm=pDpcm;
}
void PSRect::setDpi( int pDpi )
{
        dpi=pDpi;
}

void  PSRect::setStartXratio( float pStartXratio )
{
        startXratio=pStartXratio;
}
void  PSRect::setStartYratio( float pStartYratio )
{
        startYratio=pStartYratio;
}
void  PSRect::setEndXratio( float pEndXratio )
{
        endXratio=pEndXratio;
}
void  PSRect::setEndYratio( float pEndYratio )
{
        endYratio=pEndYratio;
}

void  PSRect::copy( PSRect *source)
{
    startX = source->startX;
    startY = source->startY;
    endX   = source->endX;
    endY   = source->endY;

    startXmm = source->startXmm;
    startYmm = source->startYmm;
    endXmm   = source->endXmm;
    endYmm   = source->endYmm;

    startXratio = source->startXratio;
    startYratio = source->startYratio;
    endXratio   = source->endXratio;
    endYratio   = source->endYratio;


}


//---------------------------------------------------------------------------

#pragma package(smart_init)

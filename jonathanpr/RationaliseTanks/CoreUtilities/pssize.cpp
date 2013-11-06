//---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop

#include "pssize.h"

PSSize::PSSize( void )
{
	sizeX	=-1;
	sizeY	=-1;

	sizeXmm	=	(float)-1.0;
	sizeYmm	=	(float)-1.0;

}
void PSSize::setSizeX( int x )
{   sizeX=x;
}
void PSSize::setSizeY( int y )
{    sizeY=y;
}
int  PSSize::getSizeX( void )
{    return sizeX;
}
int  PSSize::getSizeY( void )
{    return sizeY;

}

void PSSize::setSizeXmm( float x )
{   sizeXmm=x;
}
void PSSize::setSizeYmm( float y )
{    sizeYmm=y;
}
float  PSSize::getSizeXmm( void )
{    return sizeXmm;
}
float  PSSize::getSizeYmm( void )
{    return sizeYmm;

}

void PSSize::copy(PSSize* source)
{
    sizeX   = source->sizeX;
    sizeY   = source->sizeY;
    sizeXmm = source->sizeXmm;
    sizeYmm = source->sizeYmm;

}

//---------------------------------------------------------------------------

#pragma package(smart_init)

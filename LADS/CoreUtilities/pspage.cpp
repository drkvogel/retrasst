//---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop

#include "pspage.h"
#include "psstamp.h"
#include "psrect.h"
#include "pssize.h"

PSPageBase::PSPageBase(void )
{
		margins    = new PSRect();
		stamper    = new PSStamper();
		pageSize   = new PSSize();
		unprintable= new PSRect();
		colRows    = new PSSize();
		labelGap   = new PSSize();
		labelArea  = new PSRect();
		labelSize  = new PSSize(); // Physical dimensions of the label
		labelPitch = new PSSize();
}
PSPageBase::~PSPageBase(void )
{
		delete margins;
		delete stamper;
		delete pageSize;
		delete unprintable;
		delete colRows;
		delete labelGap;
		delete labelArea;
		delete labelSize;
		delete labelPitch;
}

PSRect* PSPageBase::getMargins(void )
{
		return margins;
}
PSStamper* PSPageBase::getStamper(void )
{
		return stamper;
}

PSSize* PSPageBase::getPageSize(void )
{
		return pageSize;
}
PSRect* PSPageBase::getUnprintable(void)
{
		return unprintable;
}
PSSize* PSPageBase::getColRows( void )
{
		return colRows;
}
PSSize* PSPageBase::getLabelGap(void)
{
		return labelGap;
}
PSSize* PSPageBase::getLabelPitch(void)
{
		return labelPitch;
}
PSRect* PSPageBase::getLabelArea(void)
{
		return labelArea;
}

void  PSPage::calcLayout()
{
	calcLabelSize();
}
void  PSPage::calcLabelSize( void)
{

	  float incXmm,incYmm;

	  incXmm=( pageSize->getSizeX()  - \
			 ( labelGap->getSizeXmm() * ( colRows->getSizeX() -1 ) + \
			 ( margins->getStartXmm() +margins->getEndXmm() )))/colRows->getSizeX();

	  labelSize->setSizeXmm(incXmm);

	  float pageSizeY, labelGapYmm, noColY, startYmm,endYmm, nonPStart,nonPEnd ;
	  pageSizeY	  = pageSize->getSizeY();
	  labelGapYmm = labelGap->getSizeYmm();
	  noColY	  = colRows->getSizeY();
	  startYmm	  = margins->getStartYmm();
	  endYmm	  = margins->getEndYmm();
	  nonPStart	  = unprintable->getStartYmm();
	  nonPEnd     = unprintable->getEndYmm();

//    start with page size
	  incYmm = pageSize->getSizeY();
// 	   remove margins which include the non printable area
	  incYmm = incYmm - ( margins->getStartYmm() + margins->getEndYmm() );

// 	   Add back in the non printable area as a offset
//	  incYmm = incYmm + ( unprintable->getStartYmm() + unprintable->getEndYmm());

// 		Subtract the total of the gaps.
	  incYmm = incYmm - ( labelGap->getSizeYmm() * ( colRows->getSizeY() -1 )) ;

// 		divide what is left by the number of labels.
	  incYmm = incYmm /colRows->getSizeY();

	  labelSize->setSizeYmm(incYmm);

}
void PSPage::nextLabel(  void )
{
// 	Requires LHS and Top mrgin
//  	Running count of labels across and down.
//		Number of Columns and Rows
// 		Label Size and Gap


	 float labelStartXmm,labelStartYmm,labelEndXmm,labelEndYmm;
	 float offsetX,offsetY;

	 labelCountX++;

// Work out the Offset from the LHS and Top
	 offsetX= (margins->getStartXmm() - unprintable->getStartXmm());
	 offsetY= (margins->getStartYmm() - unprintable->getStartYmm());

	 if( labelCountX >colRows->getSizeX() )
	 {
		 labelCountX=1;
		 labelCountY++;
	 }

	 labelCountTotal++;

// Work out the start of the label
	 labelStartXmm = ((labelCountX -1)*(labelSize->getSizeXmm() +labelGap->getSizeXmm() ))+ offsetX;
	 labelStartYmm = ((labelCountY -1)*(labelSize->getSizeYmm()+labelGap->getSizeYmm() ))+ offsetY;
// Work out the end of the label
	 labelEndXmm   = labelStartXmm + labelSize->getSizeXmm();
	 labelEndYmm   = labelStartYmm + labelSize->getSizeYmm();

// Store the label area.
	 labelArea->setStartXmm( labelStartXmm );
	 labelArea->setStartYmm( labelStartYmm );
	 labelArea->setEndXmm(   labelEndXmm );
	 labelArea->setEndYmm(   labelEndYmm );
}

void PSPage::clear(void )
{
		labelCountX=0;
		labelCountY=1;
		labelCountTotal=0;
}

void  PSMSPage::calcLayout(void)
{

	  labelGap->setSizeXmm( labelPitch->getSizeXmm() - labelSize->getSizeXmm() );
	  labelGap->setSizeYmm( labelPitch->getSizeYmm() - labelSize->getSizeYmm() );

	  float RHSmargin; // Margin will be calculated in mm and stored as such

	  RHSmargin = pageSize->getSizeX();
	  RHSmargin-= margins->getStartXmm();//+ unprintable->getEndXmm();
	  RHSmargin-= (labelPitch->getSizeXmm() * colRows->getSizeX());
	  RHSmargin+= labelGap->getSizeXmm();

	  margins->setEndXmm( RHSmargin );

	  float bottomMargin; // Margin will be calculated in mm and stored as such

	  bottomMargin = pageSize->getSizeX();
	  bottomMargin-= margins->getStartXmm();//+ unprintable->getEndXmm();
	  bottomMargin-= (labelPitch->getSizeXmm() * colRows->getSizeX());
	  bottomMargin+= labelGap->getSizeXmm();

	  margins->setEndXmm( bottomMargin );
}
void  PSMSPage::calcLabelSize( void)
{

}
void PSMSPage::nextLabel(  void )
{
// 	Requires LHS and Top mrgin
//  	Running count of labels across and down.
//		Number of Columns and Rows
// 		Label Size and Gap


	 float labelStartXmm,labelStartYmm,labelEndXmm,labelEndYmm;
	 float offsetX,offsetY;

	 labelCountX++;

// Work out the Offset from the LHS and Top
	 offsetX= (margins->getStartXmm() - unprintable->getStartXmm());
	 offsetY= (margins->getStartYmm() - unprintable->getStartYmm());

	 if( labelCountX >colRows->getSizeX() )
	 {
		 labelCountX=1;
		 labelCountY++;
	 }

	 labelCountTotal++;

// Work out the start of the label
	 labelStartXmm = ((labelCountX -1)*(labelSize->getSizeXmm() +labelGap->getSizeXmm() ))+ offsetX;
	 labelStartYmm = ((labelCountY -1)*(labelSize->getSizeYmm()+labelGap->getSizeYmm() ))+ offsetY;
// Work out the end of the label
	 labelEndXmm   = labelStartXmm + labelSize->getSizeXmm();
	 labelEndYmm   = labelStartYmm + labelSize->getSizeYmm();

// Store the label area.
	 labelArea->setStartXmm( labelStartXmm );
	 labelArea->setStartYmm( labelStartYmm );
	 labelArea->setEndXmm(   labelEndXmm );
	 labelArea->setEndYmm(   labelEndYmm );
}

void PSMSPage::clear(void )
{
		labelCountX=0;
		labelCountY=1;
		labelCountTotal=0;
}







//---------------------------------------------------------------------------

#pragma package(smart_init)

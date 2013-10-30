//---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop

#include "psprintr.h"
#include "psitem.h"
#include "pssymbol.h"
#include "pssize.h"
//---------------------------------------------------------------------------
PSItemBase::PSItemBase()
{
	checkFlag=true;
}
PSItemBase::~PSItemBase()
{
}
void PSItemBase::setLabelPi( PSRect* pLabelPi )
{
    labelPi=pLabelPi;
}

void PSItemText:: draw()
{
        int xA=labelPi->getStartX(); // start of label in pixels
        int yA=labelPi->getStartY(); // start of label in pixels
        int xB=labelPi->getEndX(); // start of label in pixels
        int yB=labelPi->getEndY(); // start of label in pixels

        int xR= ((xB-xA)* startXratio) + xA + pixelOffset->getSizeX();
        int yR= ((yB-yA)* startYratio) + yA + pixelOffset->getSizeY();

        bool centreText=false;
        p=pr->getCurrPrinter();

        p->Canvas->Font->Height=-(fontSize * pr->getDpi()) / 72;
        p->Canvas->Font->Color=clBlack;
        p->Canvas->Brush->Color=clWhite;
        p->Canvas->Font->Name="Arial";
        if(centreText)
        {
	        int	w = p->Canvas->TextWidth( content.c_str() );
                p->Canvas->TextOut( xR-w/2,yR,content.c_str());
        }else
        {
                p->Canvas->TextOut( xR,yR,content.c_str());
        }
}
void PSItemBitmap::draw(void)
{
		p=pr->getCurrPrinter();

        p->Canvas->Brush->Color=clBlack;

//		TRect	sourceRect( 1, 1, bm->Width, bm->Height );

        int xA=labelPi->getStartX(); // start of label in pixels
        int yA=labelPi->getStartY(); // start of label in pixels
        int xB=labelPi->getEndX(); // start of label in pixels
        int yB=labelPi->getEndY(); // start of label in pixels

        int xR= ((xB-xA)* startXratio) + xA+ pixelOffset->getSizeX();
        int yR= ((yB-yA)* startYratio) + yA+ pixelOffset->getSizeY();

        if( pr->getDpi() ==1200 )
        {
				TRect	stretchSourceRect( 1+xR, 1+yR, xR+(2* bm->Width), yR +(2* bm->Height ));
				p->Canvas->StretchDraw( stretchSourceRect, bm );
        }else
        {
                p->Canvas->Draw(xR,yR, bm );
        }




}
void PSItemRect::draw()
{
		int xA=labelPi->getStartX(); // start of label in pixels
		int yA=labelPi->getStartY(); // start of label in pixels
		int xB=labelPi->getEndX(); // start of label in pixels
		int yB=labelPi->getEndY(); // start of label in pixels

		int startXpi= ((xB-xA)* startXratio) + xA+ pixelOffset->getSizeX();
		int startYpi= ((yB-yA)* startYratio) + yA+ pixelOffset->getSizeY();
		int endXpi= ((xB-xA)* endXratio) + xA+ pixelOffset->getSizeX();
		int endYpi= ((yB-yA)* endYratio) + yA+ pixelOffset->getSizeY();

		p=pr->getCurrPrinter();

		p->Canvas->Brush->Color=clBlack;

		p->Canvas->Rectangle(startXpi,startYpi,endXpi,endYpi);

}
void PSItemFrameRect::draw()
{
		int xA=labelPi->getStartX(); // start of label in pixels
		int yA=labelPi->getStartY(); // start of label in pixels
		int xB=labelPi->getEndX(); // start of label in pixels
		int yB=labelPi->getEndY(); // start of label in pixels

		int startXpi= ((xB-xA)* startXratio) + xA+ pixelOffset->getSizeX();
		int startYpi= ((yB-yA)* startYratio) + yA+ pixelOffset->getSizeY();
		int endXpi= ((xB-xA)* endXratio) + xA+ pixelOffset->getSizeX();
		int endYpi= ((yB-yA)* endYratio) + yA+ pixelOffset->getSizeY();

		p=pr->getCurrPrinter();

		p->Canvas->Brush->Color=clBlack;

		TRect r = TRect( startXpi,startYpi,endXpi,endYpi);
		p->Canvas->FrameRect(r);

}


void PSItemBarcode::draw(void)
{

       VecBar aa;

       float xRangeRatio,yRangeRatio;
        p=pr->getCurrPrinter();

        xRangeRatio=fabs(endXratio-startXratio);
        yRangeRatio=fabs(endYratio-startYratio);

        if(xRangeRatio>=1){xRangeRatio=1;}
        if(xRangeRatio<=0){xRangeRatio=0;}

        if(yRangeRatio>=1){yRangeRatio=1;}
        if(yRangeRatio<=0){yRangeRatio=0;}


        BarcodeSymbology *bs;

        bs=BarcodeSymbology::getSymboliser();
		bs->encode(content,aa,startXmm,startYmm,xRangeRatio,yRangeRatio);

		float barStartRatio,barEndRatio;
        if(!bs->getBarcodeOrientation())
        {
                for ( unsigned int i=0;i<aa.size();i++)
                {
                        if(aa[i].inkOn==1)
                        {
                                barStartRatio=aa[i].xStart;
                                break;
                        }
                }
                for ( unsigned int i=(aa.size()-1);i>0;i--)
                {
                        if(aa[i].inkOn==1)
                        {
                                barEndRatio=aa[i].xEnd;
                                break;
                        }
                }
        }else
        {
                for ( unsigned int i=0;i<aa.size();i++)
                {
                        if(aa[i].inkOn==1)
                        {
                                barStartRatio=aa[i].yStart;
                                break;
                        }
                }
                for ( unsigned int i=(aa.size()-1);i>0;i--)
                {
                        if(aa[i].inkOn==1)
                        {
                                barEndRatio=aa[i].yEnd;
                                break;
                        }
                }
        }

        p->Canvas->Brush->Color=clBlack;

        int xA=labelPi->getStartX(); // start of label in pixels
        int yA=labelPi->getStartY(); // start of label in pixels
        int xB=labelPi->getEndX(); // start of label in pixels
        int yB=labelPi->getEndY(); // start of label in pixels


        float x1,x2,y1,y2;
        for( unsigned int i=0;i< aa.size();i++)
        {

                if( aa[i].inkOn )
                {
                x1=aa[i].xStart + startXratio;
                x2=aa[i].xEnd   + startXratio;
				y1=aa[i].yStart + startYratio;
                y2=aa[i].yEnd   + startYratio;

        int ixStart = ((xB-xA)* x1) + xA + pixelOffset->getSizeX();
        int iyStart = ((yB-yA)* y1) + yA + pixelOffset->getSizeY();
        int ixEnd   = ((xB-xA)* x2) + xA + pixelOffset->getSizeX();
        int iyEnd   = ((yB-yA)* y2) + yA + pixelOffset->getSizeY();


                        TRect r(ixStart,iyStart,ixEnd,iyEnd);
                        p->Canvas->FillRect(r);

                }
         }


// Printing text for bar code

            float xCR;

        bool centreText=true;
        int xR;
        if( centreText )
        {
            xCR=((barEndRatio-barStartRatio)/2) + (startXratio+barStartRatio);
			xR= ((xB-xA) * xCR ) + xA;
		}else
		{
				 xR= ((xB-xA)* startXratio) + xA;
		}
		int yR= ((yB-yA)* startYratio) + yA;

		p=pr->getCurrPrinter();

		p->Canvas->Font->Height=-(fontSize * pr->getDpi()) / 72;
		p->Canvas->Font->Color=clBlack;
		p->Canvas->Brush->Color=clWhite;
		p->Canvas->Font->Name="Arial";

		if( ! checkFlag )
		{
			int lenContent;
			lenContent=content.Length();
			if( lenContent >1 )
			{
				content= content.SubString(1,lenContent-1 );
			}
		}
		if(centreText)
		{
			int	w = p->Canvas->TextWidth( content.c_str() );
				p->Canvas->TextOut( xR-w/2,yR,content.c_str());
		}else
		{
				p->Canvas->TextOut( xR,yR,content.c_str());
		}



/*
		if(!bs->getBarcodeOrientation())
		{


			float textCentremm=labelRatioTommX(relativeCentreRatio);
			float textYmm;
			textYmm=labelRatioTommY(pyStartRatio);
			bool centreText=true;

			String textBarcodeWithCheck,textBarcode;
			textBarcodeWithCheck=bs->getLastBarcodeWithCheck(  );

			if(!visibleLastDigit)
			{
//                textBarcode= pBarcode.SubString(1,(pBarcode.Length()-1));
				textBarcode= textBarcodeWithCheck.SubString(1,(textBarcodeWithCheck.Length()-1));
			}else
			{
//                textBarcode= pBarcode;
				if(bs->getBarcodeCheckDigit())
                {
                        textBarcode= textBarcodeWithCheck;
                }else
                {
                        textBarcode=content;
                }
            }

            apsPrinter->textWord(textBarcode,textCentremm,textYmm, centreText);
        }else
        {

            float relativeCentreRatio;
// Centre of the barcode in the 'Y' direction
            relativeCentreRatio=((barEndRatio-barStartRatio)/2) + (pyStartRatio+barStartRatio);

// Place to locate text from the LHS of the label
            float textCentremm=labelRatioTommX(pxStartRatio);
            float textYmm;
// Place to locate text from the top of the label
            textYmm=labelRatioTommY(relativeCentreRatio);

            String textBarcode;
            if(!visibleLastDigit)
            {
                textBarcode= content.SubString(1,(pBarcode.Length()-1));
            }else
            {
                textBarcode= content;
            }

//            apsPrinter->textWord(textBarcode,textCentremm,textYmm, centreText);
            apsPrinter->barcode_text_word(textCentremm,textYmm,textBarcode,90,APSL_CENTRE);
        }*/
}

#pragma package(smart_init)



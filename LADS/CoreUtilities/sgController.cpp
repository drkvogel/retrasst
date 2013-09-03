//---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop

#include "sgController.h"

SgController::SgController( TStringGrid* sgBase )
{
        TColor fixedColor;
        sg=sgBase;

        GCGBase= new TSG_EXTRA2(sgBase);
        GSGBaseColor=sg->Color;

        fixedColor=sg->FixedColor;
		GCGBase->row_colour_set(0,fixedColor,clBlack);
		GCGBase->set_Highlight(clLime, clBlack );
		sgRowLastHL=-1;
		sgRowHL=-1;
		lastRowHighLight =-1;
		clearGrid();

//        buildDisplayOrderList( );
}
SgController::~SgController()
{
		delete GCGBase;
}
bool SgController::statusToShow( const RowItem& ss )
{
		return false;
}

void SgController::clearGrid(void )
{
		sg->RowCount=2;
        sgRow=1;
        lastRowHighLight=-1;
        blankRow();
        GCGBase->clearHighLight();
        GCGBase->clearTextWrap();
}
void SgController::blankRow( void )
{
    String Sstatus;
    Sstatus="";
    for ( unsigned int i=0; i< columnDisplayOrder.size();i++)
    {
        if( columnDisplayOrder[i].title!="Status")
        {
                sg->Cells[i][sgRow]      = "";
        }else
        {
                sg->Cells[i][sgRow]       = "";
        }
    }

	sg->Refresh();
}
void SgController::highlightRow( int highRow )
{
       if( highRow >-1 && highRow<sg->RowCount )
       {
           GCGBase->row_highlight_set(highRow);
       }
       refresh();
}
void SgController::highlightRowByTitle(int highRow,const std::string & dataSetTitle )
{

}

void SgController::toggleHighlightRow( int iRow )
{
        if( iRow >-1 && iRow<sg->RowCount )
        {
               GCGBase->row_highlight_toggle(iRow);
        }
        refresh();

}

void SgController::highlightOff(const  int unHighRow )
{
        if( unHighRow >-1 && unHighRow<sg->RowCount )
        {
               GCGBase->row_highlight_unset(unHighRow);
        }
        refresh();

}
void SgController::textWrapSet( int irow )
{
        if( irow > -1 && irow <sg->RowCount )
        {
			   GCGBase->row_textWrap_set( irow );
        }
        refresh();

}

void SgController::upRow( void )
{
        int a;
        if( sgRowLastHL>1 )
        {
                a=sgRowLastHL-1;
                highlightRow(a);
        }
}
void SgController::downRow( void )
{
        int a;
        if( sgRowLastHL<(sg->RowCount-1))
        {
                a=sgRowLastHL+1;
                highlightRow(a);
        }
}

void SgController::firstRow( void )
{
        int a;
        a=sg->RowCount-1;
        if( a > 1)
        {
                sg->Row=1;
                highlightRow(a);
        }
}
void SgController::lastRow( void )
{
        int a;
        a=sg->RowCount-1;
		sg->Row=a;
        highlightRow(a);
}
void SgController::refresh( void )
{
        sg->Refresh();
}

void SgController::DrawCell_event_handler(int ACol,int ARow,TRect &Rect)
{
        GCGBase->DrawCell_event_handler(ACol,ARow,Rect);
}


void SgController::drawRow(const RowItem& ri, const int pRow)
{
    String Sstatus;
    int colValueNumber;
    int ageOfSpecimen;
    TDateTime* specimenTaken;
        int colOffset;
    int iOffset;
    int noOfValue,noOfNonValue;


//  Specimen has been found

        for( unsigned int i=0;i<columnDisplayOrder.size();i++)
        {
            switch(i)
            {
                case 1: // Status is in second column list
                      Sstatus="non";
//                    decodeStatus(Sstatus,ri.getItemStatus(),highlight,textColour);
//                    sg->Cells[colStatus][pRow]=Sstatus;
//                    GCGBase->cell_colour_set(colStatus,pRow,highlight,textColour);
                break;
//                default:
//                     colValueNumber=columnDisplayOrder[i].fieldNumber;
			 }
		}
}

void SgController::drawSgRow(const RowItem& ri)
{
	GCGBase->row_colour_set( sgRow,GSGBaseColor,clBlack);
//    sg->ColCount=columnDisplayOrder.size();
	drawRow( ri, sgRow);
	sgRow++;
	sg->RowCount = sgRow;
}

void SgController::colourHighLight( TColor c )
{
		GCGBase->set_Highlight(c, clBlack );
}

int SgController::calcAge(const TDateTime earliest, const TDateTime latest )
{    int a;
     int b;
     a= (int) earliest;
     b= (int) latest;
	 return  ( b - a );
}
void SgController::heading( void )
{
        for( unsigned i=0;i<columnDisplayOrder.size();i++)
        {

                sg->ColWidths[i]=columnDisplayOrder[i].fieldPixelWidth;
				sg->Cells[i][0]=columnDisplayOrder[i].title;

        }

}


void SgController::getCell(const int col,const int row,std::string & pValue)
{
        if( row != -1 )
        {
				pValue=bcsToStd(sg->Cells[col][row]);
        }else
		{
                pValue="";
        }
}
// return false if no error
bool SgController::getCellAsInt(const int col,const int row, int& pValue )
{
        if( row >= 0 )
		{
                pValue=sg->Cells[col][row].ToIntDef(-1);
                return false;
        }else if ( row == -2 )
        {
// Special Case to return the row Count
                pValue=sg->RowCount;
                return false;
        }else
        {
				pValue=0;
                return true;
        }

}

void SgController::setCell(const int col,const int row, const std::string & pValue )
{
        if( row != -1 )
        {
				sg->Cells[col][row]=String(pValue.c_str());
        }else
        {
                sg->Cells[col][row]="";
        }

}

void SgController::getCurrentCell(const int col, std::string & pValue )
{
		pValue=bcsToStd(sg->Cells[col][sg->Row]);
}
int  SgController::getHighLightedRow( void )
{
        return -1;
}

//---------------------------------------------------------------------------

#pragma package(smart_init)

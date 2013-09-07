//---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop

#include "TSGSTL.h"
#include "StringUtil.h"
TColor  TSG_EXTRA2::defaultHighlight=clLime;
TColor  TSG_EXTRA2::defaultNormal=clWhite;

TColor  TSG_EXTRA2::defaultHighlightText=clBlack;


TSG_EXTRA2::TSG_EXTRA2( TStringGrid *tsg )
{
       grid.clear();
       col.clear();
       rowEntry.clear();
       rowEntryCount=0;
       highLightSet=false;

       canv=tsg->Canvas;

       canv->Font->Name=tsg->Font->Name;
       canv->Font->Size=tsg->Font->Size;
       sg=tsg;
}
TSG_EXTRA2::~TSG_EXTRA2( void )
{
       grid.clear();
       col.clear();
       rowEntryCount=0;
       rowEntry.clear();
//       delete grid;
//       delete col;
//       delete rowEntry;
}
//---------------------------------------------------------------------------
TColor TSG_EXTRA2::rgb_to_TColor( int red, int green, int blue )
{
	red   = abs( red )   % 256;	// ENSURE SENSIBLE +VE VALUES
	green = abs( green ) % 256;
	blue  = abs( blue )  % 256;
	return( (TColor)( red + ( 256 * ( green + ( 256 * blue ) ) ) ) );
}
//- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

//- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

void TSG_EXTRA2::cell_colour_set( int icol, int irow, TColor pc, TColor ptc )
{
        MapRowEntry::iterator mi=rowEntry.find(irow);
        cellAttribute lca;
        rowDetails rd;

// Has a row details been defined
        if( mi==rowEntry.end())
        {
// No
                detailedCells ldc;

                lca.c=pc;
                lca.tc=ptc;
                lca.defined=true;
                ldc.insert(detailedCells::value_type(icol,lca));

                lca.defined=false;
                rd.ca= lca;
                rd.dc=ldc;
                rd.highLighted=false;
                rd.textWrap=false;

                rowEntryCount++;
// Keep an account of sequencial colour changes against the row number
// A colour changed to a row is given sequential number which follows the row as it moves around the grid.
                rowEntry.insert( MapRowEntry::value_type(irow,rowEntryCount ) );
// Keep a container of row attributes mapped to the sequencial row counter marker.
                grid.insert( wholeGrid::value_type( rowEntryCount,rd) );
// row details not found
        }else
        {
// link between row number and sequential row change number found is there a cell defined?
                detailedCells::iterator ldc;

                int rowEntryi=mi->second;
// If there is a rowEntry element then there should be a entry in grid.
// else something is wrong
                wholeGrid::iterator gi=grid.find(rowEntryi);
// Looking for the detail of the cell
                ldc=gi->second.dc.find(icol);

                if( ldc== gi->second.dc.end())
                {
// Not found any record of individual cell settings
// set up an individuals cell's attribute since it does not exist
                        lca.c=pc;
                        lca.tc=ptc;
                        lca.defined=true;
                        gi->second.dc.insert(detailedCells::value_type(icol,lca));
                }else
                {
// redefine an individuals cell's attribute since it does exist
                      ldc->second.c=pc;
                      ldc->second.tc=ptc;
                      ldc->second.defined=true;
                }
        }
//        sg->Refresh();
}
//- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
/* SET COLOUR FOR WHOLE COLUMN */
void TSG_EXTRA2::col_colour_set( int icol, TColor pc,TColor ptc )
{
        detailedCells::iterator column=col.find(icol);
        cellAttribute ca;
        if( column == col.end())
        {
                cellAttribute ca;
                ca.c=pc;
                ca.tc=ptc;
                ca.defined=true;
                ca.textWrap=false;
                col.insert(detailedCells::value_type(icol,ca ));

        }else
        {
               ca.c=pc;
               ca.tc=ptc;
               ca.defined=true;
               ca.textWrap=false;
               column->second=ca;
        }
}
//- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
					/* SET COLOUR FOR WHOLE ROW */
void TSG_EXTRA2::row_colour_set( int irow, TColor pc ,TColor ptc)
{
        MapRowEntry::iterator mi=rowEntry.find(irow);

//        wholeGrid::iterator gi=grid.find(irow);
        cellAttribute lca;
        rowDetails rd;

        if( mi==rowEntry.end())
        {
// No details on the row found Create a link between the row number and
//a sequential row change count to enable colour changes in row swaps
                lca.c=pc;
                lca.tc=ptc;
                lca.defined=true;

                rd.ca= lca;
                rd.highLighted=false;
                rd.textWrap=false;
                rowEntryCount++;
                rowEntry.insert( MapRowEntry::value_type(irow,rowEntryCount ) );
                grid.insert( wholeGrid::value_type( rowEntryCount,rd) );
// row details not found
        }else
        {
            int rowEntryi=mi->second;
// If there is a rowEntry element then there should be a entry in grid.
// else something is wrong
            wholeGrid::iterator gi=grid.find(rowEntryi);
        // row details found is there a cell defined?
            gi->second.ca.c=pc;
            gi->second.ca.tc=ptc;
            gi->second.ca.defined=true;
        }
}
/* SET ROW TO BE HIGHLIGHTED AS IN SELECTED*/
void TSG_EXTRA2::row_highlight_set( int irow )
{
        MapRowEntry::iterator mi=rowEntry.find(irow);

//        wholeGrid::iterator gi=grid.find(irow);
        cellAttribute lca;
        rowDetails rd;

        if( mi==rowEntry.end())
        {
// No details on the row found Create a link between the row number and
//a sequential row change count to enable colour changes in row swaps
                if( !highLightSet )
                {
                        lca.c=defaultHighlight;
                        lca.tc=defaultHighlightText;
                }else
                {
                        lca.c=userHighlightColour;
                        lca.tc=userHighlightText;
                }
                lca.defined=true;

                rd.ca= lca;
                rd.textWrap=false;
                rd.highLighted=true;

                rowEntryCount++;
                rowEntry.insert( MapRowEntry::value_type(irow,rowEntryCount ) );
                grid.insert( wholeGrid::value_type( rowEntryCount,rd) );
// row details not found
        }else
        {
            int rowEntryi=mi->second;
// If there is a rowEntry element then there should be a entry in grid.
// else something is wrong
            wholeGrid::iterator gi=grid.find(rowEntryi);
        // row details found is there a cell defined?
            gi->second.highLighted=true;
        }
}
void TSG_EXTRA2::row_textWrap_set( int irow )
{
        row_textWrap_toggle( irow, true );
}
void TSG_EXTRA2::row_textWrap_unset( int irow )
{
        row_textWrap_toggle( irow, false );
}
/* SET ROW TO BE HIGHLIGHTED AS IN SELECTED*/
void TSG_EXTRA2::row_textWrap_toggle( int irow,bool pset )
{
        MapRowEntry::iterator mi=rowEntry.find(irow);

//        wholeGrid::iterator gi=grid.find(irow);
        cellAttribute lca;
        rowDetails rd;

        if( mi==rowEntry.end())
        {
// No details on the row found Create a link between the row number and
//a sequential row change count to enable colour changes in row swaps
                if( !highLightSet )
                {
                        lca.c=defaultHighlight;
                        lca.tc=defaultHighlightText;
                }else
                {
                        lca.c=userHighlightColour;
                        lca.tc=userHighlightText;
                }
                lca.defined=true;

                rd.ca= lca;
                rd.textWrap=true;
                rd.highLighted=false;

                rowEntryCount++;
                rowEntry.insert( MapRowEntry::value_type(irow,rowEntryCount ) );
                grid.insert( wholeGrid::value_type( rowEntryCount,rd) );
// row details not found
        }else
        {
            int rowEntryi=mi->second;
// If there is a rowEntry element then there should be a entry in grid.
// else something is wrong
            wholeGrid::iterator gi=grid.find(rowEntryi);
        // row details found is there a cell defined?
            gi->second.textWrap=true;
        }
}
bool TSG_EXTRA2::is_row_highlighted( int irow )
{
        MapRowEntry::iterator mi=rowEntry.find(irow);

//        cellAttribute lca;
        rowDetails rd;
        int rowEntryi;
        if( mi==rowEntry.end())
        {
                return false;
        }
        rowEntryi=mi->second;
        wholeGrid::iterator gi=grid.find(rowEntryi);
        // row details found is there a cell defined?
        if( gi != grid.end())
             return gi->second.highLighted;

        return false;

}
bool TSG_EXTRA2::is_row_textwrapped( int irow )
{
        MapRowEntry::iterator mi=rowEntry.find(irow);

        cellAttribute lca;
        rowDetails rd;
        int rowEntryi;
        if( mi==rowEntry.end())
        {
                return false;
        }
        rowEntryi=mi->second;
        wholeGrid::iterator gi=grid.find(rowEntryi);
        // row details found is there a cell defined?
        if( gi != grid.end())
             return gi->second.textWrap;

        return false;

}
/* SET ROW TO TAKE OFF HIGHLIGHTED AS IN SELECTED*/
void TSG_EXTRA2::row_highlight_unset( int irow )
{
        MapRowEntry::iterator mi=rowEntry.find(irow);

//        wholeGrid::iterator gi=grid.find(irow);
        cellAttribute lca;
        rowDetails rd;

        if( mi==rowEntry.end())
        {
// No details on the row found Create a link between the row number and
//a sequential row change count to enable colour changes in row swaps
                if( !highLightSet )
                {
                        lca.c=defaultHighlight;
                        lca.tc=defaultHighlightText;
                }else
                {
                        lca.c=userHighlightColour;
                        lca.tc=userHighlightText;
                }
                lca.defined=true;

                rd.ca= lca;
                rd.highLighted=false;

                rowEntryCount++;
                rowEntry.insert( MapRowEntry::value_type(irow,rowEntryCount ) );
                grid.insert( wholeGrid::value_type( rowEntryCount,rd) );
// row details not found
        }else
        {
            int rowEntryi=mi->second;
// If there is a rowEntry element then there should be a entry in grid.
// else something is wrong
            wholeGrid::iterator gi=grid.find(rowEntryi);
        // row details found is there a cell defined?
            gi->second.highLighted=false;
        }
}
void  TSG_EXTRA2::clearHighLight( void )
{
// Clear out all highlights
        for( wholeGrid::iterator gi= grid.begin();gi!= grid.end();gi++ )
        {
                gi->second.highLighted=false;
        }
}

void  TSG_EXTRA2::clearTextWrap( void )
{
// Clear out all highlights
        for( wholeGrid::iterator gi= grid.begin();gi!= grid.end();gi++ )
        {
                gi->second.textWrap=false;
        }
}

void TSG_EXTRA2::row_highlight_toggle( int irow )
{
        MapRowEntry::iterator mi=rowEntry.find(irow);

//        wholeGrid::iterator gi=grid.find(irow);
        cellAttribute lca;
        rowDetails rd;

        if( mi==rowEntry.end())
        {
// No details on the row found Create a link between the row number and
//a sequential row change count to enable colour changes in row swaps
                if( !highLightSet )
                {
                        lca.c=defaultHighlight;
                        lca.tc=defaultHighlightText;
                }else
                {
                        lca.c=userHighlightColour;
                        lca.tc=userHighlightText;
                }
                lca.defined=true;

                rd.ca= lca;
                rd.highLighted=true;

                rowEntryCount++;
                rowEntry.insert( MapRowEntry::value_type(irow,rowEntryCount ) );
                grid.insert( wholeGrid::value_type( rowEntryCount,rd) );
// row details not found
        }else
        {
            int rowEntryi=mi->second;
// If there is a rowEntry element then there should be a entry in grid.
// else something is wrong
            wholeGrid::iterator gi=grid.find(rowEntryi);
        // row details found is there a cell defined?
            gi->second.highLighted=!gi->second.highLighted;
        }
}


void   TSG_EXTRA2::cell_colour_set( int icol, int irow, TColor c )
{
        cell_colour_set(icol,irow,c,clBlack);
}
void   TSG_EXTRA2::col_colour_set( int icol, TColor c )
{
        cell_colour_set(icol,c,clBlack);
}
void   TSG_EXTRA2::row_colour_set( int irow, TColor c )
{
        row_colour_set(irow,c,clBlack);
}
void   TSG_EXTRA2::row_swap( int irowto, int irowfrom )
{
        rowDetails x,y;
        wholeGrid::iterator gifrom=grid.find(irowfrom);
        wholeGrid::iterator gito=grid.find(irowto);
        x=gifrom->second;
        y=gito->second;
        gifrom->second=y;
        gito->second=x;
}
void    TSG_EXTRA2::row_reshuffle( int irowto, int irowFrom )
{
        MapRowEntry tmpRowEntry;
        int rowEntryI;
// retreive entry
// add new entry
        tmpRowEntry.clear();
        if( irowFrom>irowto )
        {
// Move row up the grid
            for (MapRowEntry::iterator mi= rowEntry.begin();mi!=rowEntry.end();mi++)
            {

                if(mi->first >= irowto && mi->first<irowFrom)
                {

                        rowEntryI=mi->first;
                        rowEntryI++;
                        tmpRowEntry.insert( MapRowEntry::value_type(rowEntryI,mi->second ) );
                }else if(mi->first < irowto || mi->first>irowFrom )
                {

                        rowEntryI=mi->first;
                        tmpRowEntry.insert( MapRowEntry::value_type(rowEntryI,mi->second ) );
                } else
                {
                        tmpRowEntry.insert( MapRowEntry::value_type(irowto,mi->second ) );
                }

            }
        }else{
// Moving a row down the grid.
            for (MapRowEntry::iterator mi= rowEntry.begin();mi!=rowEntry.end();mi++)
            {
// Are we on rows
                if(mi->first <= irowto && mi->first>irowFrom)
                {

                        rowEntryI=mi->first;
                        rowEntryI--;
                        tmpRowEntry.insert( MapRowEntry::value_type(rowEntryI,mi->second ) );
                }else if(mi->first > irowto || mi->first<irowFrom )
                {

                        rowEntryI=mi->first;
                        tmpRowEntry.insert( MapRowEntry::value_type(rowEntryI,mi->second ) );
                } else
                {
                        tmpRowEntry.insert( MapRowEntry::value_type(irowto,mi->second ) );
                }
            }
        }
//        tmpRowEntry.insert( MapRowEntry::value_type(irowto,moveEntry ) );
        rowEntry.clear();
        rowEntry=tmpRowEntry;

}
void TSG_EXTRA2::set_Highlight ( TColor c, TColor tc )
{
        userHighlightColour=c;
        userHighlightText=tc;
        highLightSet=true;

}
//- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
			/* LOOK FOR COLOUR BY PRIORITY CELL > ROW > COLUMN */
cellAttribute& TSG_EXTRA2::cell_colour_get( int icol, int irow )
{
        static cellAttribute defaultColour;
        static cellAttribute ca;
// See if the cell is coloured first
// See if the row is coloured second
// See if the col is coloured third;

        ca.c=rgb_to_TColor(0,0,0);;
        ca.tc=rgb_to_TColor(0,0,0);;
        ca.defined=false;
        ca.textWrap=false;

        MapRowEntry::iterator mi=rowEntry.find(irow);

        if( mi==rowEntry.end())
        {
// No entry for the row has been found look to see idf their is a colour for the column
                detailedCells::iterator dci;
                dci=col.find(icol );

                if(dci==col.end())
                {
// no color has been found therefore return default
                        defaultColour.c=rgb_to_TColor(0,0,0);
                        defaultColour.textWrap=false;
                        return defaultColour;
                }else
                {
// A column color has been found therfore return column color
                        return dci->second;
                }
        }else{
// Row entry has been found retrieve sequential row change count
            int rowEntryi=mi->second;

//find corresponding row
            wholeGrid::iterator gi=grid.find(rowEntryi);

// Is this row highlighted if so over ride any existing predefined colors on this row
            if( gi->second.highLighted )
            {

                if( !highLightSet )
                {
                        ca.c=defaultHighlight;
                        ca.tc=defaultHighlightText;
                }else
                {
                        ca.c=userHighlightColour;
                        ca.tc=userHighlightText;
                        ca.defined=true;
                }
            }else
            {
// There is no highlighting look for a color on the call
                detailedCells::iterator ldc;
                ldc=gi->second.dc.find(icol);

                if( ldc== gi->second.dc.end())
                {
// there is a color for the row but no column details
                        ca= gi->second.ca;
                } else
                {
                        ca= ldc->second;
                }
            }
            ca.textWrap=gi->second.textWrap;
            return ca;

        }
}

//- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
#undef	abs	// USE FUNCTION NOT MACRO
//- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
				/* ASSUME ICOL, IROW >= 0 */
void TSG_EXTRA2::DrawCell_event_handler( int icol, int irow, TRect &rect )
{
        if( irow ==1 && icol ==1 && sg->Cells[1][0]== "Status" )
        {
//                int i;
//				i=i;
        }

	cellAttribute thisCell = cell_colour_get( icol, irow );
        if(!thisCell.defined )
        {
          canv->Brush->Color =    sg->Color;
        }else
        {
          canv->Brush->Color =  thisCell.c ;
        }

	canv->FillRect( rect );
        if(!thisCell.defined )
        {
	        canv->Font->Color = clBlack;
        }else
        {
                canv->Font->Color = thisCell.tc;
        }
        if ( !thisCell.textWrap )
        {
	canv->TextOut( rect.Left + 1,
		( rect.Bottom + rect.Top - abs( canv->Font->Height ) ) / 2,
		sg->Cells[icol][irow] );
        }else
        {
                int noOfLines;
				String textToWrap;
				std::string strWrap;
				char buff[256];
				textToWrap=sg->Cells[icol][irow];
				strWrap=bcsToStd( textToWrap);
				strcpy(buff,strWrap.c_str());
                noOfLines=rect.Height()/canv->Font->Height;
                 textToWrap=String(strtok(buff," ,\r\n"));
                for( int i=0; i<3; i++ )
                {


                        if( textToWrap.Length()>0)
                	canv->TextOut( rect.Left + 1,
//		        ( rect.Bottom + rect.Top )/2 - abs( canv->Font->Height ) ) / 2,
//		         rect. + (abs( canv->Font->Height ) *i) ,
//		        ( rect.Bottom + rect.Top )/2 - (abs( canv->Font->Height )  / 2),
		          rect.Top  + (abs( canv->Font->Height ) *i ) +1,
        		 textToWrap);
                         textToWrap=String(strtok(NULL," ,\r\n"));
                }
        }
}
//---------------------------------------------------------------------------

#pragma package(smart_init)

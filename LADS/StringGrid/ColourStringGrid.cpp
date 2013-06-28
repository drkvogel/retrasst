//---------------------------------------------------------------------------
#include <vcl.h>
#include <stdlib.h>
#include <map>

#pragma hdrstop

#include "ColourStringGrid.h"
#pragma package(smart_init)

//---------------------------------------------------------------------------
// ValidCtrCheck is used to assure that the components created do not have
// any pure virtual functions.

static inline void ValidCtrCheck(TColourStringGrid *)
{
    new TColourStringGrid(NULL);
}
//---------------------------------------------------------------------------

/* Constructor - do not call directly, but install the ColourStringGrid */
/* as a VCL component in C++ Builder */

__fastcall TColourStringGrid::TColourStringGrid(TComponent* Owner)
    : TStringGrid(Owner)
{
    if(colRowCount == 0)
    {
        colRowCount = 5;
        FRowCount = 5;
    }

    if(colColCount == 0)
    {
        colColCount = 5;
        FColCount = 5;
    }

     FFirstBitmapLeft = 92;

     theColourGrid = (int **) malloc( colColCount * sizeof(int *) );

     for (int i = 0; i < colColCount; i++ )
        theColourGrid[i] = (int *) malloc( colRowCount * sizeof(int) );

     for(int x = 0; x < colColCount; x++)
		 for(int y = 0; y < colRowCount; y++)
			theColourGrid[x][y] = 0;
}

/* Destructor - do not call directly */

__fastcall TColourStringGrid::~TColourStringGrid()
{
    for(int x = 0; x < FColCount; x++)
		for(int y = 0; y < FRowCount; y++)
			if(theColourGrid[x][y])
				delete (CellProperties*)theColourGrid[x][y];

	for(int x = 0; x < FColCount; x++)
		free( theColourGrid[x] );

	free( theColourGrid );
}


/* Message Map Code */

void __fastcall TColourStringGrid::CMKillFocus(TMessage &Message)
{
}

/* Use SetColCount to safely resize a ColourStringGrid by increasing or decreasing the number of columns */
/* Alternatively, set the integer property, TColourStringGrid::ColCount directly. */

void __fastcall TColourStringGrid::SetColCount(int value)
{
	// can't set columns to less than 1
	if(value <= 0)
		return;

	if(value > FColCount)
	{
		GrowGrid(FRowCount,FRowCount,FColCount,value);
		TStringGrid::ColCount = value;
		FColCount = value;
	}
	else if(value < FColCount)
	{
	   ShrinkGrid(FRowCount,FRowCount,FColCount,value);
	   TStringGrid::ColCount = value;
	   FColCount = value;
	}
}

void __fastcall TColourStringGrid::SetRoundRect(bool isRoundRect)
{
   FRoundRect = isRoundRect;
}

void __fastcall TColourStringGrid::SetOutlineCell( bool oc )
{
   FOutlineCell = oc;
}

void __fastcall TColourStringGrid::SetRoundRectX(int RoundRectX)
{
   FRoundRectX = RoundRectX;
}

void __fastcall TColourStringGrid::SetRoundRectY(int RoundRectY)
{
   FRoundRectY = RoundRectY;
}

/* Use SetRowCount to safely resize a ColourStringGrid by increasing or decreasing rows. */
/* Alternatively, set the integer property, TColourStringGrid::ColCount directly. */

void __fastcall TColourStringGrid::SetRowCount(int value)
{
	if(value <= 0)
		return;

	if(value > FRowCount)
	{
		GrowGrid(FRowCount,value,FColCount,FColCount);
		TStringGrid::RowCount = value;
		FRowCount = value;
	}
	else if(value < FRowCount)
	{
		ShrinkGrid(FRowCount,value,FColCount,FColCount);
		TStringGrid::RowCount = value;
		FRowCount = value;
	}
}

// insert a blank row at row
void __fastcall TColourStringGrid::InsertRow(int row)
{
	   if(row < FixedRows)
		return;

	   SetRowCount(FRowCount + 1);

	   for(int lastrow, cols = 0; cols < FColCount; cols++)
	   {
			for(lastrow = FRowCount - 1;lastrow > row; lastrow--)
			{
				Cells[cols][lastrow] = Cells[cols][lastrow - 1];
				theColourGrid[cols][lastrow] = theColourGrid[cols][lastrow - 1];
			}
			Cells[cols][lastrow] = "";
			theColourGrid[cols][lastrow] = NULL;
		}
}

// copy source_row to dest_row and expand grid by 1 row
void __fastcall TColourStringGrid::CopyRow(int source_row, int dest_row)
{
	   if(source_row < FixedRows || source_row >= FRowCount)
			return;

	   if(dest_row < FixedRows || dest_row >= FRowCount)
			return;

       InsertRow(dest_row);

       if(source_row < dest_row)
       {
            for(int cols = FixedCols; cols < FColCount; cols++)
            {
              Cells[cols][dest_row] = Cells[cols][source_row];

              if(theColourGrid[cols][source_row])
              {
                theColourGrid[cols][dest_row] =
                    (int)new CellProperties(*(CellProperties*)theColourGrid[cols][source_row]);

              }
            }
       }
       else
       {
            for(int cols = FixedCols; cols < FColCount; cols++)
            {
              Cells[cols][dest_row] = Cells[cols][source_row + 1];

              if(theColourGrid[cols][source_row + 1])
              {
                theColourGrid[cols][dest_row] =
                    (int)new CellProperties(*(CellProperties*)theColourGrid[cols][source_row + 1]);
              }
            }
        }
}

// delete row
void __fastcall TColourStringGrid::DeleteRow(int row)
{

        if(row < FixedRows || row >= FRowCount)
            return;

        for(int cols = 0; cols < FColCount; cols++)
        {
            if(theColourGrid[cols][row])
            {
                delete (CellProperties*)theColourGrid[cols][row];
                theColourGrid[cols][row] = 0;
            }

            for(int firstrow = row; firstrow < FRowCount; firstrow++)
            {
                if((firstrow+1) < FRowCount)
                {
                    Cells[cols][firstrow] = Cells[cols][firstrow + 1];
                    theColourGrid[cols][firstrow] = theColourGrid[cols][firstrow + 1];
                    theColourGrid[cols][firstrow + 1] = 0;
                }
                else
                {
                    // last row - simply delete (colours already deleted above)
                    Cells[cols][firstrow] = "";
                }
            }
        }

        SetRowCount(FRowCount - 1);
}

/* Use SetCellColour to set the colour of an individual cell */

void __fastcall TColourStringGrid::SetCellColour(int ACol, int ARow, TColor tc)
{

       if(FRowCount <= ARow || FColCount <= ACol)
       {
        return;
        }

       CellProperties* cp;
       if(theColourGrid[ACol][ARow])
       {
         cp = (CellProperties*)theColourGrid[ACol][ARow];
         cp -> SetCellColour(tc);
       }
       else
       {
         cp = new CellProperties;
         cp -> SetCellColour(tc);
         theColourGrid[ACol][ARow] = (int) cp;
       }
       Refresh();
}


void __fastcall TColourStringGrid::SetFontUnderline(int ACol, int ARow )
{

   if(FRowCount <= ARow || FColCount <= ACol)
   {
      return;
   }

   CellProperties* cp;
   if(theColourGrid[ACol][ARow])
   {
      cp = (CellProperties*)theColourGrid[ACol][ARow];
      cp -> SetFontUnderline();
   }
   else
   {
      cp = new CellProperties;
      cp -> SetFontUnderline();
      theColourGrid[ACol][ARow] = (int) cp;
   }
   Refresh();
}


bool __fastcall TColourStringGrid::GetFontUnderline(int ACol, int ARow )
{

    if(FRowCount <= ARow || FColCount <= ACol)
    {
        return false;
    }

    if(theColourGrid[ACol][ARow] != 0)
    {
        CellProperties* cp = (CellProperties*)theColourGrid[ACol][ARow];
        if( cp -> GetFontUnderlineSet() )
        {
            return (cp -> GetFontUnderline());
        }
    }

    return false;
}

//  keep a cache of bitmaps; return pointer to existing copy if available

Graphics::TBitmap* __fastcall TColourStringGrid::IconFromName(AnsiString icon_file)
{
   typedef std::map< String, Graphics::TBitmap* > BitMapMap;
   static BitMapMap bitmaps;

   BitMapMap::const_iterator found = bitmaps.find( icon_file );
   if( found != bitmaps.end() )
       return found -> second;

   Graphics::TBitmap* b = new Graphics::TBitmap;
   b -> LoadFromFile( icon_file );
   bitmaps.insert( BitMapMap::value_type( icon_file, b ) );
   return b;
}

void __fastcall TColourStringGrid::AddIcon(int ACol, int ARow, AnsiString icon_file)
{
    if(FRowCount <= ARow || FColCount <= ACol)
        return;

    Graphics::TBitmap* b = IconFromName( icon_file );

       CellProperties* cp;
       if(theColourGrid[ACol][ARow])
       {
         cp = (CellProperties*)theColourGrid[ACol][ARow];
         cp -> AddBitmap(b, icon_file);
       }
       else
       {
         cp = new CellProperties;
         cp -> AddBitmap(b, icon_file);
         theColourGrid[ACol][ARow] = (int) cp;
       }
       Refresh();
}

void __fastcall TColourStringGrid::RemoveIcon(int ACol, int ARow, AnsiString icon_file)
{

       if(FRowCount <= ARow || FColCount <= ACol)
       {
        return;
        }

       CellProperties* cp;
       if(theColourGrid[ACol][ARow])
       {
         cp = (CellProperties*)theColourGrid[ACol][ARow];
         cp -> RemoveBitmap(icon_file);
         if(cp -> CanDelete())
         {
            delete cp;
            theColourGrid[ACol][ARow] = 0;
         }
       }
}


void __fastcall TColourStringGrid::RemoveIcons(int ACol, int ARow)
{

       if(FRowCount <= ARow || FColCount <= ACol)
       {
        return;
        }

       CellProperties* cp;
       if(theColourGrid[ACol][ARow])
       {
         cp = (CellProperties*)theColourGrid[ACol][ARow];
         cp -> RemoveBitmaps();
         if(cp -> CanDelete())
         {
            delete cp;
            theColourGrid[ACol][ARow] = 0;
         }
       }
}

bool __fastcall TColourStringGrid::IsIconDisplayed(int ACol, int ARow, AnsiString icon_file)
{

    if(FRowCount <= ARow || FColCount <= ACol)
        return false;

       CellProperties* cp;
       if(theColourGrid[ACol][ARow])
       {
         cp = (CellProperties*)theColourGrid[ACol][ARow];
         return cp -> IsBitmapDisplayed(icon_file);

       }

       return false;
}


void __fastcall TColourStringGrid::SetFontColour(int ACol, int ARow, TColor tc)
{
       if(FRowCount <= ARow || FColCount <= ACol)
       {
        return;
        }

       CellProperties* cp;
       if(theColourGrid[ACol][ARow])
       {
         cp = (CellProperties*)theColourGrid[ACol][ARow];
         cp -> SetFontColour(tc);
       }
       else
       {
         cp = new CellProperties;
         cp -> SetFontColour(tc);
         theColourGrid[ACol][ARow] = (int) cp;
       }
       Refresh();
}



/* Use DelCellColour to reset the colour of an individual cell to no colour*/

void __fastcall TColourStringGrid::DelCellColour(int ACol, int ARow)
{
    if(FRowCount <= ARow || FColCount <= ACol)
        return;

       if(theColourGrid[ACol][ARow])
       {
            CellProperties* cp = (CellProperties*)theColourGrid[ACol][ARow];
            cp -> DelCellColour();
            if(cp -> CanDelete())
            {
               delete cp;
               theColourGrid[ACol][ARow] = 0;
            }
       }

       Refresh();
}

void __fastcall TColourStringGrid::DelFontUnderline(int ACol, int ARow)
{
   if(FRowCount <= ARow || FColCount <= ACol)
        return;

       if(theColourGrid[ACol][ARow])
       {
            CellProperties* cp = (CellProperties*)theColourGrid[ACol][ARow];
            cp -> DelFontUnderline();
            if(cp -> CanDelete())
            {
               delete cp;
               theColourGrid[ACol][ARow] = 0;
            }
       }

       Refresh();
}



void __fastcall TColourStringGrid::DelFontColour(int ACol, int ARow)
{
   if(FRowCount <= ARow || FColCount <= ACol)
        return;

       if(theColourGrid[ACol][ARow])
       {
            CellProperties* cp = (CellProperties*)theColourGrid[ACol][ARow];
            cp -> DelFontColour();
            if(cp -> CanDelete())
            {
               delete cp;
               theColourGrid[ACol][ARow] = 0;
            }
       }

       Refresh();
}


/* Use GetCellColour to get the colour of an individual cell. */
/* Returns true if the cell has a colour set, in which case tc will be set to the same Color */
/* Otherwise, returns false. */

BOOL __fastcall TColourStringGrid::GetCellColour(int ACol, int ARow, TColor& tc)
{
    if(FRowCount <= ARow || FColCount <= ACol)
        return false;

    if(theColourGrid[ACol][ARow] != 0)
    {
        CellProperties* cp = (CellProperties*)theColourGrid[ACol][ARow];
        if(cp -> GetCellColourSet())
        {
         tc = cp -> GetCellColour();
         return true;
        }
        else
        {
         return false;
        }
    }

    return false;
}


BOOL __fastcall TColourStringGrid::GetFontColour(int ACol, int ARow, TColor& tc)
{
    if(FRowCount <= ARow || FColCount <= ACol)
        return false;

    if(theColourGrid[ACol][ARow] != 0)
    {
        CellProperties* cp = (CellProperties*)theColourGrid[ACol][ARow];
        if(cp -> GetFontColourSet())
        {
         tc = cp -> GetFontColour();
         return true;
        }
        else
        {
         return false;
        }
    }

    return false;

}


/* Use SetColColour to set the colour of all the cells in a column (except any fixed cells) */

void __fastcall TColourStringGrid::SetColColour(int ACol, TColor tc)
{
   for(int x = FixedRows; x < FRowCount; x++)
	  SetCellColour(ACol,x,tc);
}

void __fastcall TColourStringGrid::DelColColour(int ACol)
{
   for(int x = FixedRows; x < FRowCount; x++)
	  DelCellColour(ACol,x);
}

/* Use SetRowColour to set the colour of all the cells in a row  */
void __fastcall TColourStringGrid::SetRowColour(int ARow, TColor tc)
{
   for(int x = FixedCols; x < FColCount; x++)
	  SetCellColour(x,ARow,tc);
}

void __fastcall TColourStringGrid::DelRowColour(int ARow)
{
   for(int x = FixedCols; x < FColCount; x++)
	  DelCellColour(x,ARow);
}

/* Use GetColColour to get the colour of a complete column. */
/* Returns true if the cell has a colour set, in which case tc will be set to the same Color */
/* Otherwise, returns false. */

BOOL __fastcall TColourStringGrid::GetColColour(int ACol, TColor& tc)
{
	TColor prev( 0 );
	for(int x = FixedRows; x < FRowCount; x++)
	{
		if( !GetCellColour( ACol, x, tc ) )
			return false;

		else if( prev == 0 )
			prev = tc;

		else if( prev != tc )
			return false;
	}
	return true;
}

/* Use GetRowColour to get the colour of a complete row. */
/* Returns true if the cell has a colour set, in which case tc will be set to the same Color */
/* Otherwise, returns false. */

BOOL __fastcall TColourStringGrid::GetRowColour(int ARow, TColor& tc)
{
	TColor prev( 0 );
	for( int x = FixedCols; x < FColCount; x++ )
	{
		if( !GetCellColour( x, ARow, tc ) )
			return false;

		else if( prev == 0 )
			prev = tc;

		else if( prev != tc )
			return false;
	}
	return true;
}

/* GrowGrid is Private to TColourStringGrid, and cannot be called externally */

void __fastcall TColourStringGrid::GrowGrid(int oldRows, int newRows,int oldCols, int newCols)
{
        theColourGrid = (int **) realloc( theColourGrid, newCols * sizeof(int *) );

	    for ( int i = 0; i < oldCols; i++ )
		{
            theColourGrid[i] = (int *) realloc( theColourGrid[i], newRows * sizeof(int) );
		}

	    for ( int i = oldCols; i < newCols; i++ )
		{
            theColourGrid[i] = (int *) malloc( newRows * sizeof(int) );
		}

    	for ( int i =  0; i < oldCols; i++ )
		{
            for ( int j = oldRows; j < newRows; j++ )
			{
                theColourGrid[i][j] = 0;
			}
		}

        for ( int i =  oldCols; i < newCols; i++ )
		{
            for ( int j = 0; j < newRows; j++ )
			{
				theColourGrid[i][j] = 0;
			}
		}
}

/* ShrinkGrid is Private to TColourStringGrid, and cannot be called externally */

void __fastcall TColourStringGrid::ShrinkGrid(int oldRows, int newRows,int oldCols, int newCols)
{
      for (int x = newCols; x < oldCols; x++)
      {
         for ( int y = 0; y < oldRows; y++ )
         {
            delete (CellProperties*)theColourGrid[x][y];
         }
      }

      for (int x = newRows; x < oldRows; x++)
      {
         for ( int y = 0; y < newCols; y++ )
         {
            delete (CellProperties*)theColourGrid[y][x];
         }
      }

      theColourGrid = (int **) realloc( theColourGrid, newCols * sizeof(int *) );

	   for ( int i = 0; i < newCols; i++ )
		{
            theColourGrid[i] = (int *) realloc( theColourGrid[i], newRows * sizeof(int) );
		}

}


/* DrawCell should not be called externally. */

void __fastcall TColourStringGrid::DrawCell(int ACol, int ARow, const TRect &ARect,
                    TGridDrawState AState)
{

        TStringGrid::DrawCell(ACol,ARow,ARect,AState);

        if(theColourGrid[ACol][ARow] != 0)
        {
           CellProperties* cp = (CellProperties*)theColourGrid[ACol][ARow];
           if( cp -> GetCellColourSet() )
           {
              Canvas -> Brush -> Color = cp -> GetCellColour();

              if( !FOutlineCell )
               Canvas -> Pen -> Color = cp -> GetCellColour();
              else
               Canvas -> Pen -> Color = clBlack;

              if(FRoundRect)
              {
                  Canvas -> RoundRect(ARect.Left,ARect.Top,ARect.Right,ARect.Bottom,RoundRectX, RoundRectY);
               }
               else
               {
                  int r = ARect.Right;
                  int b = ARect.Bottom;
                  int lft = ARect.Left;
                  int t = ARect.Top;

                  Canvas -> Rectangle(lft,t,r,b);
               }
            }

           if (cp -> GetFontColourSet())
           {
               Canvas -> Font -> Color = cp -> GetFontColour();
           }


            int next_bitmap_left = FFirstBitmapLeft;
            for( CellProperties::BitMapRange bitmap = cp -> GetBitmaps(); bitmap; ++ bitmap )
            {
               Canvas -> Draw( ARect.Left + next_bitmap_left, ARect.Top + 2 , *bitmap );
               next_bitmap_left += 21;
            }
            Canvas -> TextOut(ARect.Left+2,ARect.Top+3,Cells[ACol][ARow]);
            Canvas -> Refresh();
        }

}

void TColourStringGrid::ClearAll()
{
   for ( int x = 0; x < FColCount; x++ )
   {
      for ( int y = 0; y < FRowCount; y++ )
      {
         Cells[x][y] = "";
      }
   }

   for(int x = 0; x < FColCount; x++)
   {
      for(int y = 0; y < FRowCount; y++)
      {
         if(theColourGrid[x][y])
         {
            delete (CellProperties*)theColourGrid[x][y];
            theColourGrid[x][y] = 0;
         }
      }
    }
}

//---------------------------------------------------------------------------

namespace Colourstringgrid
{
    void __fastcall PACKAGE Register()
    {
        TComponentClass classes[1] = {__classid(TColourStringGrid)};
        RegisterComponents("Samples", classes, 0);
    }
}

//---------------------------------------------------------------------------

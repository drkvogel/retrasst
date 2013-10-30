//---------------------------------------------------------------------------

#ifndef TSGnewH
#define TSGnewH
#include <map>
#include <grids.hpp>
//#include "stdproj.h"
// defines an attribute which may be used for a single cell, or a row.
struct cellAttribute
{
        TColor c;
        TColor tc;
        bool defined;
        bool textWrap;

};
// detailedcells is used  to define individual cells whose attributes
// are different to the color of the row

typedef std::map< int,cellAttribute > detailedCells;

// defines details of a complete row
struct  rowDetails
{
     detailedCells dc; // individual cell colors of a row Key is the column
     cellAttribute ca; // Standard color of a row
     bool highLighted;
     bool textWrap;
};


// defines the whole grid where there are n rows the key is the row
typedef  std::map< int,rowDetails > wholeGrid;
typedef  std::map< int , int > MapRowEntry;

class TSG_EXTRA2
{
        private:
                detailedCells   col; //keep note of colors of each col
                wholeGrid       grid;
                MapRowEntry     rowEntry;
                int rowEntryCount;
                TStringGrid     *sg;
	        TCanvas		*canv;
                static TColor  defaultHighlight;
                static TColor  defaultNormal;
                static TColor  defaultHighlightText;
                TColor  userHighlightColour;
                TColor  userHighlightText;

                bool    highLightSet;
        public:
	TSG_EXTRA2( TStringGrid *tsg );
	~TSG_EXTRA2( void );
	static	TColor	rgb_to_TColor( int red, int green, int blue );
        void    set_Highlight ( TColor c, TColor tc );

	void	cell_colour_set( int icol, int irow, TColor c,TColor tc );
	void	col_colour_set( int icol, TColor c, TColor tc );
	void	row_colour_set( int irow, TColor c, TColor tc );
        void    row_highlight_set( int irow );
        bool    is_row_highlighted( int irow );
        bool    is_row_textwrapped( int irow );

        void    row_highlight_toggle(int irow);
        void    row_highlight_unset( int irow );
        void    row_textWrap_set( int irow );
        void    row_textWrap_unset( int irow );
        void    row_textWrap_toggle( int irow,bool pset );

        void    clearHighLight( void );
        void    clearTextWrap( void );
	void	cell_colour_set( int icol, int irow, TColor c );
	void	col_colour_set( int icol, TColor c );
	void	row_colour_set( int irow, TColor c );
        void    row_swap( int irowto, int irowfrom );
        void    row_reshuffle( int irowto, int irowfrom );

	cellAttribute& cell_colour_get( int icol, int irow );
	void	DrawCell_event_handler( int icol, int irow, TRect &rect );

};
//---------------------------------------------------------------------------
#endif

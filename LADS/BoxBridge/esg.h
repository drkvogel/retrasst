//---------------------------------------------------------------------------
/*		 EXTENDED STRING GRID		*/
//---------------------------------------------------------------------------
#include <vcl.h>
#include <Grids.hpp>
//---------------------------------------------------------------------------
#ifndef ESG_H_DEFINED
#define ESG_H_DEFINED
//---------------------------------------------------------------------------
class ESG;
class ESG_ARRAY;
//---------------------------------------------------------------------------
class ESG_VAL
{
friend class ESG;
friend class ESG_ARRAY;
private:
	ESG_VAL( void );
	ESG_VAL( const TColor bg, const TColor fg );
	~ESG_VAL( void ) {};
	bool	bg_colour_defined;
	bool	fg_colour_defined;
	TColor	bg_colour_val;
	TColor	fg_colour_val;
	void	setBColour( const TColor c );		// BACKGROUND COLOUR
	void	setFColour( const TColor c );		// FOREGROUND COLOUR
public:
};
//---------------------------------------------------------------------------

class ESG_ARRAY
{
private:
	int	nval;
	ESG	*owner;
	void	extend( const int new_size );
public:
	ESG_VAL	**val;
	ESG_ARRAY( ESG *e );
	~ESG_ARRAY( void );
	ESG_VAL *getVal( const int pos );
	ESG_VAL *returnVal( const int pos );
};
//---------------------------------------------------------------------------
class ESG_P
{
private:
	int	icol;
	int	irow;
	ESG	*owner;
public:
	ESG_P( ESG *e, const int ic, const int ir );
	~ESG_P( void ){};
	TColor	getBColour( void ) const;
	TColor	getFColour( void ) const;
	void	setBColour( const TColor c );
	void	setFColour( const TColor c );
	__property	TColor bColour =
		{ read=getBColour, write=setBColour};
	__property	TColor fColour =
		{ read=getFColour, write=setFColour};
};
//---------------------------------------------------------------------------
class ESG_C
{
private:
	int	icol;
	ESG	*owner;
public:
	ESG_C( ESG *e, const int ic );
	~ESG_C( void ){};
	ESG_P operator[]( const int ir );
};
//---------------------------------------------------------------------------
class ESG_CELL
{
friend class ESG;
private:
	ESG	*owner;
	ESG_CELL( ESG *e );
	~ESG_CELL( void ) {};
public:
	ESG_C	operator[]( const int ic );
};
//---------------------------------------------------------------------------
class ESG_ROW
{
friend class ESG;
private:
	ESG	*owner;
	ESG_ROW( ESG *e );
	~ESG_ROW( void ) {};
public:
	ESG_P	operator[]( const int ir );
};
//---------------------------------------------------------------------------
class ESG_COL
{
friend class ESG;
private:
	ESG	*owner;
	ESG_COL( ESG *e );
	~ESG_COL( void ) {};
public:
	ESG_P	operator[]( const int ic );
};
//---------------------------------------------------------------------------

class ESG
{
friend class ESG_P;
friend class ESG_CELL;
private:
	TStringGrid	*host;
	TCanvas		*canv;
	int	ncols;
	ESG_ARRAY	*col;		// VALUES FOR WHOLE COLUMNS
	ESG_ARRAY	*row;		// VALUES FOR WHOLE ROWS
	ESG_ARRAY	**cols;		// ARRAY OF COLUMNS WITHIN GRID
	void 	extend_cols( const int new_size );
	ESG_VAL *ptr_val( const int ic, const int ir );
	ESG_C	get_col( const int ic );
	TColor	getBColour( const int ic, const int ir ) const;
	TColor	getFColour( const int ic, const int ir ) const;
	void 	setBColour( const int ic, int ir, const TColor c );
	void 	setFColour( const int ic, const int ir, const TColor c );
public:
	ESG( TStringGrid *sg );
	~ESG( void );
	ESG_CELL	Cells;
	ESG_ROW		Row;
	ESG_COL		Col;
	ESG_VAL	*default_value;
	ESG_VAL	*dummy_value; 	// RETURNED ON ERROR
	void	DrawCell( const int ACol, const int ARow, const TRect &Rect );
};
//---------------------------------------------------------------------------
#endif

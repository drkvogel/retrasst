//---------------------------------------------------------------------------

#ifndef sgControllerH
#define sgControllerH

#include "TSGSTL.h"
#include "StringUtil.h"
#include <deque>

struct ColumnDisplay
{
		String title;
		int fieldNumber;
		int fieldPixelWidth;
};

typedef std::deque< ColumnDisplay > ColDispDeque;
typedef std::deque< ColumnDisplay >::const_iterator CitrColDispDeque;
typedef std::deque< ColumnDisplay >::iterator itrColDispDeque;

class RowItem;

class  SgControllerBase
{
		protected:
				virtual ~SgControllerBase() {}
                virtual void heading( void )=0;
                virtual void clearGrid(void )=0 ;
                virtual void blankRow(void )=0;
                virtual void highlightRow(int highRow )=0;
				virtual void highlightRowByTitle(int icol,const std::string & dataSetTitle )=0;
                virtual void toggleHighlightRow( int highRow )=0;
                virtual void highlightOff( const  int unHighRow  )=0;
                virtual void textWrapSet( int irow )=0;
                virtual void upRow( void )=0;
                virtual void downRow( void )=0;
                virtual void firstRow( void )=0;
                virtual void lastRow( void )=0;
                virtual void DrawCell_event_handler(int ACol,int ARow,TRect &Rect)=0;
                virtual void drawSgRow(const RowItem& ri)=0;
                virtual void drawRow(const RowItem& ri, const int row)=0;
                virtual void getCell(const int col,const int row, std::string & pValue )=0;
                virtual bool getCellAsInt(const int col,const int row, int& pValue )=0;
				virtual void setCell(const int col,const int row, const std::string & pValue )=0;
				virtual void getCurrentCell(const int col, std::string & pValue )=0;
				virtual int  getHighLightedRow( void )=0;
				virtual void refresh( void )=0;
				virtual void colourTone1( int row )=0;
				virtual void colourTone2( int row )=0;
				virtual void colourHighLight( TColor c )=0;
				virtual void colourMultiGroup( int row )=0;
				virtual void buildDisplayOrderList(void )=0;

// Functions used in previous Programs
// Left here only as a reminder that they may be implimented at a later date.
//                virtual int getColumn(int fieldNumber )=0;
//                virtual void addDisplayColumn( ColumnDisplay& cd)=0;

};
class SgController: public SgControllerBase
{
		public:
				SgController(TStringGrid *sgBase );
				virtual ~SgController( );
				virtual void heading( void );
				virtual void clearGrid(void ) ;
				virtual void blankRow(void );
				virtual void highlightRow(int highRow );
				virtual void highlightRowByTitle(int icol,const std::string & dataSetTitle );
				virtual void toggleHighlightRow( int iRow );
				virtual void highlightOff( const  int unHighRow  );
				virtual void textWrapSet( int irow );
				virtual void upRow( void );
				virtual void downRow( void );
				virtual void firstRow( void );
				virtual void lastRow( void );
//                virtual int getColumn(int fieldNumber );
				virtual void DrawCell_event_handler(int ACol,int ARow,TRect &Rect);
				virtual void drawSgRow(const RowItem& ri );
				virtual void drawRow(const RowItem& ri, const int row);
				virtual void getCell(const int col,const int row, std::string & pValue);
				virtual bool getCellAsInt(const int col,const int row, int& pValue );
				virtual int  getHighLightedRow( void );
				virtual void setCell(const int col,const int row, const std::string & pValue );
				virtual void getCurrentCell(const int col, std::string & pValue );
//                virtual void addDisplayColumn( ColumnDisplay& cd);
				virtual bool statusToShow( const RowItem& ss );
				virtual int calcAge(const TDateTime earilest, const TDateTime latest );
				virtual void refresh( void );
				virtual void colourHighLight( TColor c );

		protected:
//                virtual void buildDisplayOrderList(void );
//                void decodeStatus(String &Sstatus, eItemStatus eStatus, TColor& highlight, TColor& textColour );
				TSG_EXTRA2 *GCGBase;
				ColDispDeque columnDisplayOrder;
				ColumnDisplay colDisp;
				TStringGrid * sg;
				int  sgRow;
				int   sgRowLastHL;
				int   sgRowHL;
				TColor highlight,textColour;
				TColor GSGBaseColor;
				int colStatus;
				int lastRowHighLight;
};
//---------------------------------------------------------------------------
#endif

//---------------------------------------------------------------------------

#ifndef SMUtilH
#define SMUtilH

#include "inventory.h"
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
#include <ComCtrls.hpp>
#include <ExtCtrls.hpp>
#include <Grids.hpp>
#include <deque>
#include "LQuery.h"

//---------------------------------------------------------------------------

template<typename Container>
void delete_referenced(Container& c) {
    while (!c.empty()) delete c.back(), c.pop_back();
//    while (!c.empty()) {
//        if (c.back()) delete c.back();
//        c.pop_back();
//    }
}

class Util
{
	static const char * split( const char * fieldName );

	class TreeParts
	{
		std::deque< TTreeNode* > nodes;
		unsigned boxes, limit;

	public:

		TreeParts( unsigned required ) :  boxes( 0 ), limit( required  )
		{}

		bool finished() const
		{
			return nodes.empty() || (limit > 0 && boxes > limit);
		}

		TTreeNode * getNext()
		{
			 TTreeNode * next = NULL;
			 if( !nodes.empty() )
			 {
				next = nodes.front();
				nodes.pop_front();
			 }
			 return next;
		}

		void add( TTreeNode* node )
		{
			IPart * data = (IPart *)(node -> Data);
			if( data -> getType() == IPart::BoxType )
				boxes ++;
			else
				nodes.push_back( node );
		}
	};

public:

		enum IconType { OFF_LINE, SITE_LIST, SITE, NO_CHILDREN, PART_FILLED,
						ALL_FILLED, UNASSIGNED, ASSIGNED, AVAILABLE, UNKNOWN = -1 };

		static int getImageIndex( TTreeNode * node );
		static int getImageIndex( const IPart * data );
		static void InitPropertyGrid( TStringGrid *grdProps );
		static void ClearPropertyGrid(TStringGrid *grdProps);
		static void ShowPropertyGrid(TStringGrid *grdProps, IPart* data);

		enum LayoutError { NOT_FOUND, WRONG_TYPE };
		static int getCurrentLayout( const std::string & name );

		static bool validateText( TEdit* txt, TLabel* controlname );
		static int validateInteger(  TEdit* txt, int min, int max );
		static bool isVesselInUse( const std::string & srlno );
		static bool isPosDuplicate( int locationCID, short pos, short shelf = 0 );

		static void ShowTreeNode( TTreeView* tree, TTreeNode* parent, bool showGaps, bool update );
		static void ShowSelectedSubTree( TTreeView* tree, unsigned minBoxCount );

		static LQuery projectQuery( int proj_id = 0, bool ddb = false );
};

//---------------------------------------------------------------------------

#endif

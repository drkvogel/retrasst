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
#include <sstream>
#include "LQuery.h"

//---------------------------------------------------------------------------

template<typename Container>
void delete_referenced(Container& c) {
    while (!c.empty()) delete c.back(), c.pop_back();
}

/** Helper class to sort a vector of type T via sort functions defined in intializer
    Sorts explicitly ascending or descending, or toggle last sort order

    Initialize like this:

    Sorter<T> sorter[NUMCOLS] = {
        { T::sort_asc_col1,   T::sort_desc_col1,  "column name" },
        ...
    }

    Where the sort functions are defined like this (e.g.):

    bool sort_asc_col1(const T *a, const T *b)    { return Util::numericCompare(a->member1, b->member1); }

    Use like this:

    sorter[col].sort_asc(std::vector<T *>);     // sort ascending
    sorter[col].sort_dsc(std::vector<T *>);     // sort descending
    sorter[col].sort_toggle(std::vector<T *>);  // sort the opposite way to the last call
*/

//template <class T>
//class Sorter {
//public:
//    bool (*sort_func_asc)(const T *, const T *); // ascending sort function
//    std::string description;
//    enum SortOrder { ASCENDING, DESCENDING, TOGGLE };
//    void sort_asc(std::vector<T *> & vec) { std::sort(vec.begin(), vec.end(), sort_func_asc); }
//    void sort_dsc(std::vector<T *> & vec) { std::sort(vec.rbegin(), vec.rend(), sort_func_asc);  }
//    void sort_toggle(std::vector<T *> & vec) { last_sort_descending ? sort_asc(vec) : sort_dsc(vec); last_sort_descending = !last_sort_descending; }
//private:
//    bool last_sort_descending; // default: false
//};

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
        static bool numericCompare(const std::string a, const std::string b);

        static std::string getAliquotDescription(int aliquot_cid);

        static bool statsOnColumn(int project_id, std::string tableName, std::string colName);
};

//---------------------------------------------------------------------------

#endif

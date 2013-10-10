//---------------------------------------------------------------------------

#ifndef LIMSProfileMapH
#define LIMSProfileMapH

#include "LPDbProfileMap.h"

//---------------------------------------------------------------------------

class LIMSProfileMap : public LDbIdBase
{
	typedef std::vector< LPDbProfileMap >::iterator RecIter;
	
	void fill( RecIter start, RecIter end );
	int nextShared( RecIter start, RecIter end );

	class LessByColumn;

	static unsigned nextNode;

	LIMSProfileMap() : LDbIdBase( 0 )
	{}

public:

	static const LIMSProfileMap * create( const LPDbProfileMaps & pm );
	static LIMSProfileMap * getNode( unsigned index );

	class Link : public LDbIdBase
	{
		bool isLeaf;
		int linksTo;

	public:

		Link( int valueID, bool leaf, int next )
		 : LDbIdBase( valueID ), isLeaf( leaf ), linksTo( next )
		{}

		int getProfileID() const
		{
			if( isLeaf )
				return linksTo;

			throw Exception( "More than one profile ID available" );
		}

		const LIMSProfileMap * findChildren() const
		{
			return isLeaf ? NULL : getNode( linksTo );
		}

		std::string getDescription() const;
	};

	typedef Range< Link > Links;
	Links getLinks() const { return children; }

private:

	std::vector< Link > children;

};

//---------------------------------------------------------------------------

#endif


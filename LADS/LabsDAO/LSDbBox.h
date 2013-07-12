//---------------------------------------------------------------------------

#ifndef LSDbBoxH
#define LSDbBoxH
#include "LQuery.h"
#include "LDbNotes.h"

class LCDbObject;

class LSDbBox
{
	public:
		LSDbBox()
		 :  first( 0 ), last( 0 )
		{}

		LSDbBox (  const std::string & pBoxName )
		: boxName( pBoxName), first(0), last(0)
		{}

		LSDbBox (  const std::string & pBoxName, short pf, short pl )
		: boxName( pBoxName), first( pf ), last( pl )
		{}

	std::pair< short, short > getFilledPositions() const
	{
		return std::pair< short, short >( first, last );
	}

	void setFilledPositions( short start, short end )
	{
		first = start; last = end;
	}

	bool readRecord( LQuery project );
	bool saveRecord( LQuery query );

		short getFirst(void )const{return first;}
		short getLast(void )const{return last;}
		const std::string & getBoxName( void ) const {return boxName;}

	protected:
			short first, last;
			std::string boxName;
};

class LSDbBoxEvent
{
		LSDbBoxEvent()
		 : eventCID( 0 ), operatorCID( 0 ), processCID(0)
		{}

	public:

		LSDbBoxEvent( const LCDbObject * event );

		int getEventCID(void )        const {return eventCID;}
		int getOperatorCID(void )     const {return operatorCID;}
		XDATE getEventDate(void ) const {return eventDate;}
		int getProcessCID(void )      const {return processCID;}

		void setEventCID   ( const int pEventCID )  {eventCID    = pEventCID;}
		void setOperatorCID( const int pOperatorCID){operatorCID = pOperatorCID;}
		void setEventDate(const XDATE & pEventDate){eventDate  = pEventDate;}
		void setProcessCID ( const int pProcessCID) {processCID  = pProcessCID;}

	protected:

		int eventCID;
		int operatorCID;
		XDATE eventDate;
		int processCID;

		LSDbBoxEvent( const LQuery & query );
};

//---------------------------------------------------------------------------
#endif

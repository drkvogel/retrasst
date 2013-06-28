//---------------------------------------------------------------------------

#ifndef psstampH
#define psstampH

/*
Author: Andrew Munday. initial relased into CVS 22-01-2009.

PSstamper - keeps a list of links to items and will stamp the list of items
on to the curren label given the stamp command.

$id$

*/
#include "psprintr.h"
#include "psitem.h"
#include <deque>


class PSStamper: public PSRect
{

		public:
				void addItem( PSItemBase * anItem );
				void clearItem( void );
				void setPrinter( PSPrinter *ppr );
				void stamp(void );
				void setDpi( int pDpi );
				void setDpcm( int pDpcm );
				void setPixelOffset( PSSize* ppo);
		private:
				PSPrinter* pr;
				PSSize* pixelOffset;
				int dpi, dpcm;
				int itemCount;
				int startXpi,endXpi,startYpi,endYpi;

				std::deque< PSItemBase * > allItems;


};
//---------------------------------------------------------------------------
#endif

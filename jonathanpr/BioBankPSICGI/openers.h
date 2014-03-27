#ifndef openersH
#define openersH
//---------------------------------------------------------------------------
#include <string>
#include "xdb.h"
//---------------------------------------------------------------------------
namespace OPENERS
{
	void 	outputHead( const std::string title );
	XDB 	*openDatabase( void );
};
//---------------------------------------------------------------------------
#endif


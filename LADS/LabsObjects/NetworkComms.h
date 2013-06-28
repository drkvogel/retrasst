//---------------------------------------------------------------------------

#ifndef NetworkCommsH
#define NetworkCommsH

#include "LIMSCommsBase.h"

//---------------------------------------------------------------------------

class NetworkComms : public LIMSCommsBase
{

public:

	void open();
	unsigned available();
	unsigned readBytes( unsigned char * buffer, unsigned max );
	unsigned sendBytes( const unsigned char * bytes, unsigned count );
	void clear();
};

//---------------------------------------------------------------------------
#endif

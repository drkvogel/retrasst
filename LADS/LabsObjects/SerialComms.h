//---------------------------------------------------------------------------

#ifndef SerialCommsH
#define SerialCommsH

#include "InputFile.h"

//---------------------------------------------------------------------------

class SerialComms : public LIMSCommsBase
{
	unsigned write( const unsigned char * bytes, unsigned count );

public:

	struct PortParams
	{
		String port;
		String baud;
		String start;
		String data;
		String stop;
		String parity;
	};

	SerialComms();
	SerialComms( const PortParams & params );

	unsigned available();
	unsigned readBytes( unsigned char * buffer, unsigned count );
	unsigned sendBytes( const unsigned char * bytes, unsigned count );
	void clear();

private:
	bool open( const PortParams & params );
};

//---------------------------------------------------------------------------

#endif


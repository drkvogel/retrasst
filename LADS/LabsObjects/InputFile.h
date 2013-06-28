//---------------------------------------------------------------------------

#ifndef InputFileH
#define InputFileH

#include "LIMSCommsBase.h"

//---------------------------------------------------------------------------

class InputFile : public LIMSCommsBase
{
protected:

	InputFile( const String & filePath );
	unsigned length();

public:

	unsigned sendBytes( const unsigned char * bytes, unsigned count );
};

//---------------------------------------------------------------------------

class TextInputFile : public InputFile
{
	unsigned remaining;

public:

	TextInputFile( const String & path ) : InputFile( path ), remaining( length() )
	{}

	unsigned readBytes( unsigned char * buffer, unsigned max );

	void clear() { remaining = 0; }
	unsigned available() { return remaining; }
};

//---------------------------------------------------------------------------

class XMLInputFile : public InputFile
{
	unsigned char * input;
	const char * next;

	static unsigned char convert( const char * entity, unsigned length );
	static unsigned fromHex( unsigned char ch );

public:

	XMLInputFile( const String & path ) : InputFile( path ), input( NULL ), next( NULL )
	{}

	~XMLInputFile()
	{
		if( input != NULL )
			free( (void *) input );
	}

	void clear() { next = NULL; }

	unsigned available()
	{
		return (input == NULL) ? length() : (next == NULL) ? 0 : strlen( next );
	}

	unsigned readBytes( unsigned char * buffer, unsigned max );
};

//---------------------------------------------------------------------------

#endif


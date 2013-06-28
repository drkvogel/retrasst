//---------------------------------------------------------------------------

#ifndef LIMSCommsBaseH
#define LIMSCommsBaseH

#include "XMLFile.h"

//---------------------------------------------------------------------------

class LIMSCommsBase
{
	XMLFile log;

protected:

	String source;
	HANDLE IO_Handle;

	LIMSCommsBase( String logFile = "" )
	 : IO_Handle( INVALID_HANDLE_VALUE ), log( "io", logFile.IsEmpty(), true, "", logFile )
	{}

	void open( const String & name, unsigned desiredAccess, unsigned shareMode );
	unsigned read( unsigned char * buffer, unsigned max );

	static const char * getName( unsigned char byte );
	void record( const char * direction, const unsigned char * bytes, unsigned count );

public:

	const String & getSource() const { return source; }

	static const char NUL = 0, SOH = 1, STX = 2, ETX = 3, EOT = 4, ENQ = 5, ACK = 6,
		BEL = 7, BS = 8, HT = 9, LF = 10, VT = 11, FF = 12, CR = 13, SO = 14, SI = 15,
		DLE = 16, XON = 17, DC1 = 17, DC2 = 18, DC3 = 19, XOFF = 19, DC4 = 20,
		NAK = 21, SYN = 22, ETB = 23, CAN = 24, EM = 25, SUB = 26, ESC = 27;

	virtual ~LIMSCommsBase()
	{
		if( IO_Handle != INVALID_HANDLE_VALUE )
			CloseHandle( IO_Handle );
	}

	virtual unsigned available() = 0;
	virtual unsigned readBytes( unsigned char * buffer, unsigned max ) = 0;
	virtual unsigned sendBytes( const unsigned char * bytes, unsigned count ) = 0;
	virtual void clear() = 0;
};

//---------------------------------------------------------------------------

class Buffer
{
	const static BLOCK_SIZE = 100;

	unsigned char * store;
	unsigned start, end, size;

public:

	Buffer() : store( NULL ), start( 0 ), end( 0 ), size( 0 )
	{}

	~Buffer();

	struct Contents
	{
		const unsigned char * data;
		unsigned length;

		Contents( const unsigned char * d, unsigned l ) : data( d ), length( l )
		{}

		operator String() const
		{
			return String( (char *) data, length );
		}
	};

	Contents getContents() const
	{
		return Contents( store + start, end - start );
	}

	void clear() { start = end = 0; }
	bool empty() const { return end == start; }

	const Buffer & operator=( unsigned char byte );
	const Buffer & operator=( const char * c_str );
	void add( unsigned char byte );
	void add( const unsigned char * data, unsigned length );
	unsigned char * makeSpace( unsigned length = 1 );
	void skip( unsigned length );
	void accept( unsigned length ) { end += length; }
};

//---------------------------------------------------------------------------

#endif


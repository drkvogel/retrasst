//---------------------------------------------------------------------------

#ifndef XMLFileH
#define XMLFileH

#include <stack>
#include <vector>
#include <iostream>
#include <fstream>
#include <string>

//---------------------------------------------------------------------------

class XMLFile
{
	std::ofstream pfOut;

	std::string type, fileName;
	std::vector< std::string > parts;

	std::stack< std::string > tags;
	bool inTag, chunks, encodeSpace;

	static const short SHORT_STRING = 100;	   	// enough for dates etc.
	static const int CHUNK_SIZE = 500000;       // file < half megabyte

	std::ofstream & openFile();

	std::string createChunkFile();
	std::string uniqueName( std::string extension = "" );

	void encode( const char * text );
	void encode( unsigned char byte );
	void encode( const TDateTime & when );

	XMLFile( const XMLFile & );				// not implemented
	XMLFile & operator=( XMLFile & );		// not implemented

public:

	XMLFile( const char * root, bool split = true, bool encodeAll = false, const std::string & out = "" )
	 : type( root ), fileName( out ), inTag( false ), chunks( split ), encodeSpace( encodeAll )
	{}

	~XMLFile();

	void addHeader();
	void start( const char * tag );
	void endTag();

	bool inElement() const;
	const std::string & getElement() const;

	void addEmpty( const char * tag );

	void addLeaf( const char * tag, TDateTime timestamp );
	void addLeaf( const char * tag, int value );
	void addLeaf( const char * tag, const std::string & body );

	void addAttribute( const char * name, const std::string & value );
	void addAttribute( const char * name, const TDateTime & when );

	void addText( const std::string & text );
	void addCharacter( const unsigned char byte );

	static void logError( const char * tag, const std::string & message );
	static TDateTime isoDateTime( const std::string & when );
};

//---------------------------------------------------------------------------

#endif


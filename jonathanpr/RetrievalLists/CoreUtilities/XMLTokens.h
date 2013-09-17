//---------------------------------------------------------------------------

#ifndef XMLTokensH
#define XMLTokensH

#include <vector>
#include <map>
#include <string>

//---------------------------------------------------------------------------

class XMLTokens
{
	static const BUFF_SIZE = 200;

	String source;
	const char * nextChar;
	bool emptyElement;

public:

	XMLTokens( const String & text )
	 : source( text ), nextChar( source.c_str() ), emptyElement( false ), current( parseTag() )
	{}

	void checkHeader();

	class Tag
	{
		String name;
		std::map< String, String > attributes;
		bool isEndTag;

		void addAttributes( const char * start, const char * end );

	public:

		Tag( const String & element ) : name( element ), isEndTag( false )
		{}

		Tag( const char * start, const char * end, bool closing );

		Tag endTag() const;
		operator String() const;

		bool operator<( const Tag & expected ) const;
		bool operator==( const String & expected ) const;
		bool operator==( const Tag & expected ) const;
		bool operator!=( const String & expected ) const;
		bool operator!=( const Tag & expected ) const;
	};

	class Fields
	{
		Tag tag;
		std::map< Tag, String > fields;

	public:

		Fields( const Tag & type ) : tag( type )
		{}

		String getType() const { return tag; }
		operator String() const;

		bool hasField( const Tag & tag ) const
		{
			return fields.find( tag ) != fields.end();
		}

		void addField( const Tag & tag, const String & value )
		{
			fields.insert( std::pair< Tag, String >( tag, value ) );
		}

		const String & readString( const String & tag ) const;
		int readInteger( const String & tag ) const
		{
			return readString( tag ).ToInt();
		}
		TDateTime readDateTime( const String & tag ) const;
	};

	struct EInvalidFormat : public Exception
	{
		EInvalidFormat() : Exception( "Invalid XML format" ) {}
	};

	const Tag & currentTag() const { return current; }
	const Tag & nextTag() { return current = parseTag(); }
	const Fields parseFields();

private:

	Tag current;

	const Tag parseTag();
	const char * parseText();
	char skipComments();
	char replaceEntity();
};

//---------------------------------------------------------------------------

#endif


//---------------------------------------------------------------------------
//
//		19 July 2005:  		Moved checkHeader() here; should match XMLFile
//      24 January 2006:    Simplified Tag handling (but still far too slow)
//      14 February, NG:	Keep copy of input String in case it's deleted
//
//---------------------------------------------------------------------------

#include <vcl.h>
#include "XMLTokens.h"
#include "XMLFile.h"
#include "LCDbProject.h"

#pragma hdrstop
#pragma package(smart_init)

//---------------------------------------------------------------------------
//  Parse fields from current record; return as a <tag, text> map
//---------------------------------------------------------------------------

const XMLTokens::Fields XMLTokens::parseFields()
{
	Tag section = currentTag();
	Fields record( section );
	while( nextTag() != section.endTag() )
	{
		const char * text = parseText();
		if( parseTag() != current.endTag() )
			throw EInvalidFormat();

		if( text != NULL && *text != '\0' )
			record.addField( current, text );
	}
	return record;
}

//---------------------------------------------------------------------------
//  Copy text between tags, replacing &lt; &gt; &apos; &quot; and &amp;
//---------------------------------------------------------------------------

const char * XMLTokens::parseText()
{
	if( emptyElement )
		return NULL;                         	// still in current tag

	short pos = 0, pending = 0;
	static char buffer[ BUFF_SIZE + 2 ];
	for( ;; )
	{
		char toAdd;
		switch( *nextChar )
		{
		case '\0':
			throw EInvalidFormat();             // unexpected end of file

		case '<':
			buffer[ pos ] = '\0';
			return buffer;                      // ready to copy into map

		case '\r':
		case '\n':
		case '\t':
		case ' ':
			pending ++;
			nextChar ++;
			continue;                           // only copy when required

		case '&':
			toAdd = replaceEntity();            // replace entity reference
			break;

		default:
			toAdd = *nextChar ++;
		}

		if( pos >= BUFF_SIZE )
			throw EInvalidFormat();             // no closing tag found

		else if( pending > 0 )
		{
			if( pos > 0 )
				buffer[ pos ++ ] = ' ';
			pending = 0;                        // compress white space
		}
		buffer[ pos ++ ] = toAdd;
	}
}

//---------------------------------------------------------------------------
//  Parse next tag; keep pointer to start; return length or exception
//---------------------------------------------------------------------------

const XMLTokens::Tag XMLTokens::parseTag()
{
	if( emptyElement )
	{
		emptyElement = false;                   // as if 2nd tag read
		return current.endTag();
	}

	if( skipComments() != '<' )
		throw EInvalidFormat();                 // text when tag expected

	bool endTag = false;
	if( * nextChar == '/' )
	{
		endTag = true;
		nextChar ++;                            // skip </ for end tag
	}

	const char * tagStart = nextChar;
	nextChar = std::strchr( tagStart, '>' );	// ignore attributes
	if( nextChar == NULL )
		throw EInvalidFormat();

	if( nextChar[ -1 ] == '/' )
	{
		emptyElement = true;
		nextChar ++;
		return Tag( tagStart, nextChar - 2, endTag );
	}
	else return Tag( tagStart, nextChar ++, endTag );
}

//---------------------------------------------------------------------------
//  Skip leading spaces and comments; return next significant character
//---------------------------------------------------------------------------

char XMLTokens::skipComments()
{
	if( nextChar == NULL )
		return '\0';

	short open = 0;
	for( ;; )
	{
		switch( *nextChar ++ )
		{
			case '<':
				if( open > 0 || *nextChar == '!' || *nextChar == '?' )
				{
					open ++;                     // comment or processing
					continue;
				}
				else return '<';

			case '>':
				if( open > 0 )
				{
					open --;                     // end of instructions
					continue;
				}
				else return '>';

			case '\r':
			case '\n':
			case '\t':
			case ' ':
				continue;                        // skip any white space

			default:
				if( open == 0 )
					return nextChar[ -1 ];       // text before tag start
		}
	}
}

//---------------------------------------------------------------------------
//  Return character for entities &lt; &gt; &apos; &quot; and &amp;
//---------------------------------------------------------------------------

char XMLTokens::replaceEntity()
{
	static const char * refs[] = { "gt;", "lt;", "apos;", "quot;", "amp;", NULL };
	static const char * values = "><\'\"&";

	nextChar ++;                                 // all refs start with &
	for( short i = 0; refs[ i ] != NULL; i ++ )
	{
		unsigned len = std::strlen( refs[ i ] );
		if( std::strncmp( nextChar, refs[ i ], len ) == 0 )
		{
			nextChar += len;
			return values[ i ];                  // replace entity reference
		}
	}
	return '&';                                  // copy if unrecognised
}

//---------------------------------------------------------------------------
//	Create Tag from passed input string; add attributes if present
//---------------------------------------------------------------------------

XMLTokens::Tag::Tag( const char * start, const char * end, bool closing )
 : isEndTag( closing )
{
	const char * nameEnd = std::strpbrk( start, "> \r\n\t" );
	if( nameEnd < end )
	{
		name = String( start, nameEnd - start );
		addAttributes( nameEnd + 1, end );
	}
	else name = String( start, end - start );
}

//---------------------------------------------------------------------------

XMLTokens::Tag XMLTokens::Tag::endTag() const
{
	Tag tag( name );
	tag.isEndTag = true;
	return tag;
}

//---------------------------------------------------------------------------
//	Store attributes from the give section of the input buffer
//---------------------------------------------------------------------------

void XMLTokens::Tag::addAttributes( const char * next, const char * end )
{
	while( next < end )
	{
		if( isspace( *next ) )
			next ++;
		else
		{	const char * equals = std::strchr( next, '=' );
			if( equals == NULL || equals == next || equals >= end )
				throw XMLTokens::EInvalidFormat();

			char quote = equals[ 1 ];
			if( quote != '\'' && quote != '\"' )
				throw XMLTokens::EInvalidFormat();

			const char * value = equals + 2;
			const char * last = std::strchr( value, quote );
			if( last == NULL || last >= end )
				throw XMLTokens::EInvalidFormat();

			if( last > value )
			{
				String name( next, equals - next ), val( value, last - value );
				attributes.insert( std::pair< String, String >( name, val ) );
			}
			next = last + 1;
		}
	}
}

//---------------------------------------------------------------------------
//  Compare two tags or a tag with a string; return true or false
//---------------------------------------------------------------------------

bool XMLTokens::Tag::operator<( const Tag & other ) const
{
	int diff = name.AnsiCompare( other.name );
	return diff == 0 ? other.isEndTag && !isEndTag : diff < 0;
}

//---------------------------------------------------------------------------

bool XMLTokens::Tag::operator==( const Tag & other ) const
{
	return isEndTag == other.isEndTag &&  name == other.name;
}

//---------------------------------------------------------------------------

bool XMLTokens::Tag::operator==( const String & expected ) const
{
	return !isEndTag && name == expected;
}

//---------------------------------------------------------------------------

bool XMLTokens::Tag::operator!=( const Tag & other ) const
{
	return isEndTag != other.isEndTag || name != other.name;
}

//---------------------------------------------------------------------------

bool XMLTokens::Tag::operator!=( const String & expected ) const
{
	return isEndTag || name != expected;
}

//---------------------------------------------------------------------------
//	Convert tag back to the text that it might have come from
//---------------------------------------------------------------------------

XMLTokens::Tag::operator String() const
{
	String result = name;
	std::map< String, String >::const_iterator fi = attributes.begin();
	while( fi != attributes.end() )
	{
		result = result + ' ' + fi -> first + "=\'" + fi -> second + "\'";
		++ fi;
	}
	return isEndTag ? "</" + result + '>' : "<" + result + '>';
}

//---------------------------------------------------------------------------
//  Return values from the field map if the passed tag was present
//---------------------------------------------------------------------------

const String & XMLTokens::Fields::readString( const String & tag ) const
{
	std::map< Tag, String >::const_iterator next = fields.find( tag );
	if( next == fields.end() )
		throw EInvalidFormat();

	return next -> second;
}

//---------------------------------------------------------------------------

TDateTime XMLTokens::Fields::readDateTime( const String & tag ) const
{
	return XMLFile::isoDateTime( readString( tag ) );
}

//---------------------------------------------------------------------------
//  Convert the fields from the Fields back into XML-like text
//---------------------------------------------------------------------------

XMLTokens::Fields::operator String() const
{
	String result = tag;
	std::map< Tag, String >::const_iterator fi = fields.begin();
	while( fi != fields.end() )
	{
		result = result + fi -> first + fi -> second + fi -> first.endTag();
		++ fi;
	}
	return result + tag.endTag();
}

//---------------------------------------------------------------------------
//	Check the input was created from the current project (cf XMLFile)
//---------------------------------------------------------------------------

void XMLTokens::checkHeader()
{
	if( nextTag() != "history" )
		throw EInvalidFormat();

	String project = parseFields().readString( "project" );
	const LCDbProject * source = LCDbProjects::records().findByName( project );
	if( source == NULL || source -> getID() != LCDbProjects::getCurrentID() )
		throw Exception( "Data file does not match current project" );
}

//---------------------------------------------------------------------------


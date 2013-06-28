//---------------------------------------------------------------------------
//
//	Version history:
//
//		7 June 2005		Store start_delimiter if it exists in descrip
//      16 June, NG:	Added method to find descriptor by field name
//						and renamed other find methods to avoid confusion
//		21 July 2005	Added getObjectType() and refinded isActive()
//      16 September	Leave map field or start delimiter empty if ' '
//		24 Sep 2008		Make source optional; try to use flags instead
//		2 Oct 08, NG:	added position (display_order) for Specimen Reception
//      26 Aug 09, NG:	added grouping level for NightShift reporting
//      7 June 2012:	Borland C++Builder XE2 version
//
//---------------------------------------------------------------------------

#include <vcl.h>
#include <sstream>
#pragma hdrstop

#include "LQuery.h"
#include "LCDbProject.h"
#include "LPDbDescriptor.h"
#include "xb36Code.h"

#pragma package(smart_init)

//---------------------------------------------------------------------------
//  Keep relevant values from descrip and descriptor_values
//---------------------------------------------------------------------------

LPDbDescriptor::LPDbDescriptor( const LQuery & query )
 : LPDbID( query.readInt( "descriptor_id" ) ),
   LDbNames( query.readString( "descriptor_name" ),
			 query.readString( "description" ) ),
//   parseOrder( query.readShort( "parse_order" ) ),
   lengths( query.readInt( "min_length" ),
			query.readInt( "max_length" ) )
{
//	String value = query.readString( "start_delimiter" );
//	delimiter = value.IsEmpty() ? '\0' : value[ 1 ];

	std::string value = query.readString( "map_field_name" );
	if( !value.empty() && value != "." )
		mapField = value; // .LowerCase();

	value = query.readString( "specimen_field_name" );
	if( !value.empty() && value != "." )
		specimenField = value; // .LowerCase();

	srPosition = query.fieldExists( "display_order" ) ? query.readInt( "display_order" ) : 1;
	nsGrouping = query.fieldExists( "grouping_level" ) ? query.readInt( "grouping_level" ) : 1;

	active = true;
	objectName = false;
	byHand = true;
	int flags = query.readInt( "descriptor_flags" );
	if( flags == DELETED )
		active = false;
	else
	{ 	if( (flags & IN_SPECIMEN) != 0 )
			byHand = false;
		if( (flags & IS_OBJECT) != 0 )
			objectName = true;
	}

	if( query.fieldExists( "source" ) )
	{
		value = query.readString( "source" ); // .Trim();
		if( !value.empty() && value != "." && value.compare( "byhand" ) != 0 )
		{
			byHand = false;
//			if( value.AnsiCompareIC( "barcode" ) == 0 && parseOrder == 0 )
//				parseOrder = 1;
		}
	}
}

//---------------------------------------------------------------------------

LPDbDescriptor::Value::Value( const LQuery & query )
 : LPDbID( query.readInt( "value_id" ) ),
   name( query.readString( "value_name" ) ),
   range( query.readString( "min_value" ),
		  query.readString( "max_value" ) )
{}

//---------------------------------------------------------------------------
//  Read and store descriptor values for the current project
//---------------------------------------------------------------------------

bool LPDbDescriptors::read( LQuery pQuery, bool readAll )
{
	if( readAll )
		pQuery.setSQL( "select * from descrip"
					  " order by descriptor_id" );
	else
	{	pQuery.setSQL( "select * from descrip where descriptor_flags <> :del"
					  " order by descriptor_id" );
		pQuery.setParam( "del", LPDbDescriptor::DELETED );
	}
	if( !readData( pQuery ) )
		return false;


	if( readAll )
	{	pQuery.setSQL( "select * from descriptor_value"
					  " order by descriptor_id, value_id" );
	}
	else
	{	pQuery.setSQL( "select * from descriptor_value where status <> :del"
					  " order by descriptor_id, value_id" );
		pQuery.setParam( "del", LPDbDescriptor::DELETED );
	}

	iterator ci = begin();
	for( pQuery.open(); !pQuery.eof(); pQuery.next() )
	{
		int did = pQuery.readInt( "descriptor_id" );
		while( ci != end() && ci -> getID() < did )
			ci ++;

		if( ci != end() && ci -> getID() == did )
			ci -> addValue( pQuery );
	}
	return true;
}

//---------------------------------------------------------------------------
//	Create/update database entry for this descriptor; copy to cache
//---------------------------------------------------------------------------

bool LPDbDescriptor::saveRecord( LQuery query )
{
	if( saved )
	{
		query.setSQL( "Update descrip set descriptor_name = :name, description = :des,"
//					 " parse_order = :po, start_delimiter = :sdl,
					 " min_length = :mnl, max_length = :mxl,"
					 " specimen_field_name = :sfn, map_field_name = :mfn,"
					 " descriptor_flags = :flg, display_order = :do, grouping_level = :gl"
					 " where descriptor_id = :did" );
	}
	else
	{	claimNextID( query );
		query.setSQL( "Insert into descrip (descriptor_id, descriptor_name, description,"
					" parse_order, start_delimiter, min_length, max_length, specimen_field_name,"
					" map_field_name, descriptor_flags, display_order, grouping_level)"
					" values ( :did, :name, :des, :po, :sdl, :mnl, :mxl, :sfn, :mfn, :flg, :do, :gl )" );
	}

	LCMatcher lcName( getName() );
	query.setParam( "did", getID() );
	query.setParam( "name", lcName.getLCValue() );
	query.setParam( "des", getDescription() );
//	query.setParam( "po", parseOrder );
//	query.setParam( "sdl", delimiter );
	query.setParam( "mnl", lengths.first );
	query.setParam( "mxl", lengths.second );

	if( specimenField.empty() )
		query.setParam( "sfn", "." );
	else
		query.setParam( "sfn", specimenField );

	if( mapField.empty() )
		query.setParam( "mfn", "." );
	else
		query.setParam( "mfn", mapField );

	query.setParam( "flg", getFlagBits() );
	query.setParam( "do", srPosition );
	query.setParam( "gl", nsGrouping );
	if( !query.execSQL() )
		return false;
	else
	{	saved = true;
		setName( lcName.getLCValue() );
		LPDbDescriptors::records().insert( *this );
		return true;
	}
}

//---------------------------------------------------------------------------

int LPDbDescriptor::getFlagBits() const
{
	if( !isActive() )
		return DELETED;

	int status = isByHand() ? 0 : IN_SPECIMEN;
//	if( isFromBarcode() )
//		status |= BARCODE;
	if( !hasValues() )
		status |= NO_VALUES;
	if( !isInProfileMap() )
		status |= NO_PROFILE;
	if( isObject() )
		status |= IS_OBJECT;
	return status;
}

//---------------------------------------------------------------------------
//	Create/update database entry for this descriptor; copy to cache
//---------------------------------------------------------------------------

void LPDbDescriptor::addValue( LQuery query, Value definition )
{
	if( valueList.findByID( definition.getID() ) != NULL )
	{
		query.setSQL( "Update descriptor_value set value_name = :name,"
					 " min_value = :mnv, max_value = :mxv, status = :sts"
					 " where value_id = :vid" );
		query.setParam( "sts", 1 );
	}
	else
	{	definition.claimNextID( query );
		query.setSQL( "Insert into descriptor_value (value_id,"
					 " descriptor_id, value_name, min_value, max_value, status)"
					 " values ( :vid, :did, :name, :mnv, :mxv, :sts )" );
		query.setParam( "did", getID() );
		query.setParam( "sts", 0 );
	}

	LCMatcher lcName( definition.getName() );
	std::pair< std::string, std::string > range = definition.getRange();
	query.setParam( "vid", definition.getID() );
	query.setParam( "name", lcName.getLCValue() );
	query.setParam( "mnv", range.first );
	query.setParam( "mxv", range.second );
	if( query.execSQL() )
	{
		definition.setName( lcName.getLCValue() );
		valueList.insert( definition );
		LPDbDescriptors::records().insert( *this );
	}
}

//---------------------------------------------------------------------------
//	Check if value matches the descriptor value or falls within a range
//---------------------------------------------------------------------------

bool LPDbDescriptor::Value::includes( const std::string & value ) const
{
	static const xb36Code base36;
	return base36.compare( value, range ) == 0;
}

//---------------------------------------------------------------------------
//	Find a value for the given descriptor by finding its name
//---------------------------------------------------------------------------

const LPDbDescriptor::Value * LPDbDescriptor::findValueByName( const std::string & name ) const
{
	for( Range< Value > dvi = getValues(); dvi.isValid(); ++ dvi )
		if( name.compare( dvi -> getName() ) == 0 )
			return &(*dvi);

	return NULL;
}

//---------------------------------------------------------------------------
//	Find the value/range that includes the given string(s)
//---------------------------------------------------------------------------

const LPDbDescriptor::Value * LPDbDescriptor::findValueByValue( const std::string & value ) const
{
	for( Range< Value > dvi = getValues(); dvi.isValid(); ++ dvi )
		if( dvi -> includes( value ) )
			return &(*dvi);

	return NULL;
}

//---------------------------------------------------------------------------

const LPDbDescriptor::Value * LPDbDescriptor::findValueRange( const std::pair< std::string, std::string > & vr ) const
{
	if( vr.first == vr.second )
		return findValueByValue( vr.first );
		
	for( Range< Value > dvi = getValues(); dvi.isValid(); ++ dvi )
		if( dvi -> includes( vr.first ) && dvi -> includes( vr.second ) )
			return &(*dvi);

	return NULL;
}

//---------------------------------------------------------------------------
//	Find the project descriptor which includes the given value
//---------------------------------------------------------------------------

class LPDbDescriptors::ValueNameMatch : public std::unary_function< LPDbDescriptor, bool >
{
	const std::string & value;

public:

	ValueNameMatch( const std::string & name ) : value( name )
	{}

	operator std::string() const { return value; }

	bool operator() ( const LPDbDescriptor & candidate ) const
	{
		return candidate.findValueByName( value ) != NULL;
	}
};

//---------------------------------------------------------------------------

const LPDbDescriptor * LPDbDescriptors::findByValueName( const std::string & name ) const
{
	return findMatch( ValueNameMatch( name ) );
}

//---------------------------------------------------------------------------
//	Find the project descriptor which includes the given value ID
//---------------------------------------------------------------------------

class LPDbDescriptors::ValueIDMatch : public std::unary_function< LPDbDescriptor, bool >
{
	const int valueID;

public:

	ValueIDMatch( int id ) : valueID( id )
	{}

	operator std::string() const {
		std::stringstream out;
		out << "ID: " << valueID;
		return out.str();
	}

	bool operator() ( const LPDbDescriptor & candidate ) const
	{
		return candidate.findValueByID( valueID ) != NULL;
	}
};

//---------------------------------------------------------------------------

const LPDbDescriptor * LPDbDescriptors::findByValueID( int valueID ) const
{
	return findMatch( ValueIDMatch( valueID ) );
}

//---------------------------------------------------------------------------
//	Find the descriptor with values stored in the given field
//---------------------------------------------------------------------------

class LPDbDescriptors::FieldMatch : public std::unary_function< LPDbDescriptor, bool >
{
	const std::string field;

public:

	FieldMatch( const std::string & fieldName ) : field( fieldName )
	{}

	operator std::string() const { return field; }

	bool operator() ( const LPDbDescriptor & candidate ) const
	{
		return (field.compare( candidate.getSpecimenField() ) == 0
			 || field.compare( candidate.getMapField() ) == 0);
	}
};

//---------------------------------------------------------------------------

const LPDbDescriptor * LPDbDescriptors::findByField( const std::string & field ) const
{
	return findMatch( FieldMatch( field ) );
}

//---------------------------------------------------------------------------
//	Find the project descriptor with the given name or description
//---------------------------------------------------------------------------

const LPDbDescriptor * LPDbDescriptors::findByName( const std::string & name ) const
{
	return findMatch( LDbNames::LCMatcher( name ) );
}

//---------------------------------------------------------------------------
//	Suggest the next unused field in the (version 2.2) profile map table
//---------------------------------------------------------------------------

int getFieldNumber( const std::string & field ) {
	int num = 0;
	for( unsigned n = 4; n < field.length(); n ++ ) {
		char ch = field[ n ];
		if( ch >= '0' && ch <= '9' ) {
			num = (num * 10) + (ch - '0');
		}
	}
	return num;
}

//---------------------------------------------------------------------------

std::string LPDbDescriptors::getNextMapField()
{
	int next = 1;
	for( Range< LPDbDescriptor > dr = *this; dr.isValid(); ++ dr ) {
		int num = getFieldNumber( dr -> getMapField() );
		if( num >= next )
			next = num + 1;
	}

	std::stringstream out;
	out << "value_id" << next;
	return out.str();
}

//---------------------------------------------------------------------------
//	Suggest the next unused value field in the specimen table
//---------------------------------------------------------------------------

std::string LPDbDescriptors::getNextSpecimenField()
{
	int next = 1;
	for( Range< LPDbDescriptor > dr = *this; dr.isValid(); ++ dr ) {
		int num = getFieldNumber( dr -> getSpecimenField() );
		if( num >= next )
			next = num + 1;
	}

	std::stringstream out;
	out << "value" << next;
	return out.str();
}

//---------------------------------------------------------------------------
//	Delete database entry for this descriptor value; update cache
//---------------------------------------------------------------------------

void LPDbDescriptor::removeValue( LQuery query, int valueID )
{
	query.setSQL( "delete from descriptor_value where value_id = :vid" );
	query.setParam( "vid", valueID );
	query.execSQL();
	valueList.erase( valueID );
	LPDbDescriptors::records().insert( *this );
}

//---------------------------------------------------------------------------


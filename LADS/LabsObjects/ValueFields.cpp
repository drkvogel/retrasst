/*---------------------------------------------------------------------------
 *
 *	Version history:
 *
 *	   	6 November 2008, NG:	extracted from LDbSource and refactored
 *      13 January 2009, NG:    added makeWhereClause() for shared use
 *      20 April 2011, NG:		removed barcode descriptors - no parsing
 *      6 June 2012, NG:		C++Builder XE2 version
 *		18 March 2013, NG:		Allow multiple values, not in a range
 *
 *---------------------------------------------------------------------------*/

#include <vcl.h>

#include "LCDbProject.h"
#include "ValueFields.h"
#include "xb36Code.h"
#include "LPDbProfileMap.h"

#pragma hdrstop
#pragma package(smart_init)

//---------------------------------------------------------------------------

ValueField::ValueField( const std::string & field )
 : fieldName( field ), descriptID( 0 ), valueID( 0 ), isRange( false )
{}

//---------------------------------------------------------------------------

ValueField::ValueField( const LPDbDescriptor & dRef )
 : fieldName( dRef.getSpecimenField() ), descriptID( dRef.getID() ), valueID( 0 ), isRange( false )
{}

//---------------------------------------------------------------------------

void ValueField::clearValues() {
	valueID = 0;
	values.clear();
	isRange = false;
}

//---------------------------------------------------------------------------
//	set the field name; value should follow; leave descriptor for later
//---------------------------------------------------------------------------

ValueField & ValueField::field( const std::string & fName ) {
	clearValues();
	descriptID = 0;
	fieldName = fName;
	return *this;
}

//---------------------------------------------------------------------------
//  set descriptor to match given name; throw Exception if it does not
//---------------------------------------------------------------------------

ValueField & ValueField::descriptor( const std::string & dName ) {
	const LPDbDescriptor * dp = LPDbDescriptors::records().findByName( dName );
	if( dp == NULL ) {
		throw Exception( String( "Invalid descriptor: " ) + dName.c_str() );
	} else {
		clearValues();
		descriptID = dp->getID();
		fieldName = dp->getSpecimenField();
		return *this;
	}
}

//---------------------------------------------------------------------------
//	attempt to find descriptor for the current field, otherwise NULL
//---------------------------------------------------------------------------

const LPDbDescriptor * ValueField::findDescriptor() const {
	const LPDbDescriptor * found = NULL;
	const LPDbDescriptors & cache = LPDbDescriptors::records();
	if( descriptID != 0 ) {
		found = cache.findByID( descriptID );
	}
	if( found == NULL && !fieldName.empty() ) {
		found = cache.findByField( fieldName );
	}
	return found;
}

//---------------------------------------------------------------------------

const LPDbDescriptor * ValueField::getDescriptor() {
	const LPDbDescriptor * found = findDescriptor();
	if( found != NULL ) {
		descriptID = found->getID();
	}
	return found;
}

//---------------------------------------------------------------------------

int ValueField::getDescripID() {
	if( descriptID == 0 ) {
		const LPDbDescriptor * found = findDescriptor();
		if( found != NULL ) {
			descriptID = found->getID();
		}
	}
	return descriptID;
}

//---------------------------------------------------------------------------

const LPDbDescriptor * ValueField::getDescriptor() const {
	return findDescriptor();
}

//---------------------------------------------------------------------------

int ValueField::getDescripID() const {
	if( descriptID != 0 ) {
		return descriptID;
	}
	const LPDbDescriptor * found = findDescriptor();
	if( found != NULL ) {
		return found->getID();
	}
	throw Exception( "Invalid descriptor ID" );
}

//---------------------------------------------------------------------------
//	add (another) value to the value set; assume it matches descriptor
//---------------------------------------------------------------------------

ValueField & ValueField::value( const std::string & value ) {
	values.insert( value );
	return *this;
}

//---------------------------------------------------------------------------
//	replace current values with the given range and descriptor value ID
//---------------------------------------------------------------------------

void ValueField::setValueRange( const std::pair< std::string, std::string > & range, int id ) {
	values.clear();
	values.insert( range.first );
	values.insert( range.second );
	valueID = id;
	isRange = (values.size() == 2);
}

//---------------------------------------------------------------------------
//	set range from given descriptor value; assume descriptor already set
//---------------------------------------------------------------------------

ValueField & ValueField::value( const LPDbDescriptor::Value * dvp ) {
	if( dvp == NULL ) {
		clearValues();
	} else {
		setValueRange( dvp -> getRange(), dvp -> getID() );
	}
	return *this;
}

//---------------------------------------------------------------------------
//	set range from descriptor value with given name, if descriptor OK
//---------------------------------------------------------------------------

ValueField & ValueField::valueName( const std::string & vName ) {
	const LPDbDescriptor::Value * dvp = NULL;
	const LPDbDescriptor * owner = getDescriptor();
	if( owner != NULL ) {
		dvp =  owner -> findValueByName( vName );
	}
	if( dvp == NULL ) {
		throw Exception( String( "No descriptor for " ) + vName.c_str() );
	} else {
		setValueRange( dvp -> getRange(), dvp -> getID() );
		return *this;
	}
}

//---------------------------------------------------------------------------
//	attempt to find the descriptor value the specimen value belongs to
//---------------------------------------------------------------------------

const LPDbDescriptor::Value * ValueField::findValue() const {
	const LPDbDescriptor * owner = findDescriptor();
	if( owner != NULL ) {
		if( valueID != 0 ) {
			return owner -> findValueByID( valueID );
		}
		if( !values.empty() ) {
			for( Range< LPDbDescriptor::Value > dvi = owner->getValues(); dvi.isValid(); ++ dvi ) {
				unsigned found = 0;
				std::set< std::string >::const_iterator vi = values.begin();
				do {
					if( dvi -> includes( *vi ) ) {
						found ++;
					}
				} while( ++vi != values.end() );
				if( found == values.size() ) {
					return &(*dvi);
				}
			}
		}
	}
	return NULL;
}

//---------------------------------------------------------------------------

const LPDbDescriptor::Value * ValueField::getDescriptorValue() {
	const LPDbDescriptor::Value *found = findValue();
	if( found != NULL ) {
		valueID = found->getID();
	}
	return found;
}

//---------------------------------------------------------------------------

const LPDbDescriptor::Value * ValueField::getDescriptorValue() const {
	return findValue();
}

//---------------------------------------------------------------------------

int ValueField::getValueID() const {
	if( valueID != 0 ) {
		return valueID;
	}
	const LPDbDescriptor::Value *found = findValue();
	if( found != NULL ) {
		return found->getID();
	}
	throw Exception( "Invalid descriptor value" );
}

//---------------------------------------------------------------------------
//	Return value(s) as a range; throw Exception if not applicable
//---------------------------------------------------------------------------

std::pair< std::string, std::string > ValueField::getRange() const {
	std::pair< std::string, std::string > result;
	std::set< std::string >::const_iterator si = values.begin();
	switch( values.size() ) {
		case 0:
			break;
		case 1:
			result.first = *si;
			result.second = *si;
			break;
		case 2:
			if( isRange ) {
				result.first = *si ++;
				result.second = *si;
				break;
			}
		default:
			throw Exception( "Invalid value range" );
	}
	return result;
}
//---------------------------------------------------------------------------

std::string ValueField::getValue() const {
	switch( values.size() ) {
		case 0:
			return "";
		case 1:
			return *(values.begin());
		default:
			throw Exception( "More than one value available" );
	}
}

//---------------------------------------------------------------------------
//	copy the given values but throw Exception if not the same field
//---------------------------------------------------------------------------

void ValueField::copy( const ValueField & other )
{
	if( fieldName.empty() ) {
		fieldName = other.fieldName;
	}
	else if( !other.fieldName.empty() && fieldName != other.fieldName ) {
		throw Exception( "Specimen fields do not match" );
	}
	if( descriptID == 0 ) {
		descriptID = other.getDescripID();
	}
	valueID = other.valueID;
	values = other.values;
	isRange = other.isRange;
}

//---------------------------------------------------------------------------
//	function object used to find the field with the given name
//---------------------------------------------------------------------------

class SameFieldName {
	const std::string & name;
public:
	SameFieldName( const std::string & s ) : name( s ) {}
	bool operator()( const ValueField & other ) const {
		return name.compare( other.getField() ) == 0;
	}
};

//---------------------------------------------------------------------------
//	extract a value if it has been set, otherwise return an empty string
//---------------------------------------------------------------------------

std::string ValueFields::getValue( const std::string & field ) const
{
	std::vector< ValueField >::const_iterator found
		= std::find_if( fields.begin(), fields.end(), SameFieldName( field ) );

	if( found == fields.end() ) {
		return "";
	} else {
		return found -> getValue();
	}
}

//---------------------------------------------------------------------------
//	find the value stored in the field for the given descriptor
//---------------------------------------------------------------------------

std::string ValueFields::getDescriptorValue( const std::string & descriptor ) const {
	const LPDbDescriptor * dp = LPDbDescriptors::records().findByName( descriptor );
	if( dp != NULL ) {
		std::vector< ValueField >::const_iterator found
			= std::find_if( fields.begin(), fields.end(), SameFieldName( dp -> getSpecimenField() ) );
		if( found != fields.end() )
			return found -> getValue();
	}
	return "";
}

//---------------------------------------------------------------------------
//	Add or update a field; throws Exception if
//---------------------------------------------------------------------------

void ValueFields::insert( const ValueField & vf ) {
	std::vector< ValueField >::iterator found
		= std::find_if( fields.begin(), fields.end(), SameFieldName( vf.getField() ) );
	if( found == fields.end() ) {
		fields.push_back( vf );
	} else {
		found -> copy( vf );
	}
}

//---------------------------------------------------------------------------

void ValueFields::insert( const LPDbDescriptor::Value & value ) {
	bool done = false;
	for( Range< LPDbDescriptor > dr = LPDbDescriptors::records(); dr.isValid() && !done; ++ dr ) {
		for( Range< LPDbDescriptor::Value > vr = dr -> getValues(); vr.isValid() && !done; ++ vr ) {
			if( vr -> getID() == value.getID() ) {
				insert( ValueField( *dr ).value( &(*vr) ) );
				done = true;
			}
		}
	}
	if( !done ) {
		throw Exception( "Descriptor value not recognised" );
	}
}

//---------------------------------------------------------------------------

ValueField & ValueFields::getValueField( const std::string & fieldName ) {
	std::vector< ValueField >::iterator found
		= std::find_if( fields.begin(), fields.end(), SameFieldName( fieldName ) );
	if( found == fields.end() ) {
		fields.push_back( fieldName );
		return fields.back();
	} else {
		return *found;
	}
}

//---------------------------------------------------------------------------
//	remove the selected pair from the list; return descriptor ID or 0
//---------------------------------------------------------------------------

ValueField ValueFields::remove( unsigned index ) {
	ValueField result;
	if( index < fields.size() ) {
		result.copy( fields[ index ] );
		fields.erase( fields.begin() + index );
	}
	return result;
}

//---------------------------------------------------------------------------

ValueField ValueFields::remove( const std::string & name ) {
	std::vector< ValueField >::iterator found
		= std::find_if( fields.begin(), fields.end(), SameFieldName( name ) );
	ValueField result;
	if( found != fields.end() ) {
		result.copy( *found );
		fields.erase( found );
	}
	return result;
}

//---------------------------------------------------------------------------
//	check if the current field values match the given filter
//---------------------------------------------------------------------------

bool ValueFields::matches( const ValueFields & filter ) const {
	for( Range< ValueField > dvi = filter.getValueFields(); dvi.isValid(); ++ dvi ) {
		static const xb36Code base36;
		if( base36.compare( getValue( dvi -> getField() ), dvi -> getRange() ) != 0 ) {
			return false;
		}
	}
	return true;
}

//---------------------------------------------------------------------------
//	remove any values that do not match the given filter
//---------------------------------------------------------------------------

void ValueFields::filter( const ValueFields & other ) {
	for( std::vector< ValueField >::iterator fi = fields.begin(); fi != fields.end(); ) {
		if( other.getValue( fi -> getField() ) == fi -> getValue() ) {
			fi ++;
		} else {
			fi = fields.erase( fi );
		}
	}
}

//---------------------------------------------------------------------------
//	add or overwrite this list's values with values from another
//---------------------------------------------------------------------------

void ValueFields::merge( const ValueFields & other ) {
	for( Range< ValueField > fr = other.fields; fr.isValid(); ++ fr ) {
		if( fr -> hasValue() ) {
			insert( *fr );
		}
	}
}

//---------------------------------------------------------------------------
//	Return an SQL WHERE clause that will filter the specimen table
//  NOTE: this may not work if the descriptor value ranges are base36
//---------------------------------------------------------------------------

std::string ValueField::makeWhereClause() const {
	std::string condition;
	std::set< std::string >::const_iterator st = values.begin();
	if( st != values.end() ) {
		if( isRange ) {
			condition = fieldName + " between \'" + *st;
			st ++;
			condition += "\' and \'" + *st + "\'";
		} else {
			condition = fieldName + " in (\'" + *st;
			while( (++st) != values.end() ) {
				condition += "\',\'" + *st;
			}
			condition += "\')";
		}
	}
	return condition;
}

//---------------------------------------------------------------------------

std::string ValueFields::makeWhereClause() const {
	std::string clause;
	for( std::vector< ValueField >::const_iterator vfi = fields.begin(); vfi != fields.end(); ++ vfi ) {
		std::string condition = vfi -> makeWhereClause();
		if( !condition.empty() ) {
			if( clause.empty() ) {
				clause = "WHERE " + condition;
			} else {
				clause = clause + " AND " + condition;
			}
		}
	}
	return clause;
}

//---------------------------------------------------------------------------
//	Initialise list from the "value" fields from given specimen record
//---------------------------------------------------------------------------

void ValueFields::addValueFields( const LQuery & query ) {
	char field[ 20 ];
	for( int n = 1; n < 32; n ++ ) {
		std::sprintf( field, "value%d", n );
		if( query.fieldExists( field ) ) {
			std::string val = query.readString( field );
			if( !val.empty() && val != "." ) {
				fields.push_back( ValueField( field ).value( val ) );
            }
		}
	}
}

//---------------------------------------------------------------------------
//	Find profile map entry uniquely matching the current value set
//---------------------------------------------------------------------------

const LPDbProfileMap * ValueFields::findProfileMap() const {
	const LPDbProfileMap * found;
	int count = 0;
	for( Range< LPDbProfileMap > pr = LPDbProfileMaps::records(); pr.isValid(); ++ pr )
	{
		if( pr -> matches( *this ) )
		{
			found = &(*pr);
			count ++;
		}
	}
	return count == 1 ? found : NULL;
}

//---------------------------------------------------------------------------


#ifndef ValueFieldsH
#define ValueFieldsH

#include <vector>
#include <set>
#include "LPDbDescriptor.h"

class LPDbProfileMap;

//---------------------------------------------------------------------------

class ValueField {
	friend class ValueFields;

	std::string fieldName;
	int descriptID, valueID;
	std::set< std::string > values;
	bool isRange;

	void clearValues();
	void setValueRange( const std::pair< std::string, std::string > & range, int id );
	void copy( const ValueField & other );
	std::string makeWhereClause() const;

	const LPDbDescriptor * findDescriptor() const;
	const LPDbDescriptor::Value * findValue() const;

public:

	ValueField() : descriptID( 0 ), valueID( 0 ), isRange( false ) {}
	ValueField( const std::string & field );
	ValueField( const LPDbDescriptor & dRef );

	// builders
	ValueField & field( const std::string & field );
	ValueField & descriptor( const std::string & dName );
	ValueField & value( const std::string & value );
	ValueField & valueName( const std::string & vName );
	ValueField & value( const LPDbDescriptor::Value * dvp );

	// may modify IDs behind the scenes
	const LPDbDescriptor * getDescriptor();
	int getDescripID();
	const LPDbDescriptor::Value * getDescriptorValue();

	// simple getters - may throw Exception
	const std::string & getField() const { return fieldName; }
	const LPDbDescriptor * getDescriptor() const;
	int getDescripID() const;
	bool hasValue() const { return !values.empty(); }
	int getValueID() const;
	const LPDbDescriptor::Value * getDescriptorValue() const;
	std::string getValue() const;
	std::pair< std::string, std::string > getRange() const;
};

//---------------------------------------------------------------------------

class ValueFields
{
	std::vector< ValueField > fields;

public:

	ValueFields() {}

	ValueFields( const std::string & fieldName, const std::string & val ) {
		fields.push_back( ValueField( fieldName ).value( val ) );
	}

	void clear() { fields.clear(); }
	Range< ValueField > getValueFields() const { return fields; }
	ValueField & getValueField( const std::string & fieldName );

	void insert( const ValueField & vf );
	void insert( const LPDbDescriptor::Value & value );
	void addValueFields( const LQuery & query );

	void setField( const std::string & fieldName, const std::string & val ) {
		insert( ValueField( fieldName ).value( val ) );
	}

	void setDescriptor( const std::string & name, const std::string & val ) {
		insert( ValueField().descriptor( name ).value( val ) );
	}

	ValueField remove( unsigned index );
	ValueField remove( const std::string & fieldName );

	void merge( const ValueFields & other );
	void filter( const ValueFields & other );
	std::string makeWhereClause() const;

	bool matches( const ValueFields & filter ) const;
	const LPDbProfileMap * findProfileMap() const;

	std::string getValue( const std::string & name ) const;
	std::string getDescriptorValue( const std::string & descriptor ) const;
};

//---------------------------------------------------------------------------

#endif


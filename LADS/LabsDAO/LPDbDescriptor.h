//---------------------------------------------------------------------------

#ifndef LPDbDescriptorH
#define LPDbDescriptorH

#include "LDbIdClasses.h"
#include "LDbNameBase.h"
#include "LDbCacheBase.h"

//---------------------------------------------------------------------------

class LPDbDescriptor : public LPDbID, public LDbNames
{
	std::string specimenField, mapField;
//	short parseOrder;
//	char delimiter;
	short srPosition, nsGrouping;
	std::pair< short, short > lengths;
	bool active, byHand, objectName;

	int getFlagBits() const;

public:

	enum Flags
	{
		DELETED = 99,			// no longer used, otherwise bit flags
		IN_SPECIMEN = 1,		// value in specimen table, not by hand
		BARCODE = 2,			// values extracted from barcode
		NO_VALUES = 4,			// no corresponding descriptor values
		NO_PROFILE = 8,			// not used used in profile map
		IS_OBJECT = 16			// values must exist in c_object_name
	};

	LPDbDescriptor( int id = 0 )
	 : LPDbID( id ), active( true ), byHand( false ), objectName( false ),
	   lengths( 1, 16 ), /* parseOrder( 0 ), */  srPosition( 0 ), nsGrouping( 0 )
	{}

	LPDbDescriptor( const LQuery & query );

	bool isInProfileMap() const { return !mapField.empty(); }
	const std::string & getMapField() const { return mapField; }
	void setMapField( const std::string & field ) { mapField = field; }

	const std::string & getSpecimenField() const { return specimenField; }
	void setSpecimenField( const std::string & field ) { specimenField = field; }

	bool isActive() const { return active; }
	void setActive( bool live ) { active = live; }

	bool isByHand() const { return byHand; }
//	bool isFromBarcode() const { return parseOrder > 0; }
	bool hasValues() const { return !valueList.empty(); }
	bool isObject() const { return objectName; }

	short getPosition() const { return srPosition; }
	void setPosition( short pos ) { srPosition = pos; }

	const std::pair< short, short > & getLengthRange() const { return lengths; }
	void setLengthRange( const std::pair< short, short > & range ) { lengths = range; }

//	short getParseOrder() const { return parseOrder; }
	short getGrouping() const { return nsGrouping; }

	class Value : public LPDbID
	{
		std::string name;
		std::pair< std::string, std::string > range;

	public:

		Value( int id = 0 ) : LPDbID( id ) {}
		Value( const LQuery & query );

		const std::string & getName() const { return name; }
		bool includes( const std::string & value ) const;
		const std::pair< std::string, std::string > & getRange() const { return range; }

		void setName( const std::string & s ) { name = s; }
		void setValue( const std::string & s ) {	range.first = range.second = s; }
		void setRange( const std::pair< std::string, std::string > & p ) { range = p; }
	};

	const LDbCache< Value > & getValues() const { return valueList; }
	void addValue( const Value & val ) { valueList.insert( val ); }
	void addValue( LQuery query, Value definition );
	void removeValue( LQuery query, int valueID );
	bool saveRecord( LQuery pQuery );

	const Value * findValueByID( int id ) const { return valueList.findByID( id ); }
	const Value * findValueByName( const std::string & name ) const;
	const Value * findValueByValue( const std::string & value ) const;
	const Value * findValueRange( const std::pair< std::string, std::string > & vr ) const;

private:

	LDbCache< Value > valueList;
};

//---------------------------------------------------------------------------

class LPDbDescriptors : public LDbCache< LPDbDescriptor >, public LPDbCacheMap< LPDbDescriptors >
{
	class ValueIDMatch;
	class ValueNameMatch;
	class FieldMatch;

	int getFieldNumber( const std::string & field ) const;

public:

	bool read( LQuery pQuery, bool readAll = true );

	const LPDbDescriptor * findByName( const std::string & name ) const;
	const LPDbDescriptor * findByField( const std::string & field ) const;
	const LPDbDescriptor * findByValueID( int valueID ) const;
	const LPDbDescriptor * findByValueName( const std::string & name ) const;

	std::string getNextMapField() const;
	std::string getNextSpecimenField() const;
};

//---------------------------------------------------------------------------

#endif


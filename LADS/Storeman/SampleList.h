//---------------------------------------------------------------------------

#ifndef SampleListH
#define SampleListH

#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
#include <Grids.hpp>
#include "LCDbObject.h"
#include "LDbSource.h"
#include "LPDbCryovial.h"

//---------------------------------------------------------------------------

class SampleList
{
	LPDbCryovials cryovials;
	LPDbSources specimens;

	const LPDbCryovial * findCryovial( LQuery & pQuery, const std::string & barcode, const LCDbObject & aliquot );
	const LPDbSource * findSpecimen( LQuery & pQuery, int sampleID );

public:

	class Entry
	{
		std::string barcode;
		int sourceID, aliquotType;
		ValueFields fields;
		TDateTime firstSeen;

	public:
		Entry( const std::string & cryovialID );
		Entry( const LPDbCryovial & cryovial );
		void setFields( const LPDbSource * specimen );
		const std::string & getBarcode() const { return barcode; }
		int getSourceID() const { return sourceID; }
		TDateTime whenFirstSeen() const { return firstSeen; }
		ValueFields & getFields() { return fields; }
		const ValueFields & getFields() const { return fields; }
		std::string getProfile() const;
	};

	void clear();
	void add( const std::string & barcode );
	void add( const LPDbCryovial & cryovial );
	void merge( LQuery pQuery, const ValueFields & shared );
	void save( LQuery pQuery, LDbSource::Stage stage );

	Range< Entry > getEntries() const { return entries; }

private:

	std::vector< Entry > entries;
};

//---------------------------------------------------------------------------
#endif

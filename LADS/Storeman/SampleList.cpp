//---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop

#include "SampleList.h"
#include "LPDbProfile.h"

//---------------------------------------------------------------------------
//	Store the barcode; source can be found if aliquot type defined later
//---------------------------------------------------------------------------

SampleList::Entry::Entry( const std::string & cryovialID )
 : barcode( cryovialID ), sourceID( 0 ), aliquotType( 0 )
{}

//---------------------------------------------------------------------------
//	Keep details from the cryovial; source and aliquot type are fixed
//---------------------------------------------------------------------------

SampleList::Entry::Entry( const LPDbCryovial & cryovial )
 : barcode( cryovial.getBarcode() ),
   sourceID( cryovial.getSampleID() ),
   aliquotType( cryovial.getAliquotType() )
{}

//---------------------------------------------------------------------------
//	Set or reset values from parent but add/overwrite aliquot type
//---------------------------------------------------------------------------

void SampleList::Entry::setFields( const LPDbSource * specimen ) {
	if( specimen == NULL ) {
		fields.clear();
		firstSeen = 0;
	} else {
		fields = *specimen;
		firstSeen = specimen -> getSwipeTime();
	}
	if( aliquotType != 0 ) {
		const LCDbObject & type = LCDbObjects::records().get( aliquotType );
		fields.setDescriptor( "aliquot_type", type.getName() );
	}
}

//---------------------------------------------------------------------------
//	look up the profile for this specimen entry, if any; return its name
//---------------------------------------------------------------------------

std::string SampleList::Entry::getProfile() const
{
	const LPDbProfileMap * pm = fields.findProfileMap();
	if( pm == NULL ) {
		return "Unknown";
	}
	const LPDbProfile * pp = LPDbProfiles::records().findByID( pm -> getProfileID() );
	if( pp == NULL ) {
		return "Invalid";
	}
	return pp -> getName();
}

//---------------------------------------------------------------------------
//	add a cryovial barcode to the end of the list of entries to create
//---------------------------------------------------------------------------

void SampleList::add( const std::string & barcode )
{
	entries.push_back( barcode );
}

//---------------------------------------------------------------------------

void SampleList::add( const LPDbCryovial & cryovial )
{
	entries.push_back( cryovial );
	cryovials.insert( cryovial );
}

//---------------------------------------------------------------------------

void SampleList::clear()
{
	entries.clear();
	cryovials.clear();
	specimens.clear();
}

//---------------------------------------------------------------------------
//	merge given descriptor values with those from the original specimen
//  if aliquot type specified use it to find source, if not already known
//---------------------------------------------------------------------------

void SampleList::merge( LQuery pQuery, const ValueFields & shared )
{
	const LCDbObject * type = NULL;
	std::string atNew = shared.getDescriptorValue( "aliquot_type" );
	if( !atNew.empty() ) {
		type = LCDbObjects::records().find( atNew, LCDbObject::ALIQUOT_TYPE );
	}
	for( std::vector< Entry >::iterator si = entries.begin(); si != entries.end(); ++ si ) {
		int sourceID = si -> getSourceID();
		if( type != NULL ) {
			const LPDbCryovial * cryovial = findCryovial( pQuery, si -> getBarcode(), *type );
			if( cryovial != NULL ) {
				sourceID = cryovial -> getSampleID();
			}
		}
		si -> setFields( findSpecimen( pQuery, sourceID ) );
		si -> getFields().merge( shared );
	}
}

//---------------------------------------------------------------------------
//	find the original cryovial entry, given barcode and aliquot type
//---------------------------------------------------------------------------

const LPDbCryovial * SampleList::findCryovial( LQuery & pQuery, const std::string & barcode, const LCDbObject & aliquot )
{
	const LPDbCryovial * cryovial = cryovials.find( barcode, aliquot.getID() );
	if( cryovial == NULL ) {
		cryovial = cryovials.readRecord( pQuery, barcode, aliquot.getID() );
	}
	return cryovial;
}

//---------------------------------------------------------------------------
//	find the original specimen entry, given the cryovial's source ID
//---------------------------------------------------------------------------

const LPDbSource * SampleList::findSpecimen( LQuery & pQuery, int sampleID )
{
	const LPDbSource * specimen = specimens.findByID( sampleID );
	if( specimen == NULL ) {
		specimen = specimens.readRecord( pQuery, sampleID );
	}
	return specimen;
}

//---------------------------------------------------------------------------
//	create specimen records for each entry, with the given status
//---------------------------------------------------------------------------

void SampleList::save( LQuery pQuery, LDbSource::Stage stage )
{
	for( std::vector< Entry >::iterator si = entries.begin(); si != entries.end(); ++ si ) {
		LPDbSource specimen( si -> getBarcode(), si -> whenFirstSeen(), 0 );
		specimen.merge( si -> getFields() );
		specimen.setStage( stage );
		specimen.createName( pQuery, "CRYOVIAL" );
		specimen.saveRecord( pQuery );
	}
}

//---------------------------------------------------------------------------


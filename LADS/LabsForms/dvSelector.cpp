/*---------------------------------------------------------------------------
 *
 *	Frame used to allow users to select descriptor/value pairs
 *
 *		20 Jan 2009   	Display most commonly used descriptors first
 *
 *--------------------------------------------------------------------------*/

#include <vcl.h>

#include "LCDbProject.h"
#include "LPDbDescriptor.h"
#include "LPDbProfileMap.h"
#include "dvSelector.h"

#pragma hdrstop

//---------------------------------------------------------------------------

#pragma package(smart_init)
#pragma resource "*.dfm"

TselectorFrame *selectorFrame;

//---------------------------------------------------------------------------

__fastcall TselectorFrame::TselectorFrame(TComponent* Owner)
 : TFrame( Owner ), modified( true ), showAll( true ), finished( true )
{}

//---------------------------------------------------------------------------

void TselectorFrame::prepare( bool allDescriptors )
{
	valid.clear();
	cbDescrip -> Clear();
	available.clear();
	lbDValues -> Clear();
	selected.clear();
	lbDVSelected -> Clear();
	finished = modified = true;
	showAll = allDescriptors;
}

//---------------------------------------------------------------------------
//	add a descriptor to the list the user can select from
//---------------------------------------------------------------------------

void TselectorFrame::allow( const LPDbDescriptor & descriptor )
{
	valid.insert( descriptor.getID() );
	selected.remove( descriptor.getSpecimenField() );
	available.insert( descriptor.getID() );
	finished = false;
}

//---------------------------------------------------------------------------
//	add a value to the selected list as if clicked on by the user
//---------------------------------------------------------------------------

void TselectorFrame::select( const ValueField & dvPair )
{
	selected.insert( dvPair );
	available.erase( dvPair.getDescripID() );
	updateDisplay();
}

//---------------------------------------------------------------------------
//	remove a descriptor from the list the user can select from
//---------------------------------------------------------------------------

void TselectorFrame::remove( const LPDbDescriptor & descriptor )
{
	selected.remove( descriptor.getSpecimenField() );
	valid.erase( descriptor.getID() );
	available.erase( descriptor.getID() );
	updateDisplay();
}

//---------------------------------------------------------------------------
//	return selected discriptor/value pairs; mark as up-to-date
//---------------------------------------------------------------------------

const ValueFields & TselectorFrame::getSelected()
{
	modified = false;
	return selected;
}

//---------------------------------------------------------------------------
//	show the descriptors that have yet to be used for the filter
//---------------------------------------------------------------------------

void __fastcall TselectorFrame::cbDescripDropDown(TObject *)
{
	cbDescrip -> Clear();
	for( Range< LPDbDescriptor > dr = LPDbDescriptors::records(); dr.isValid(); ++ dr )
		if( available.count( dr -> getID() ) != 0 )
			cbDescrip -> Items -> Add( dr -> getName().c_str() );
}

//---------------------------------------------------------------------------
//	list the allowed values for the current descriptor, if any
//---------------------------------------------------------------------------

void __fastcall TselectorFrame::cbDescripChange(TObject *)
{
	listValues( getDescriptor() );
}

//---------------------------------------------------------------------------

void TselectorFrame::listValues( const LPDbDescriptor * descrip )
{
	if( descrip == NULL )
		lbDValues -> Clear();
	else
	{	lbDValues -> Items -> Text = "(any value)";
		for( Range< LPDbDescriptor::Value > dv = descrip -> getValues(); dv.isValid(); ++ dv )
			lbDValues -> Items -> Add( dv -> getName().c_str() );
	}
}

//---------------------------------------------------------------------------
//  add a descriptor value to the filter when requested by user
//---------------------------------------------------------------------------

void __fastcall TselectorFrame::lbDValuesClick(TObject *)
{
	const LPDbDescriptor * dp = getDescriptor();
	if( dp != NULL )
	{
		const LPDbDescriptor::Value * dvp = dp -> findValueByName( getValueName() );
		available.erase( dp -> getID() );
		selected.insert( ValueField( *dp ).value( dvp ) );
	}
	updateDisplay();
}

//---------------------------------------------------------------------------

const LPDbDescriptor * TselectorFrame::getDescriptor() const
{
	if( cbDescrip -> Text.IsEmpty() )
		return NULL;
	std::string dn = AnsiString( cbDescrip -> Text ).c_str();
	return LPDbDescriptors::records().findByName( dn );
}

//---------------------------------------------------------------------------

bool TselectorFrame::setDescriptor( const std::string & name )
{
	const LPDbDescriptor * d = LPDbDescriptors::records().findByName( name );
	if( d == NULL || available.count( d -> getID() ) == 0 )
		return false;
	else
	{   listValues( d );
		cbDescrip -> Text = d -> getName().c_str();
		return true;
	}
}

//---------------------------------------------------------------------------

std::string TselectorFrame::getValueName() const
{
	AnsiString result;
	if( lbDValues -> ItemIndex >= 0 )
		result = lbDValues -> Items -> Strings[ lbDValues -> ItemIndex ];
	return  result.c_str();
}

//---------------------------------------------------------------------------
//	deselect descriptor/value pair so user can select a new value
//---------------------------------------------------------------------------

void __fastcall TselectorFrame::lbDVSelectedClick(TObject *)
{
	ValueField vf = selected.remove( lbDVSelected -> ItemIndex );
	int descriptorID = vf.getDescripID();
	if( valid.count( descriptorID ) != 0 )
		available.insert( descriptorID );
	updateDisplay();
}

//---------------------------------------------------------------------------
//	deselect all values and display the first descriptor again
//---------------------------------------------------------------------------

void __fastcall TselectorFrame::btnClearClick(TObject *)
{
	selected.clear();
	cbDescrip -> Clear();
	available = valid;
	updateDisplay();
}

//---------------------------------------------------------------------------
//	update display (if necessary) to show current descriptor's values
//---------------------------------------------------------------------------

void TselectorFrame::updateDisplay()
{
	const LPDbDescriptor * descrip, * displayed = getDescriptor();
	if( displayed != NULL && available.count( displayed -> getID() ) > 0 )
		descrip = displayed;
	else
	{	int nextID = nextSharedDID();
		if( nextID == 0 )
			descrip = NULL;
		else
			descrip = LPDbDescriptors::records().findByID( nextID );
	}

	if( descrip == NULL )
	{
		finished = true;
		cbDescrip -> Clear();
		lbDValues -> Clear();
	}
	else
	{	finished = false;
		cbDescrip -> Text = descrip -> getName().c_str();
		listValues( descrip );
		lbDValues -> SetFocus();
	}

	lbDVSelected -> Clear();
	for( Range< ValueField > dvp = selected.getValueFields(); dvp.isValid(); ++ dvp )
	{
		std::string text = dvp -> getDescriptor() -> getName();
		const LPDbDescriptor::Value * value = dvp -> getDescriptorValue();
		if( value == NULL ) {
			text += " = (any value)";
		} else {
			text += " = ";
			text += value -> getName();
		}
		lbDVSelected -> Items -> Add( text.c_str() );
	}
	modified = true;
}

//---------------------------------------------------------------------------
//	check descriptor IDs to determine which should be used next
//---------------------------------------------------------------------------

int TselectorFrame::nextSharedDID()
{
	// find profile maps that include the descriptor values selected so far
	std::vector< int > used;
	for( Range< LPDbProfileMap > pmr = LPDbProfileMaps::records(); pmr.isValid(); ++ pmr )
	{
		if( pmr -> matches( selected ) )
		{
			std::map< int, int > dvp = pmr -> getDVPairs();
			for( std::map< int, int >::const_iterator dvi = dvp.begin(); dvi != dvp.end(); ++ dvi )
				used.push_back( dvi -> first );
		}
	}

	// if all descriptors should be included, allow first even if not in a map
	int nextID = 0, found = showAll ? -1 : 0;
	for( std::set< int >::const_iterator fdi = available.begin(); fdi != available.end(); ++ fdi )
	{
		// count how many times each descriptor is used in the selected maps
		int count = 0;
		for( std::vector< int >::const_iterator ui = used.begin(); ui != used.end(); ++ ui )
			if( *ui == *fdi )
				count ++;

		// keep id of first descriptor and/or the one that's in the most maps
		if( count > found )
		{
			nextID = *fdi;
			found = count;
		}
   }
   return nextID;
}

//---------------------------------------------------------------------------


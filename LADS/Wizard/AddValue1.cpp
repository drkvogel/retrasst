//---------------------------------------------------------------------------
#include <vcl.h>

#include "AddValue1.h"
#include "Descriptor1.h"
#include "ObjectName1.h"
#include "LCDbProject.h"
#include "LQuery.h"
#include "Login1.h"
#include "LCDbObject.h"
#include "xb36Code.h"

#pragma hdrstop

//---------------------------------------------------------------------------

#pragma package(smart_init)
#pragma resource "*.dfm"

TAddValue *AddValue;

//---------------------------------------------------------------------------

__fastcall TAddValue::TAddValue(TComponent* Owner)
 : TForm( Owner ), valueID( 0 )
{}

//---------------------------------------------------------------------------

void TAddValue::setDescriptor( int descripID )
{
	owner = LPDbDescriptors::records().get( descripID );
	valueID = 0;
}

//---------------------------------------------------------------------------

void TAddValue::setValue( int dvID )
{
	const LPDbDescriptor * descrip = LPDbDescriptors::records().findByValueID( dvID );
	if( descrip == NULL )
	{
		owner = LPDbDescriptor( 0 );
		valueID = 0;
	}
	else
	{	owner = *descrip;
		valueID = dvID;
	}
}

//---------------------------------------------------------------------------

void __fastcall TAddValue::FormActivate(TObject *Sender)
{
	String function;
	const LPDbDescriptor::Value * theValue = owner.findValueByID( valueID );
	if( theValue == NULL )
	{
		ValueName->Text = "";
		Minimum->Text = "";
		Maximum->Text = "";
		function = "Add";
	}
	else
	{	ValueName->Text = theValue -> getName();
		std::pair< String, String > range = theValue -> getRange();
		Minimum->Text = range.first;
		Maximum->Text = range.second;
		function = "Modify";
	}
	Caption = function + " value for " + Login -> setProjectCaption( owner.getName() );
}

//---------------------------------------------------------------------------

void __fastcall TAddValue::MinimumExit(TObject *Sender)
{
	if( Maximum->Text.IsEmpty() )
		Maximum->Text = Minimum->Text;
}

//---------------------------------------------------------------------------

void __fastcall TAddValue::btnSaveClick(TObject *Sender)
{
	if( ValueName -> Text.IsEmpty() )
		Application->MessageBox( "Invalid Value name", NULL, MB_OK );

	else if( CheckValues( Minimum->Text, Maximum->Text )
	 && CheckLength( Minimum->Text ) && CheckObject( Minimum->Text )
	 && CheckLength( Maximum->Text ) && CheckObject( Maximum->Text ) )
	{
		LPDbDescriptor::Value theValue( valueID );
		theValue.setName( ValueName->Text );
		theValue.setRange( std::pair< String, String >( Minimum->Text, Maximum->Text ) );
		owner.addValue( QueryProject, theValue );
		Close();
	}
}

//---------------------------------------------------------------------------

bool TAddValue::CheckValues( AnsiString strMinimum, AnsiString strMaximum )
{
	if( strMinimum != strMaximum )
	{
		if( owner.getSource() == "byhand" )
		{
			Application->MessageBox( "Minimum should equal maximum"
									" for byhand descriptors", NULL, MB_OK );
			return false;
		}

		unsigned __int64 outMin = 0, outMax = 0;
		try
		{	static xb36Code base36;
			outMin = base36.xb36Encode( strMinimum );
			outMax = base36.xb36Encode( strMaximum );
		}
		catch( ... )
		{
			Application->MessageBox( "Minimum and maximum should be valid base 36",
									 NULL, MB_OK );
			return false;
		}

		if( outMin > outMax )
		{
			Application->MessageBox( "Minimum must be less than maximum",
									 NULL, MB_OK );
			return false;
		}
	}
	return true;
}

//---------------------------------------------------------------------------

bool TAddValue::CheckLength( AnsiString value )
{
	std::pair< short, short > lengths = owner.getLengthRange();
	if( value.Length() < lengths.first )
	{
		Application->MessageBox( "Value has too few characters", NULL, MB_OK );
		return false;
	}
	else if( value.Length() > lengths.second )
	{
		Application->MessageBox( "Value has too many characters", NULL, MB_OK );
		return false;
	}
	else return true;
}

//---------------------------------------------------------------------------

bool TAddValue::CheckObject( AnsiString value )
{
	LCDbObject::Category type = owner.getObjectType();
	if( type == LCDbObject::UNKNOWN )
		return true;

	const LCDbObject * obj = LCDbObjects::records().find( value, type );
	if( obj != NULL )
		return true;

	String message = "Value does not match " + LCDbObject::findDescription( type )
				   + " in the central database";
	Application -> MessageBox( message.c_str(), NULL, MB_OK );
	return false;
}

//---------------------------------------------------------------------------

void __fastcall TAddValue::btnCancelClick(TObject *Sender)
{
	Close();
}

//---------------------------------------------------------------------------





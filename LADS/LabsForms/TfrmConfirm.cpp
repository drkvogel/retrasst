/*---------------------------------------------------------------------------
 *
 *	LIMS Security and Auditting sign-off screen - use as is or extend
 *
 *		29 July 2008, NG:	Add summary message, set by calling application
 *      13 January 09, NG:	Only enable OK button when user name is valid
 *
 *--------------------------------------------------------------------------*/

#include <vcl.h>

#include "TfrmConfirm.h"
#include "LCDbOperator.h"
#include "LCDbProject.h"
#include "LCDbAuditTrail.h"

#pragma hdrstop

//---------------------------------------------------------------------

#pragma resource "*.dfm"

TfrmConfirm *frmConfirm;

//---------------------------------------------------------------------

__fastcall TfrmConfirm::TfrmConfirm(TComponent* AOwner) : TForm(AOwner)
{}

//---------------------------------------------------------------------
//	List operators allowed to run program; select logged-in user
//---------------------------------------------------------------------

void TfrmConfirm::initialise( short stage, const std::string & summary )
{
	std::set< int > projects;
	projects.insert( LCDbProjects::getCurrentID() );
	initialise( stage, summary, projects );
}

void TfrmConfirm::initialise( short stage, const std::string & summary, const std::set< int > & projects )
{
	cbUserNames -> Clear();
	int current = -1;
	for( Range< LCDbOperator > user = LCDbOperators::records(); user.isValid(); ++ user )
	{
		unsigned count = 0;
		for( std::set< int >::const_iterator pi = projects.begin(); pi != projects.end(); ++ pi )
			if( user -> canRun( *pi, 0, stage ) )
				count ++;

		if( count == projects.size() )
		{
			if( user -> getID() == LCDbOperators::getCurrentID() )
				current = cbUserNames -> Items -> Count;
			cbUserNames -> Items -> Add( user -> getDescription().c_str() );
		}
	}

	cbUserNames -> ItemIndex = current;
	edtPassword -> Text = "";
	lblSummary -> Caption = summary.c_str();
}

//---------------------------------------------------------------------
//	Remove users with the give IDs (before displaying the form)
//---------------------------------------------------------------------

void TfrmConfirm::remove( const std::set< int > & userIDs )
{
	for( std::set< int >::const_iterator fi = userIDs.begin(); fi != userIDs.end(); fi ++ )
	{
		const LCDbOperator * user = LCDbOperators::records().findByID( *fi );
		if( user != NULL )
		{   String name =  user -> getDescription().c_str();
			for( int i = 0; i < cbUserNames -> Items -> Count; i ++ )
			{
				if( cbUserNames -> Items -> Strings[ i ].CompareIC( name ) == 0 )
					cbUserNames -> Items -> Delete( i );
			}
		}
	}
}

//---------------------------------------------------------------------
//	Check user name and password; log new user in if necessary
//---------------------------------------------------------------------

void __fastcall TfrmConfirm::userDetailChange(TObject *)
{
	BitBtn1 -> Enabled = !edtPassword -> Text.IsEmpty()
					&& (cbUserNames -> Items -> IndexOf( cbUserNames -> Text ) >= 0);
}

//---------------------------------------------------------------------------

void __fastcall TfrmConfirm::BitBtn1Click(TObject *)
{
	AnsiString name = cbUserNames -> Text.Trim();
	AnsiString pwd = edtPassword -> Text.Trim();
	LCDbOperators & users = LCDbOperators::records();
	const LCDbOperator * user = users.check( name.c_str(), pwd.c_str() );
	edtPassword -> Clear();
	if( user != NULL ) {
		if( user -> getID() != LCDbOperators::getCurrentID() ) {
        	users.setCurrent( *user );
			LCDbAuditTrail::getCurrent().start( "New user logging in to sign off process" );
		}
		ModalResult = mrOk;
	}
}

//---------------------------------------------------------------------


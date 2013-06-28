/*---------------------------------------------------------------------------
 *
 *		6 Aug 09, NG: 	split out from confirmation screen
 *
 *--------------------------------------------------------------------------*/

#include <vcl.h>

#include "TfrmQuestion.h"
#include "LCDbOperator.h"
#include "LCDbProject.h"

#pragma hdrstop

//---------------------------------------------------------------------

#pragma resource "*.dfm"

TfrmQuestion *frmQuestion;

//---------------------------------------------------------------------

__fastcall TfrmQuestion::TfrmQuestion(TComponent* AOwner) : TForm(AOwner)
{}

//---------------------------------------------------------------------

void TfrmQuestion::setDetail( const String & text )
{
	lblDetail -> Caption = text;
	lblDetail -> Width = Width - 2 * lblDetail -> Left;
}

//---------------------------------------------------------------------

bool TfrmQuestion::confirm( const String & text, const String & okText, const String & cancel )
{
	lblSummary -> Caption = text;
	okBtn -> Caption = okText;
	cancelBtn -> Caption = cancel;
	return ShowModal() == mrOk;
}

//---------------------------------------------------------------------

bool TfrmQuestion::confirmExit()
{
	setDetail( "All changes will be lost" );
	return confirm( "Are you sure you want to exit?", "Yes", "No" );
}

//---------------------------------------------------------------------


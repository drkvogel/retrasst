//---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop

#include "FormCommon.h"
#include "FormUtils.h"
#include "FormFront.h"
#include "FormEnterResults.h"
#include "LCDbAnalyser.h"
#include "LCDbTest.h"
#include "LDbWorklist.h"
#include "TfrmAboutBox.h"
#include "LeaseIDs.h"
#include "LCDbObject.h"
#include "LIMSParams.h"

//---------------------------------------------------------------------------

#pragma package(smart_init)
#pragma link "IdExplicitTLSClientServerBase"
#pragma link "IdSMTPBase"
#pragma resource "*.dfm"

TfrmCommon *frmCommon;

const char * TfrmAboutBox::author = "Nicholas Goodwin";
const char * TfrmAboutBox::compiled = __DATE__;
const char * TfrmAboutBox::extension = "x3532";
const char * LCDbAuditTrail::default_email = "Nicholas.Goodwin@ctsu.ox.ac.uk";
const int LCDbProjects::leaseID = LEASEE_DEFAULT;

//---------------------------------------------------------------------------
__fastcall TfrmCommon::TfrmCommon(TComponent* Owner) : TForm(Owner), auditTrail(qryLabCentral)
{

	if (!MYDEBUG) {
		LCDbProjects::changeDbName( dbCentral, "ldbc" );
		frmEnterResults->OpenDialog4->InitialDir = "K:\\LABS\\HPLC\\HPLC exports";
		frmFront->Caption = frmFront->Caption + "  **live**";
	} else {
		LCDbProjects::changeDbName( dbCentral, "t_ldbc" );
		frmFront->Caption = frmFront->Caption + "  **test**";
	}

	LCDbProjects::records().read(qryLabCentral,false);
	LCDbTests::records().read(qryLabCentral,false);    
	LCDbObjects & names = LCDbObjects::records();
	names.read( qryLabCentral, false );
	LCDbAnalysers & analysers = LCDbAnalysers::records();
	analysers.read( qryLabCentral, false );

	LIMSParams & params = LIMSParams::instance();
	if( params.checkUnique() && params.checkMachine() && params.checkDiskSpace() )
	{
		const LCDbAnalyser * analyser = analysers.findByID( LCDbAnalysers::getCurrentID() );
		if( analyser != NULL )
			frmFront->Caption = frmFront->Caption + ": " + analyser -> getName();
	}
	auditTrail.start();
}

//---------------------------------------------------------------------------



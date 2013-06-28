//---------------------------------------------------------------------------
#include <vcl.h>
#pragma hdrstop

#include <stdio.h>
#include "LeaseMod1.h"
#include "lease.h"
#include "DbShared.h"
#include "LogMod1.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"
TLeaseMod *LeaseMod;
//---------------------------------------------------------------------------
__fastcall TLeaseMod::TLeaseMod(TComponent* Owner)
    : TDataModule(Owner)
{
	LeaseManager = NULL;
	ProjectLeasePid = -1;			// PROJECT ID
	CentralLeasePid = -1;			// PROJECT ID

    LeaseTid = LEASEE_WIZARD;
    LeaseLevel = LEASE_TYPE_TOTAL;
    LeaseDescrip = "Wizard";
}
//---------------------------------------------------------------------------
void TLeaseMod::SetUpLeaseManager( TDatabase* database )
{
    // set up lease manager
    TDateTime	tsys, tcen;
    DeleteLeaseManager();
    LeaseManager = new TLeaseManager(
        database->AliasName,"LeaseMan - Wizard",database->Params->Text );

//            bool sync_ok = LeaseManager->synch_time( tsys, tcen );
//            String	msg = String("System time: ") + tsys.DateTimeString()
//                + "\nCentral time: " + tcen.DateTimeString()
//                + "\n\nSynchronised " + ( sync_ok ? "YES" : "NO" );
//            Application->MessageBox( msg.c_str(), "Time Check", MB_OK );

    CentralLeasePid = 0; // 0 for central database
    // clear any existing leases for the central database
    LeaseManager->Deactivate( CentralLeasePid, LeaseTid );
    LeaseManager->SetDescript( LeaseDescrip );
}
//---------------------------------------------------------------------------
bool TLeaseMod::ActivateCentralLease()
{
    bool activated = true;
    AnsiString msg;
    char tmp[2000];
    int length = 5; // 5 minutes

    // 0 if available, else number of minutes until available
	int	avail = LeaseManager->Activate( CentralLeasePid, LeaseTid,
        length, msg, LeaseLevel );
    if ( avail != 0 )
    {
        activated = false;
    	sprintf( tmp, "%s\n Proj %d, task %d, active = %d, msg=\"%s\"",
            "Central database not currently available.",
	    	CentralLeasePid, LeaseTid, avail, msg.c_str() );
        Application->MessageBox(
            tmp,
            "Wizard Message", MB_OK );
        AnsiString strTmp = tmp;
        LogMod->LogCentralDb( TLogMod::LOGLOCAL, "Leasing problem: " + strTmp );
    }
    return activated;
}
//---------------------------------------------------------------------------
bool TLeaseMod::ActivateProjectLease()
{
    bool activated = true;
    AnsiString msg;
    char tmp[2000];
    int length = 5; // 5 minutes

    // 0 if available, else number of minutes until available
	int	avail = LeaseManager->Activate(
        ProjectLeasePid, LeaseTid, length, msg,
        LeaseLevel );
    if ( avail != 0 )
    {
        activated = false;
    	sprintf( tmp, "%s\n Proj %d, task %d, active = %d, msg=\"%s\"",
            "Project database not currently available.", ProjectLeasePid,
            LeaseTid, avail, msg.c_str() );
        Application->MessageBox(
            tmp,
            "Wizard Message", MB_OK );
        AnsiString strTmp = tmp;
        LogMod->LogProjectDb( TLogMod::LOGLOCAL, "Leasing problem: " + strTmp );
    }
    return activated;
}
//---------------------------------------------------------------------------
void TLeaseMod::DeactivateCentralLease()
{
    LeaseManager->Deactivate( CentralLeasePid, LeaseTid );
}
//---------------------------------------------------------------------------
void TLeaseMod::DeactivateProjectLease()
{
    LeaseManager->Deactivate( ProjectLeasePid, LeaseTid );
}
//---------------------------------------------------------------------------
void TLeaseMod::DeleteLeaseManager()
{
    if ( LeaseManager != NULL )
    {
        delete LeaseManager;
        LeaseManager = NULL;
    }
}
//---------------------------------------------------------------------------


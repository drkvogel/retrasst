//---------------------------------------------------------------------------
#include <vcl.h>
#pragma hdrstop

#include "DataMod1.h"
#include "LogMod1.h"
#include "lease.h"
#include "DbShared.h"
#include "LeaseMod1.h"
#include "ProjectClass.h"
#include "Prompt.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"
TDataMod *DataMod;
//---------------------------------------------------------------------------
__fastcall TDataMod::TDataMod(TComponent* Owner)
    : TDataModule(Owner)
{
    SystemLive = true;
    DatabaseType = "Ingres";
    CentralDbName = "unknown";
    ProjectDbName = "unknown";
    ProjectDdbName = "unknown";
}
//---------------------------------------------------------------------------
bool TDataMod::OpenCentralDb( bool sl )
{
    bool status = false;
    AnsiString SystemType;

    CloseCentralDb();

    SystemLive = sl;

    if ( SystemLive )
    {
        // operational database
        CentralDbName = "ldbc";
    }
    else
    {
        // test database
//        frmPrompt->setPrompt( "Enter database name:" );
//        frmPrompt->setValue( "t_ldbc" );
//        frmPrompt->setValue( "t_ldbc_demo" );
//        frmPrompt->ShowModal();
//        CentralDbName = frmPrompt->getValue();
        CentralDbName = "t_ldbc";
    }

    if ( CentralDbName != "" )
    {
        bool central = true;
        OpenDatabase( CentralDbName, DatabaseCentral, QueryCentral, central );
        theCentral = new ProjectClass;
        theCentral->LoadFromDatabaseByDbName( QueryCentral, CentralDbName );
        LogMod->StartLogDb( SystemLive, DatabaseCentral,QueryCentral );
        LeaseMod->SetUpLeaseManager( DatabaseCentral );
        if ( isValidCentralDbVersion() )
            status = true;
    }
    return status;
}
//---------------------------------------------------------------------------
bool TDataMod::OpenDatabase( AnsiString dbName, TDatabase* dbComponent,
    TQuery* query, bool central )
{
    AnsiString databaseType = "project";
    if ( central ) databaseType = "central";

    bool databaseFound = false;
    while ( !databaseFound )
    {
        databaseFound = true;
        try
        {
            DbShared::changeDbName( dbName, dbComponent );
        }
        catch(Exception& e)
        {
            AnsiString message = databaseType + " database cannot be accessed.";
            if ( central )
            {
                message +=  "\n"
                    "Ingres may not have been started.\n"
                    "Start Ingres, then press OK.";
                Application->MessageBox(
                    message.c_str(),
                    "DataMod Error",MB_OK );
            }
            else
            {
                Application->MessageBox(
                    message.c_str(),
                    "DataMod Error",MB_OK );
                return false;
            }
            databaseFound = false;
        }
    }

    try
    {
        dbComponent->Open();
    }
    catch(Exception& e)
    {
        AnsiString strError =
            "Error opening " + databaseType + " database.\n" + e.Message;
        Application->MessageBox(
            strError.c_str(),
            "DataMod Error",MB_OK);
        if ( central )
        {
            LogMod->LogCentralDb( TLogMod::LOGLOCAL, strError );
        }
        else
        {
            LogMod->LogProjectDb( TLogMod::LOGLOCAL, strError );
        }
        return false;
    }
    if ( DatabaseType == "Ingres" )
    {
        query->SQL->Clear();
        query->SQL->Add("SET AUTOCOMMIT ON");
        query->ExecSQL();
        query->SQL->Clear();
    }
    return true;
}
//---------------------------------------------------------------------------
bool TDataMod::isValidCentralDbVersion()
{
    // check that the central database's version is at least v2.1 and
    //    no greater than v2.2
    bool valid_project = false;

    if ( !theCentral->isBeforeVersion( 2, 1 ) &&
        theCentral->isBeforeVersion( 2, 3 ) )
        valid_project = true;

    return valid_project;
}
//---------------------------------------------------------------------------
void TDataMod::CloseCentralDb()
{
    LogMod->StopLogDb( SystemLive );
    LeaseMod->DeleteLeaseManager();
    DatabaseCentral->Connected = false;
    if ( theCentral != NULL )
    {
        delete theCentral;
        theCentral = NULL;
    }
}
//---------------------------------------------------------------------------

bool TDataMod::OpenProjectDb( AnsiString projName )
{
    bool validProject = false;
    bool central = false;
    CloseProjectDb();
    ProjectName = projName;
    theProject = new ProjectClass;

    if ( ValidateProject() )
    {
        ProjectDbName = theProject->GetDatabaseName();
        if ( OpenDatabase( ProjectDbName, DatabaseProject,
            QueryProject, central ) )
        {
            LogMod->LogProjectDb( TLogMod::LOGINFO,
                "Connected to project " + theProject->GetName() +
                ", database " + theProject->GetDatabaseName() );
            // open distributed database
            ProjectDdbName = ProjectDbName + "_ddb";
            validProject = OpenDatabase(
                ProjectDdbName, DatabaseProjectDdb, QueryProjectDdb, central );
        }
    }
    else
    {
        CloseProjectDb();
    }
    return validProject;
}
//---------------------------------------------------------------------------
bool TDataMod::ValidateProject()
{
    bool valid_project = false;

    if ( ProjectName == "" )
    {
        Application->MessageBox(
            "Blank project\n"
            "Enter valid project.",
            "Login Error",MB_OK);
    }
    else
    {
        if ( theProject->LoadFromDatabaseByName( QueryCentral, ProjectName ) )
            valid_project = isValidProjectDbVersion();
    }
    return valid_project;
}
//---------------------------------------------------------------------------
bool TDataMod::isValidProjectDbVersion()
{
    // check that the project database's version is at least v2.0 and
    //    no greater than v2.2
    bool valid_project = false;

    if ( !theProject->isBeforeVersion( 2, 0 ) &&
        theProject->isBeforeVersion( 2, 3 ) )
        valid_project = true;

    return valid_project;
}
//---------------------------------------------------------------------------
void TDataMod::CloseProjectDb()
{
    if ( theProject != NULL )
    {
        LogMod->LogProjectDb( TLogMod::LOGINFO,
            "Disconnected from project " + theProject->GetName() +
            ", database " + theProject->GetDatabaseName() );
        DatabaseProject->Connected = false;
        DatabaseProjectDdb->Connected = false;
        delete theProject;
        theProject = NULL;
    }
    ProjectName = "";
    ProjectDbName = "";
    ProjectDdbName = "";
}
//---------------------------------------------------------------------------
AnsiString TDataMod::SetCaption( AnsiString Caption )
{
    if ( !SystemLive )
    {
        Caption = Caption + " (TEST Database)";
    }
    return Caption;
}
//---------------------------------------------------------------------------
AnsiString TDataMod::SetProjectCaption( AnsiString Caption )
{
    if ( !SystemLive )
    {
        Caption = Caption + " (TEST Database)";
    }
    if ( (DataMod->GetProject())->isStatusTest() )
    {
        Caption = Caption + " (TEST Project)";
    }
    else if ( (DataMod->GetProject())->isStatusDead() )
    {
        Caption = Caption + " (OBSOLETE Project)";
    }
    return Caption;
}
//---------------------------------------------------------------------------


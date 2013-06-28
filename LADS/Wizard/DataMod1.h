//---------------------------------------------------------------------------
#ifndef DataMod1H
#define DataMod1H
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
#include <Db.hpp>
#include <DBTables.hpp>
//---------------------------------------------------------------------------
class TLeaseManager;
class ProjectClass;
class TDataMod : public TDataModule
{
__published:	// IDE-managed Components
    TQuery *QueryProject;
    TQuery *QueryCentral;
    TQuery *QueryCentral2;
    TDatabase *DatabaseProject;
    TDatabase *DatabaseCentral;
    TDatabase *DatabaseProjectDdb;
    TQuery *QueryProjectDdb;
private:	// User declarations
    bool SystemLive;
    AnsiString DatabaseType; // always Ingres
    ProjectClass* theCentral;
    AnsiString CentralDbName;
    ProjectClass* theProject;
    AnsiString ProjectName;
    AnsiString ProjectDbName;
    AnsiString ProjectDdbName;

    bool ValidateProject();
    bool OpenDatabase( AnsiString dbName, TDatabase* dbComponent,
        TQuery* query, bool central );
    bool isValidCentralDbVersion();
    bool isValidProjectDbVersion();

public:		// User declarations
    __fastcall TDataMod(TComponent* Owner);
    bool IsSystemLive() { return SystemLive; }

    bool OpenCentralDb( bool sl );
    void CloseCentralDb();
    bool OpenProjectDb( AnsiString projName );
    void CloseProjectDb();

    AnsiString GetCentralDbName() { return CentralDbName; }
    ProjectClass* GetCentral() { return theCentral; }
    ProjectClass* GetProject() { return theProject; }

    AnsiString SetCaption( AnsiString Caption );
    AnsiString SetProjectCaption( AnsiString Caption );
};
//---------------------------------------------------------------------------
extern PACKAGE TDataMod *DataMod;
//---------------------------------------------------------------------------
#endif

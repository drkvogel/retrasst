//---------------------------------------------------------------------------

#ifndef LogMod1H
#define LogMod1H
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
#include <stdio.h>
#include <DbTables.hpp>
#include "DbAuditTrail.h"
class ProjectClass;
class DbAuditTrail;
//---------------------------------------------------------------------------
class TLogMod : public TDataModule
{
__published:	// IDE-managed Components
private:	// User declarations
    bool loggingStarted;
    bool auditStarted;
    FILE* LogFile;
    DbAuditTrail AuditTrail;
    void LogDb_LogToFile( int type, AnsiString dbName, AnsiString text );

    static const int ACTIVE_STATUS;
    static const int NEW_STATUS;
    static const int UPDATE_STATUS;
    static const int DELETE_STATUS;

public:		// User declarations
    __fastcall TLogMod(TComponent* Owner);
    static const int LOGLOCAL;   // write to Wizard.log only
    static const int LOGINFO;    // information only ( write to all logs and audit trail )
    static const int LOGERROR;   // error message ( write to all logs and audit trail )

    void StartLogDb( bool LiveSystem, TDatabase* DatabaseCentral,
        TQuery* QueryCentral );
    void StopLogDb( bool LiveSystem );
    void LogProjectDb( int type, TQuery* query );
    void LogProjectDb( int type, AnsiString text );
    void LogCentralDb( int type, TQuery* query );
    void LogCentralDb( int type, AnsiString text );
    void LogOperator( int operatorID );
    void Commit( TQuery* query );
    int GetProcessID();

};
//---------------------------------------------------------------------------
extern PACKAGE TLogMod *LogMod;
//---------------------------------------------------------------------------

#endif



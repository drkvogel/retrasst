//---------------------------------------------------------------------------

#ifndef ProjectClassH
#define ProjectClassH
//---------------------------------------------------------------------------

class ProjectClass : public TObject{

public:
    ProjectClass();
    virtual __fastcall ~ProjectClass();

    //set get ProjectID
    int GetProjectID(){ return ProjectID;}
    void SetProjectID(int pid){ ProjectID = pid; }

    // set get theName
    AnsiString GetName(){ return theName; }
    void SetName(AnsiString name){ theName = name; }

    // set get FullName
    AnsiString GetFullName(){ return FullName; }
    void SetFullName(AnsiString fname){ FullName = fname; }

    // set get ValidFrom
    AnsiString GetValidFrom(){ return ValidFrom; }
    void SetValidFrom(AnsiString from){ ValidFrom = from; }

    // set get ValidTo
    AnsiString GetValidTo(){ return ValidTo; }
    void SetValidTo(AnsiString to){ ValidTo = to; }

    //set get ProjectStatus
    int GetProjectStatus(){ return Status;}
    void SetProjectStatus(int status){ Status = status; }
    bool isStatusLive() { return ( Status == PROJECTLIVE ); }
    bool isStatusTest() { return ( Status == PROJECTTEST ); }
    bool isStatusDead() { return ( Status == PROJECTDEAD ); }

    //set get DbVersion
    int GetDbMajorVersion(){ return DbMajorVersion;}
    void SetDbMajorVersion(int vers){ DbMajorVersion = vers; }

    //set get DbMinorVersion
    int GetDbMinorVersion(){ return DbMinorVersion;}
    void SetDbMinorVersion(int mvers){ DbMinorVersion = mvers; }

    //set get ProjectStatusName
    AnsiString GetProjectStatusName(){ return StatusName;}

    // set get DatabaseName
    AnsiString GetDatabaseName(){ return DatabaseName; }
    void SetDatabaseName(AnsiString dbname){ DatabaseName = dbname; }

    // set get URL
    AnsiString GetURL(){ return URL; }
    void SetURL(AnsiString url){ URL = url; }

    // set get NoteExists
    short GetNoteExists() { return NoteExists; }
    void SetNoteExists(short exists){ NoteExists = exists; }

    bool LoadFromDatabase(TQuery* query, int recno);
    bool LoadFromDatabaseByName(TQuery* query, AnsiString name );
    bool LoadFromDatabaseByDbName(TQuery* query, AnsiString dbName);
    bool LoadRecord( TQuery* query );
    bool InsertToDatabase(TQuery* query);
    bool UpdateToDatabase(TQuery* query);
    void LogRecord( AnsiString Text );
    bool isBeforeVersion( int majorVersion, int minorVersion );

private:
    void FormatSQL( TQuery* query );

    // persistent parameters
    int ProjectID;
    AnsiString theName;
    AnsiString FullName;
    AnsiString ValidTo;
    AnsiString ValidFrom;
    AnsiString DatabaseName;
    AnsiString URL;
    AnsiString StatusName;
    int Status;
    int DbMajorVersion;
    int DbMinorVersion;
    short NoteExists;

    static const int PROJECTTEST;
    static const int PROJECTLIVE;
    static const int PROJECTDEAD;
};

#endif


#ifndef PROFILECLASS
#define PROFILECLASS

class ProfileTestClass;
class ProfileBoxTypeClass;
class ProjectClass;

class ProfileClass : public TObject{

public:
    ProfileClass();
    ProfileClass(AnsiString aName);
    virtual __fastcall ~ProfileClass();

    AnsiString GetName(){return theName;}
    void SetName(AnsiString name){theName = name;}

    //set get ProfileID
    int GetProfileID(){ return ProfileID;}
    void SetProfileID(int id){ ProfileID = id; }

    // set get Description
    AnsiString GetDescription(){ return Description; }
    void SetDescription(AnsiString desc){ Description = desc; }

    // set get DateLastModified
    AnsiString GetDateLastModified() { return DateLastModified.DateString(); }
    void SetDateLastModified(TDateTime dlm) {
        DateLastModified = dlm; }

    void ClearProfileFlag() { ProfileFlag = 0; }
    void SetProfileFlagBit( int bit ) { ProfileFlag = ProfileFlag | bit; }
    bool isStoreSample() { return ProfileFlag & SAMPLE_BIT; }
    bool isStoreWholeTube() { return ( ProfileFlag & WHOLE_TUBE_BIT ); }

    void SetProfileFlag( int flag ) { ProfileFlag = flag; }
    int GetProfileFlag() { return ProfileFlag; }

    // set get NoteExists
    short GetNoteExists() { return NoteExists; }
    void SetNoteExists(short ne) { NoteExists = ne; }

    void LoadFromDatabase(TQuery* query, int recno);
    void InsertToDatabase(TQuery* query);
    void UpdateToDatabase(TQuery* query);
    void DeleteFromDatabase(TQuery* query);
    void LogRecord( AnsiString Text );

    void LoadTestsFromDatabase(TQuery* query);
    void DeleteTests();
    ProfileTestClass* GetTest( int index );
    ProfileTestClass* GetTest( AnsiString TestName );
    ProfileTestClass* GetTestByID( int id  );
    int GetNumTests() { return numTests; }

    void LoadBoxTypesFromDatabase(TQuery* query);
    void DeleteBoxTypes();
    ProfileBoxTypeClass* GetBoxType( int index );
    ProfileBoxTypeClass* GetBoxType( AnsiString BoxTypeName );
    int GetNumBoxTypes() { return numBoxTypes; }

    void SetProject( ProjectClass* proj ) { theProject = proj; }

    static const int SAMPLE_BIT;
    static const int WHOLE_TUBE_BIT;

private:

    AnsiString theName;

    int ProfileID;
    AnsiString Description;
    TDateTime DateLastModified;
    short NoteExists;
    short DeleteStatus;
    int ProfileFlag;

    int numTests;
    ProfileTestClass** theTests;

    int numBoxTypes;
    ProfileBoxTypeClass** theBoxTypes;

    ProjectClass* theProject;

    void FormatSQL( TQuery* query );
};

#endif

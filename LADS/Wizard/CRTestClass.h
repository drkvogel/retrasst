//---------------------------------------------------------------------------
#ifndef CRTestClassH
#define CRTestClassH
//---------------------------------------------------------------------------
class CRTestClass{

public:
    CRTestClass();
    CRTestClass::CRTestClass( AnsiString aName );
    CRTestClass::CRTestClass(CRTestClass& aCRTestClass);
    virtual __fastcall ~CRTestClass();

    AnsiString GetName() { return theName; }

    int GetCRID() { return CRID; }
    void SetCRID(int crID) { CRID = crID;}

    int GetTestID() { return TestID; }
    void SetTestID(int tID) { TestID = tID;}

    void LoadFromDatabase(TQuery* query, int recno);
    void InsertToDatabase(TQuery* query);
    void UpdateToDatabase(TQuery* query);
    void DeleteFromDatabase(TQuery* query);
    void LogRecord( AnsiString Text );

private:
    void FormatSQL( TQuery* query );

    // persistent properties
    AnsiString theName;
    int CRID;
    int TestID;
};

#endif

#ifndef MACHINECLASS
#define MACHINECLASS
class TestClass;

class MachineClass : public TObject{

public:
    MachineClass();
    MachineClass(AnsiString aName);
    virtual __fastcall ~MachineClass();

    //set get MachineID
    int GetMachineID(){ return MachineID;}
    void SetMachineID(int mid){ MachineID = mid; }

    //set get MachineName
    AnsiString GetMachineName(){ return MachineName;}
    void SetMachineName(AnsiString name){ MachineName = name; }

    // set get Description
    AnsiString GetDescription(){ return Description; }
    void SetDescription(AnsiString desc){ Description = desc; }

    // set get ValidFrom
    AnsiString GetValidFrom() { return DateToStr(ValidFrom); }
    void SetValidFrom(AnsiString vf) { ValidFrom = StrToDate(vf); }

    // set get ValidTo
    AnsiString GetValidTo() { return DateToStr(ValidTo); }
    void SetValidTo(AnsiString vt) { ValidTo = StrToDate(vt); }

    //set get ClusterID
    int GetClusterID(){ return ClusterID;}
    void SetClusterID(int cid){ ClusterID = cid; }

    //set get ClusterName
    AnsiString GetClusterName(){ return ClusterName;}
    void SetClusterName(AnsiString name){ ClusterName = name; }

    // set get Status
    short GetStatus() { return Status; }
    void SetStatus(short st) { Status = st; }

    // set get Location
    AnsiString GetLocation() { return Location; }
    void SetLocation(AnsiString loc) { Location = loc; }

    // set get NoteExists
    short GetNoteExists() { return NoteExists; }
    void SetNoteExists(short ne) { NoteExists = ne; }

    void LoadFromDatabase(TQuery* query, int recno);
    void InsertToDatabase(TQuery* query);
    void UpdateToDatabase(TQuery* query);
    void LogRecord( AnsiString Text );

    // needed for PlotQC
    int GetNumTests() { return numTests; }
    TestClass* GetTests( int index ) { return theTests[index]; }
    void LoadTestsFromDatabase(TQuery* query);
    TestClass* SearchForTest( AnsiString TestName );
    void DeleteTestClass();

private:
    void FormatSQL( TQuery* query );

    // persistent parameters
    int MachineID;
    AnsiString MachineName;
    AnsiString Description;
    TDate ValidFrom;
    TDate ValidTo;
    AnsiString ClusterName;
    int ClusterID;
    short Status;
    AnsiString Location;
    short NoteExists;

    // needed for PlotQC
    int numTests;
    TestClass** theTests;
};
#endif

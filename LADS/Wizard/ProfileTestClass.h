#ifndef PROFILETESTCLASS
#define PROFILETESTCLASS
class ProjectClass;

class ProfileTestClass{

public:
    ProfileTestClass();
    ProfileTestClass(AnsiString aName);
    ProfileTestClass(ProfileTestClass& aProfileTestClass);
    virtual __fastcall ~ProfileTestClass();

    AnsiString GetName(){return theName;}
    void SetName(AnsiString name) { theName = name; }

    int GetTestID() { return TestID; }
    void SetTestID(int tID) { TestID = tID;}

    AnsiString GetMachineName(){return MachineName;}
    void SetMachineName(AnsiString mname) { MachineName = mname; }

    int GetMachineID() { return MachineID; }
    void SetMachineID(int tID) { MachineID = tID;}

    int GetProfileID() { return ProfileID; }
    void SetProfileID(int tpID) { ProfileID = tpID;}

    AnsiString GetUpperLimit() { return UpperLimit; }
    void SetUpperLimit(AnsiString ul) { UpperLimit = ul; }

    AnsiString GetLowerLimit() { return LowerLimit; }
    void SetLowerLimit(AnsiString ll) { LowerLimit = ll; }

//    AnsiString GetNoLimitFlag() { return NoLimitFlag; }
//    void SetNoLimitFlag(AnsiString nl) { NoLimitFlag = nl; }
    void ClearTestFlag() { TestFlag = 0; }
    void SetTestFlagBit( int bit ) { TestFlag = TestFlag | bit; }
    void SetTestFlag( int flag ) { TestFlag = flag; }
    int GetTestFlag() { return TestFlag; }
    bool ignoreUpperLimit() { return TestFlag & IGNORE_UPPER_LIMIT_BIT; }
    bool ignoreLowerLimit() { return TestFlag & IGNORE_LOWER_LIMIT_BIT; }
    bool isPrivate() { return TestFlag & PRIVATE_BIT; }

    int GetTriggerID() { return TriggerID; }
    void SetTriggerID(int ttf) { TriggerID = ttf; }

    AnsiString GetTriggerName(){return TriggerName;}
    void SetTriggerName(AnsiString tname) { TriggerName = tname; }

    AnsiString GetTriggerUpperLimit() { return TriggerUpperLimit; }
    void SetTriggerUpperLimit(AnsiString tul) { TriggerUpperLimit = tul; }

    AnsiString GetTriggerLowerLimit() { return TriggerLowerLimit; }
    void SetTriggerLowerLimit(AnsiString tll) { TriggerLowerLimit = tll; }

    void InsertToDatabase(TQuery* query);
    void UpdateToDatabase(TQuery* query);
    void FormatSQL( TQuery* query );
    void DeleteFromDatabase(TQuery* query);
    void LogRecord( AnsiString Text );

    void SetProject( ProjectClass* proj ) { theProject = proj; }

    static const int PRIVATE_BIT;
    static const int IGNORE_UPPER_LIMIT_BIT;
    static const int IGNORE_LOWER_LIMIT_BIT;

private:

    // persistent properties
    AnsiString theName;
    int TestID;
    int MachineID;
    AnsiString MachineName;
    int ProfileID;
    AnsiString UpperLimit;
    AnsiString LowerLimit;
    int TestFlag;
    int TriggerID;
    AnsiString TriggerName;
    AnsiString TriggerUpperLimit;
    AnsiString TriggerLowerLimit;

    ProjectClass* theProject;
};

#endif

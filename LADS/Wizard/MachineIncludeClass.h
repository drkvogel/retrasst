#ifndef MACHINEINCLUDECLASS
#define MACHINEINCLUDECLASS

class Dbtables::TQuery;

class MachineIncludeClass : public TObject{

public:
    MachineIncludeClass();
    MachineIncludeClass(AnsiString aName);
    virtual __fastcall ~MachineIncludeClass();

    //set get MachineID
    int GetMachineID(){ return MachineID;}
    void SetMachineID(int mid){ MachineID = mid; }

    //set get MachineName
    AnsiString GetMachineName(){ return MachineName;}
    void SetMachineName(AnsiString name){ MachineName = name; }

    //set get Include
    bool GetInclude(){ return Include;}
    void SetInclude(){ Include = true; }
    void ResetInclude(){ Include = false; }

private:
    // persistent parameters
    int MachineID;
    AnsiString MachineName;
    bool Include;
};

#endif

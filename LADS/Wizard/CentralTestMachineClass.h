#ifndef CENTRALTESTMACHINECLASS
#define CENTRALTESTMACHINECLASS

class MachineClass;

class CentralTestMachineClass : public TObject{

public:
    CentralTestMachineClass();
    virtual __fastcall ~CentralTestMachineClass();

    //set get TestID
    int GetTestID(){ return TestID;}
    void SetTestID(int tid){ TestID = tid; }

    //set get MachineID
    int GetMachineID(){ return MachineID;}
    void SetMachineID(int mid){ MachineID = mid; }

    //set get Protocol
    short GetProtocol(){ return Protocol;}
    void SetProtocol(short pro){ Protocol = pro; }

    //set get TestOrder
    short GetTestOrder(){ return TestOrder;}
    void SetTestOrder(short to){ TestOrder = to; }

    // get MachineName
    AnsiString GetMachineName(){ return MachineName; }
    void SetMachineName(AnsiString mn) { MachineName = mn; }

    // get MachineTestName
    AnsiString GetMachineTestName(){ return MachineTestName; }
    void SetMachineTestName(AnsiString tn) { MachineTestName = tn; }

    void LoadFromDatabase(TQuery* query, int recno);

    void InsertToDatabase(TQuery* query);
    void UpdateToDatabase(TQuery* query);
    void DeleteFromDatabase(TQuery* query);
    void LogRecord( AnsiString Text );

private:
    void FormatSQL( TQuery* query );

    // persistent parameters
    int TestID;
    int MachineID;
    AnsiString MachineTestName;
    short Protocol;
    short TestOrder;

    AnsiString MachineName;

//  bool LookupMachineName( TQuery* machineQuery );
};

#endif

//---------------------------------------------------------------------------
#ifndef QCTestClassH
#define QCTestClassH

#include <DBTables.hpp>

//---------------------------------------------------------------------------
class QCTestClass{

public:
    QCTestClass();
    QCTestClass(AnsiString aName);
    QCTestClass(QCTestClass& aQCTestClass);
    virtual __fastcall ~QCTestClass();

    AnsiString GetName(){return theName;}
    void SetName(AnsiString name) { theName = name; }

    int GetQCID() { return QCID; }
    void SetQCID(int qcID) { QCID = qcID;}

    int GetQCTestCID() { return QCTestCID; }
    void SetQCTestCID(int tCID) { QCTestCID = tCID;}

    AnsiString GetPacketMean() { return PacketMean; }
    void SetPacketMean(AnsiString pm) { PacketMean = pm; }

    AnsiString GetPacketStdDev() { return PacketStdDev; }
    void SetPacketStdDev(AnsiString pstd) { PacketStdDev = pstd; }

    void InsertToDatabase(TQuery* query);
    void UpdateToDatabase(TQuery* query);
    void DeleteFromDatabase(TQuery* query);
    void LogRecord( AnsiString Text );

private:
    void FormatSQL( TQuery* query );

    // persistent properties
       AnsiString theName;
       int QCID;
       int QCTestCID;
       AnsiString PacketMean;
       AnsiString PacketStdDev;
};

#endif
 
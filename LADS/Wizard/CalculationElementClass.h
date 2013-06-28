//---------------------------------------------------------------------------

#ifndef CalculationElementClassH
#define CalculationElementClassH
//---------------------------------------------------------------------------
class CalculationElementClass : public TObject{

public:
    CalculationElementClass();
    virtual __fastcall ~CalculationElementClass();

    //set get ResultTestID
    int GetResultTestID(){ return ResultTestID;}
    void SetResultTestID(int tid){ ResultTestID = tid; }

    //set get Calculation
    short GetCalculation(){ return Calculation;}
    void SetCalculation(short calc){ Calculation = calc; }

    //set get TestPosition
    short GetTestPosition(){ return TestPosition;}
    void SetTestPosition(short pos){ TestPosition = pos; }

    //set get SourceTestID
    int GetSourceTestID(){ return SourceTestID;}
    void SetSourceTestID(int tid){ SourceTestID = tid; }

    void LoadFromDatabase(TQuery* query, int recno);

    void InsertToDatabase(TQuery* query);
    void UpdateToDatabase(TQuery* query);
    void DeleteFromDatabase(TQuery* query);
    void LogRecord( AnsiString Text );

private:
    void FormatSQL( TQuery* query );

    // persistent parameters
    int ResultTestID;
    int SourceTestID;
    short Calculation;
    short TestPosition;
};


#endif

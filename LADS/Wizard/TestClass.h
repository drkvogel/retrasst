#ifndef TESTCLASS
#define TESTCLASS
class CalculationElementClass;

class TestClass : public TObject{

public:
    TestClass();
    TestClass(AnsiString aName);
    virtual __fastcall ~TestClass();

    static const T_NUM_DATATYPES;
    static const short T_DATATYPENUM[];
    static const AnsiString T_DATATYPENAME[];

    AnsiString GetName(){return theName;}
    void SetName(AnsiString name){theName = name;}

    //set get TestID
    int GetTestID(){ return TestID;}
    void SetTestID(int id){ TestID = id; }

    // set get Description
    AnsiString GetDescription(){ return Description; }
    void SetDescription(AnsiString desc){ Description = desc; }

    // set get ValidFrom
    AnsiString GetValidFrom() { return DateToStr(ValidFrom); }
    void SetValidFrom(AnsiString vf) { ValidFrom = StrToDate(vf); }

    // set get ValidTo
    AnsiString GetValidTo() { return DateToStr(ValidTo); }
    void SetValidTo(AnsiString vt) { ValidTo = StrToDate(vt); }

    // set get Status
    short GetStatus() { return Status; }
    void SetStatus(short st) { Status = st; }

    // set get DataType
    int GetDataTypeIndex() { return DataTypeIndex; }
    short GetDataType() { return T_DATATYPENUM[DataTypeIndex]; }
    void SetDataTypeIndexByType(short dt); // also sets table name
    void SetDataTypeIndexByIndex(int di); // also sets table name
    AnsiString GetDataTypeName() { return T_DATATYPENAME[DataTypeIndex]; }

    // set get MinDataPoints
    int GetMinDataPoints() { return MinDataPoints; }
    void SetMinDataPoints(int mindp) { MinDataPoints = mindp; }

    // set get MaxDataPoints
    int GetMaxDataPoints() { return MaxDataPoints; }
    void SetMaxDataPoints(int maxdp) { MaxDataPoints = maxdp; }

    // set get DefaultLowerLimit
    AnsiString GetDefaultLowerLimit() { return DefaultLowerLimit; }
    void SetDefaultLowerLimit(AnsiString llim) { DefaultLowerLimit = llim; }

    // set get DefaultUpperLimit
    AnsiString GetDefaultUpperLimit() { return DefaultUpperLimit; }
    void SetDefaultUpperLimit(AnsiString ulim) { DefaultUpperLimit = ulim; }

    // set get SampleType
    AnsiString GetSampleType() { return SampleType; }
    void SetSampleType(AnsiString st) { SampleType = st; }

    // get numCalculations
    void SetNumCalcElements( int num ) { numCalcElements = num; }
    bool HasCalcElements();

    void LoadFromDatabase(TQuery* query, TQuery* query1, int recno);
    void InsertToDatabase(TQuery* query);
    void UpdateToDatabase(TQuery* query);
    void LogRecord( AnsiString Text );

private:
    void FormatSQL( TQuery* query );

    AnsiString theName;

    // persistent parameters
    int TestID;
    AnsiString Description;
    TDate ValidFrom;
    TDate ValidTo;
    short Status;
    short DataTypeIndex;
    int MinDataPoints;
    int MaxDataPoints;
    AnsiString DefaultLowerLimit;
    AnsiString DefaultUpperLimit;
    AnsiString SampleType;

    int numCalcElements;
};

#endif

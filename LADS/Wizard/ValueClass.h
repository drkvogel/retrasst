#ifndef VALUECLASS
#define VALUECLASS

class ValueClass{

public:
    ValueClass();
    ValueClass(AnsiString aName);
    ValueClass(ValueClass& aValueClass);
    virtual __fastcall ~ValueClass();

    AnsiString GetName(){return theName;}
    void SetName(AnsiString name) { theName = name; }

    int GetValueID() { return ValueID; }
    void SetValueID(int vID) { ValueID = vID;}

    int GetDescriptorID() { return DescriptorID; }
    void SetDescriptorID(int dID) { DescriptorID = dID;}

    AnsiString GetMinValue() { return MinValue; }
    void SetMinValue(AnsiString minV) { MinValue = minV; }

    AnsiString GetMaxValue() { return MaxValue; }
    void SetMaxValue(AnsiString maxV) { MaxValue = maxV; }

    void InsertToDatabase(TQuery* query);
    void UpdateToDatabase(TQuery* query);
    void DeleteFromDatabase(TQuery* query);
    void LogRecord( AnsiString Text );

private:

    // persistent properties
    AnsiString theName;
    int ValueID;
    int DescriptorID;
    AnsiString MinValue;
    AnsiString MaxValue;

    void FormatSQL( TQuery* query );
};

#endif

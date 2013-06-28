#ifndef DESCRIPTORCLASS
#define DESCRIPTORCLASS

class ValueClass;

class DescriptorClass : public TObject{

public:
    DescriptorClass();
    DescriptorClass(AnsiString aName);
    virtual __fastcall ~DescriptorClass();

    AnsiString GetName(){return theName;}
    void SetName(AnsiString name){theName = name;}

    //set get DescriptorID
    int GetDescriptorID(){ return DescriptorID;}
    void SetDescriptorID(int id){ DescriptorID = id; }

    //set get Descriptor_source
    AnsiString GetDescriptorSource(){ return DescriptorSource; }
    void SetDescriptorSource(AnsiString ds){ DescriptorSource = ds; }

    // set get Description
    AnsiString GetDescription(){ return Description; }
    void SetDescription(AnsiString desc){ Description = desc; }

    // set get MaximumLength
    short GetMaximumLength() { return MaximumLength; }
    void SetMaximumLength(short maxl) { MaximumLength = maxl; }

    // set get MinimumLength
    short GetMinimumLength() { return MinimumLength; }
    void SetMinimumLength(short minl) { MinimumLength = minl; }

    // set get SpecimenFieldName
    AnsiString GetSpecimenFieldName() { return SpecimenFieldName; }
    void SetSpecimenFieldName(AnsiString field) { SpecimenFieldName = field; }
    int GetSpecimenNum();
    // get NextSpecimenNum
    static int GetNextSpecimenNum() { return NextSpecimenNum++; }

    // set get MapFieldName
    AnsiString GetMapFieldName() { return MapFieldName; }
    void SetMapFieldName(AnsiString field) { MapFieldName = field; }
    int GetMapNum()
        { return StrToInt(MapFieldName.SubString(9,MapFieldName.Length())); }
    // get NextMapNum
    static int GetNextMapNum() { return NextMapNum++; }

    // set get ParseOrder
    short GetParseOrder() { return ParseOrder; }
    void SetParseOrder(short ord) { ParseOrder = ord; }

    // set get DataType
    short GetDataTypeInt() { return DataType; }
    AnsiString GetDataTypeString() { return strDataType[DataType]; }
    void SetDataType(short type) { DataType = type; }

    // Flag
    void ClearFlag() { Flag = 0; }
    void SetFlagBit( int bit ) { Flag = Flag | bit; }
    int GetFlag() { return Flag; }
    bool isTubeID() { return Flag & TUBEID_BIT; }

    bool CheckDuplicateName(TQuery* query);
    bool CheckDuplicateParseOrder(TQuery* query);

    void DeleteValues();
    int GetNumValues() {return numValues;}
    void ResetNumValues() {numValues = 0;}
    ValueClass* GetValue( int index );
    ValueClass* GetValue( AnsiString ValueName );
    ValueClass* GetValueByID( int id  );
    ValueClass* GetValueByData( AnsiString data );
    ValueClass GetNext();

    void LoadFromDatabase(TQuery* query, int recno);
    void LoadValuesFromDatabase(TQuery* query);
    void LoadProfilesFromDatabase(TQuery* query);

    //    void SaveAll(TDatabase* db);
    void InsertToDatabase(TQuery* query);
    void UpdateToDatabase(TQuery* query);
    void DeleteFromDatabase(TQuery* query);
    void LogRecord( AnsiString Text );

    // static parameters
    static const int intString;
    static const int intBoolean;
    static const int intDate;
    static const int intInteger;
    static const int intReal;

    static const int TUBEID_BIT;

private:
    AnsiString theName;

    // persistent parameters
    int DescriptorID;
    AnsiString DescriptorSource;
    AnsiString Description;
    short MaximumLength;
    short MinimumLength;
    AnsiString SpecimenFieldName;
    AnsiString MapFieldName;
    short DataType;
    short ParseOrder;
    int Flag;

    // static parameters
    static int NextSpecimenNum;
    static int NextMapNum;
    static const AnsiString strDataType[5];

    // runtime parameters
    int numValues;
    ValueClass** theValues;

    void FormatSQL( TQuery* query );

};
#endif

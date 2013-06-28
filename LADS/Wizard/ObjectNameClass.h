//---------------------------------------------------------------------------
#ifndef ObjectNameClassH
#define ObjectNameClassH
//---------------------------------------------------------------------------
class ObjectNameClass : public TObject{

public:
    ObjectNameClass();
    ObjectNameClass(AnsiString aName);
    virtual __fastcall ~ObjectNameClass();

    AnsiString GetName(){return theName;}
    void SetName(AnsiString name){theName = name;}

    AnsiString GetFullName(){return FullName;}
    void SetFullName(AnsiString name){FullName = name;}

    int GetObjectID(){ return ObjectID;}
    void SetObjectID(int id){ ObjectID = id; }

    int GetObjectType(){ return ObjectType;}
    void SetObjectType( int type ){ ObjectType = type;}

    void LoadFromDatabase(TQuery* query, int recno);
    void InsertToDatabase(TQuery* query);
    void UpdateToDatabase(TQuery* query);
    void LogRecord( AnsiString Text );

    static const int OBJ_TYPE_CALIBRANT;
    static const int OBJ_TYPE_REAGENT;
    static const int OBJ_TYPE_ANALYZER_EVENT;
    static const int OBJ_TYPE_SAMPLE_INVENTORY_SYSTEM_EVENT;
    static const int OBJ_TYPE_MACHINE_CLUSTER;
    static const int OBJ_TYPE_ALIQUOT_TYPE;
    static const int OBJ_TYPE_WESTGARD_QC_RULE;
    static const int OBJ_TYPE_TANK_LOCATION;
    static const int OBJ_TYPE_HIVE_OF_STORAGE_BOXES;
    static const int OBJ_NUM_OBJECTTYPES;

private:
    void FormatSQL( TQuery* query );

    AnsiString theName;

    // persistent parameters
    int ObjectID;
    AnsiString FullName;
    int ObjectType;
    int Status;
    int ProcessID;
};

#endif

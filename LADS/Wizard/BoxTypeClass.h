//---------------------------------------------------------------------------

#ifndef BoxTypeClassH
#define BoxTypeClassH
class AliquotTypeClass;
//---------------------------------------------------------------------------
class BoxTypeClass : public TObject{

public:
    BoxTypeClass();
    BoxTypeClass(AnsiString aName);
    virtual __fastcall ~BoxTypeClass();

    //set get BoxID
    int GetBoxTypeID(){ return BoxTypeID;}
    void SetBoxTypeID(int bid){ BoxTypeID = bid; }

    //set get BoxName
    AnsiString GetBoxName(){ return BoxName;}
    void SetBoxName(AnsiString name){ BoxName = name; }

    // set get Status
    short GetStatus() { return Status; }
    void SetStatus(short st) { Status = st; }

    //set get BoxCapacity
    short GetBoxCapacity(){ return BoxCapacity;}
    void SetBoxCapacity(short cap){ BoxCapacity = cap; }

    //set get ProjectID
    int GetProjectID(){ return ProjectID;}
    void SetProjectID(int pid){ ProjectID = pid; }

    void LoadFromDatabase(TQuery* query, int recno);
    void InsertToDatabase(TQuery* query);
    void UpdateToDatabase(TQuery* query);
    void DeleteFromDatabase(TQuery* query);
    void LogRecord( AnsiString Text );

    void LoadAliquotTypesFromDatabase(TQuery* query);
    AliquotTypeClass* GetAliquotType( int index );
    AliquotTypeClass* GetAliquotType( AnsiString AliquotTypeName );
    void DeleteAliquotTypes();
    int GetNumAliquots() { return numAliquotTypes; }

private:
    void FormatSQL( TQuery* query );

    // persistent parameters
    int BoxTypeID;
    AnsiString BoxName;
    short Status;
    short BoxCapacity;
    int ProjectID;

    AliquotTypeClass** theAliquotTypes;
    int numAliquotTypes;

};

#endif
 
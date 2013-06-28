//---------------------------------------------------------------------------

#ifndef AllAliquotClassH
#define AllAliquotClassH
//---------------------------------------------------------------------------
class AllAliquotClass : public TObject{

public:
    AllAliquotClass();
    AllAliquotClass(AnsiString aName);
    virtual __fastcall ~AllAliquotClass();

    //set get AliquotID
    int GetAliquotTypeID(){ return AliquotTypeID;}
    void SetAliquotTypeID(int aid){ AliquotTypeID = aid; }

    //set get AliquotName
    AnsiString GetAliquotName(){ return AliquotName;}
    void SetAliquotName(AnsiString name){ AliquotName = name; }

    // set get Status
    short GetStatus() { return Status; }
    void SetStatus(short st) { Status = st; }

    void LoadFromDatabase(TQuery* query, int recno);

private:

    // persistent parameters
    int AliquotTypeID;
    AnsiString AliquotName;
    short Status;
};

#endif

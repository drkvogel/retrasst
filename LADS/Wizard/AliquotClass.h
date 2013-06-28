//---------------------------------------------------------------------------

#ifndef AliquotClassH
#define AliquotClassH
//---------------------------------------------------------------------------
class AliquotClass : public TObject{

public:
    AliquotClass();
    AliquotClass(AnsiString aName);
    AliquotClass(AliquotClass& aAliquotClass);
    virtual __fastcall ~AliquotClass();

    AnsiString GetName(){return theName;}
    void SetName(AnsiString name) { theName = name; }

    int GetBoxTypeID() { return BoxTypeID; }
    void SetBoxTypeID(int tID) { BoxTypeID = tID;}

    void LoadFromDatabase(TQuery* query, int recno);

private:

    // persistent properties
    AnsiString theName;
    int BoxTypeID;
};
#endif

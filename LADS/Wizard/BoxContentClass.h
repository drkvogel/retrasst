//---------------------------------------------------------------------------

#ifndef BoxContentClassH
#define BoxContentClassH
//---------------------------------------------------------------------------
class BoxContentClass{

public:
    BoxContentClass();
    BoxContentClass(AnsiString aName);
    BoxContentClass(BoxContentClass& aBoxContentClass);
    virtual __fastcall ~BoxContentClass();

    AnsiString GetName(){return theName;}
    void SetName(AnsiString name) { theName = name; }

    int GetBoxTypeID() { return BoxTypeID; }
    void SetBoxTypeID(int bid) { BoxTypeID = bid;}

    AnsiString GetDescription(){return Description;}
    void SetDescription(AnsiString desc) { Description = desc; }

    short GetStatus() { return Status; }
    void SetStatus(short st) { Status = st; }

    int GetBoxSizeID() { return BoxSizeID; }
    void SetBoxSizeID(int id) { BoxSizeID = id; }

    int GetAliquotTypeID1() { return AliquotTypeID1; }
    void SetAliquotTypeID1(int tID) { AliquotTypeID1 = tID;}
    int GetAliquotTypeID2() { return AliquotTypeID2; }
    void SetAliquotTypeID2(int tID) { AliquotTypeID2 = tID;}
    int GetAliquotTypeID3() { return AliquotTypeID3; }
    void SetAliquotTypeID3(int tID) { AliquotTypeID3 = tID;}

    void LoadFromDatabase(TQuery* query, int recno);
    void InsertToDatabase(TQuery* query);
    void UpdateToDatabase(TQuery* query);
    void FormatSQL( TQuery* query );
    void DeleteFromDatabase(TQuery* query);
    void LogRecord( AnsiString Text );

private:

    // persistent properties
    AnsiString theName;
    AnsiString Description;
    int AliquotTypeID1;
    int AliquotTypeID2;
    int AliquotTypeID3;
    short Status;
    int BoxTypeID;
    int BoxSizeID;
};

#endif


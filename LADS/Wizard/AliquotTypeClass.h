//---------------------------------------------------------------------------

#ifndef AliquotTypeClassH
#define AliquotTypeClassH
//---------------------------------------------------------------------------
class AliquotTypeClass{

public:
    AliquotTypeClass();
    AliquotTypeClass(AnsiString aName);
    AliquotTypeClass(AliquotTypeClass& aAliquotTypeClass);
    virtual __fastcall ~AliquotTypeClass();

    AnsiString GetName(){return theName;}
    void SetName(AnsiString name) { theName = name; }

    int GetAliquotTypeID() { return AliquotTypeID; }
    void SetAliquotTypeID(int tID) { AliquotTypeID = tID;}

    short GetAliquotOrder() { return AliquotOrder; }
    void SetAliquotOrder(short ord) { AliquotOrder = ord;}

    int GetBoxTypeID() { return BoxTypeID; }
    void SetBoxTypeID(int bid) { BoxTypeID = bid;}

    void InsertToDatabase(TQuery* query);
    void UpdateToDatabase(TQuery* query);
    void FormatSQL( TQuery* query );
    void DeleteFromDatabase(TQuery* query);
    void LogRecord( AnsiString Text );

private:

    // persistent properties
    AnsiString theName;
    int AliquotTypeID;
    short AliquotOrder;
    int BoxTypeID;
};

#endif
 
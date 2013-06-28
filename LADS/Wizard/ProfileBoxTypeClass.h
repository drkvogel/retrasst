//---------------------------------------------------------------------------

#ifndef ProfileBoxTypeClassH
#define ProfileBoxTypeClassH
//---------------------------------------------------------------------------
class ProfileBoxTypeClass{

public:
    ProfileBoxTypeClass();
    ProfileBoxTypeClass(AnsiString aName);
    ProfileBoxTypeClass(ProfileBoxTypeClass& aProfileBoxTypeClass);
    virtual __fastcall ~ProfileBoxTypeClass();

    AnsiString GetName(){return theName;}
    void SetName(AnsiString name) { theName = name; }

    int GetBoxTypeID() { return BoxTypeID; }
    void SetBoxTypeID(int tID) { BoxTypeID = tID;}

    short GetBoxOrder() { return BoxOrder; }
    void SetBoxOrder(short ord) { BoxOrder = ord;}

    int GetProfileID() { return ProfileID; }
    void SetProfileID(int pID) { ProfileID = pID;}

    void InsertToDatabase(TQuery* query);
    void UpdateToDatabase(TQuery* query);
    void FormatSQL( TQuery* query );
    void DeleteFromDatabase(TQuery* query);
    void LogRecord( AnsiString Text );

private:

    // persistent properties
    AnsiString theName;
    int BoxTypeID;
    short BoxOrder;
    int ProfileID;
};

#endif
 
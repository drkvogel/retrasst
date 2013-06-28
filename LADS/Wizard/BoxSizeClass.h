//---------------------------------------------------------------------------

#ifndef BoxSizeClassH
#define BoxSizeClassH
class AliquotTypeClass;
//---------------------------------------------------------------------------
class BoxSizeClass : public TObject{

public:
    BoxSizeClass();
    virtual __fastcall ~BoxSizeClass();

    //set get BoxID
    int GetBoxSizeID(){ return BoxSizeID;}
    void SetBoxSizeID(int bid){ BoxSizeID = bid; }

    //set get BoxName
    AnsiString GetDescription(){ return Description;}
    void SetDescription(AnsiString desc){ Description = desc; }

    //set get BoxCapacity
    short GetCapacity(){ return BoxCapacity;}
    void SetCapacity(short cap){ BoxCapacity = cap; }

    // set get EmptyPosition
    short GetEmptyPosition() { return EmptyPosition; }
    void SetEmptyPosition(short pos) { EmptyPosition = pos; }

    void LoadFromDatabase(TQuery* query, int recno);
    void InsertToDatabase(TQuery* query);
    void UpdateToDatabase(TQuery* query);
    void DeleteFromDatabase(TQuery* query);
    void LogRecord( AnsiString Text );

private:
    void FormatSQL( TQuery* query );

    // persistent parameters
    int BoxSizeID;
    AnsiString Description;
    short BoxCapacity;
    short EmptyPosition;
};

#endif

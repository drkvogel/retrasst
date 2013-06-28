//---------------------------------------------------------------------------
#ifndef ProjectNoteClassH
#define ProjectNoteClassH
//---------------------------------------------------------------------------

class ProjectNoteClass : public TObject{

public:
    ProjectNoteClass();
    virtual __fastcall ~ProjectNoteClass();

    //set get NoteID
    int GetNoteID(){ return NoteID;}
    void SetNoteID(int nid){ NoteID = nid; }

    //set get ObjectID
    int GetObjectID(){ return ObjectID;}
    void SetObjectID(int obid){ ObjectID = obid; }

    // set get Contents
    AnsiString GetContents(){ return Contents; }
    void SetContents(AnsiString cont){ Contents = cont; }

    // get Timestamp
    TDateTime GetTimestamp() { return Timestamp; }

    void LoadFromDatabase(TQuery* query, int recno);
    void InsertToDatabase(TQuery* query);
    void UpdateToDatabase(TQuery* query);
    void DeleteFromDatabase(TQuery* query);
    void LogRecord( AnsiString Text );

private:
    int NoteID;
    int ObjectID;
    AnsiString Contents;
    TDateTime Timestamp;

    void FormatSQL( TQuery* query );
};

#endif

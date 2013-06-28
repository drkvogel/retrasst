#ifndef PROFILEMAPCLASS
#define PROFILEMAPCLASS

class ProfileMapClass : public TObject{

public:
// the name is an AnsiString representation of the profile_id
    ProfileMapClass();
    virtual __fastcall ~ProfileMapClass();

    //set get ProfileMapID
    int GetProfileMapID(){ return ProfileMapID;}
    void SetProfileMapID(int id){ ProfileMapID = id; }

    // set get ValueIDs
    int GetValueID( int index ){ return ValueID[index]; }
    void SetValueID( int index, int vid ){ValueID[index] = vid;}
/*
    void SetValueID(int vid, int index, TQuery query);
*/
    // set get ProfileID
    int GetProfileID() { return ProfileID; }
    void SetProfileID(int tpid) { ProfileID = tpid; }

    // get numValues
//    int GetNumValues(){ return numValues; }

    void LoadFromDatabase(TQuery* query, int recno);
    void InsertToDatabase(TQuery* query);
    void UpdateToDatabase(TQuery* query);
    void DeleteFromDatabase(TQuery* query);

    int maxValues;

private:
    AnsiString theName;
    int ProfileMapID;
    int ValueID[30];
    int ProfileID;

    AnsiString fieldNamePrefix;

    void FormatSQL( TQuery* query );
};

#endif

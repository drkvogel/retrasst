//---------------------------------------------------------------------------

#ifndef AllBoxTypes1H
#define AllBoxTypes1H
//---------------------------------------------------------------------------
class BoxTypeClass;
class ProjectClass;

class TAllBoxTypes : public TObject{

private:	// User declarations
    ProjectClass* theProject;
    BoxTypeClass** theBoxType;
    int numBoxTypes;
public:		// User declarations
    TAllBoxTypes();
    virtual __fastcall ~TAllBoxTypes();

    void SetProject( ProjectClass* proj ) { theProject = proj; }

    void CopyFromDatabaseToClass( TQuery* query );
    void DeleteBoxTypeClass();

    int GetNumBoxTypes() { return numBoxTypes; }
    BoxTypeClass** GetAllBoxTypes() { return theBoxType; }
    int GetBoxTypeIDFromName(AnsiString name);
};
#endif

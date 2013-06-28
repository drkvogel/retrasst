//---------------------------------------------------------------------------

#ifndef AllAliquotTypes1H
#define AllAliquotTypes1H
//---------------------------------------------------------------------------
class AllAliquotClass;

class TAllAliquotTypes : public TObject{

private:	// User declarations
    AllAliquotClass** theAllAliquot;
    int numAllAliquots;
public:		// User declarations
    TAllAliquotTypes();
    virtual __fastcall ~TAllAliquotTypes();
    void CopyFromDatabaseToClass( TQuery* query );
    void DeleteAllAliquotClass();

    AllAliquotClass** GetAllAliquots() { return theAllAliquot; }
    int GetNumAllAliquots() { return  numAllAliquots; }
    int GetAliquotTypeIDFromName(AnsiString name);
};
#endif

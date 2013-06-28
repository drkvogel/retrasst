//---------------------------------------------------------------------------

#ifndef AllAliquots1H
#define AllAliquots1H
//---------------------------------------------------------------------------
class AliquotClass;

class TAllAliquots : public TObject{

private:	// User declarations
public:		// User declarations
    TAllAliquots();
    virtual __fastcall ~TAllAliquots();
    void CopyFromDatabaseToClass( TQuery* query );
    void DeleteAliquotClass();

    AliquotClass** theAliquot;
    int numAliquots;
};
#endif

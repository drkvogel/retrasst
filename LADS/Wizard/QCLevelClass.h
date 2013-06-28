//---------------------------------------------------------------------------
#ifndef QCLevelClassH
#define QCLevelClassH

#include <DBTables.hpp>

//---------------------------------------------------------------------------

class QCLevelClass{

public:
    QCLevelClass();
    QCLevelClass(AnsiString aName);
    QCLevelClass(QCLevelClass& aLevelClass);
    virtual __fastcall ~QCLevelClass();

    //set get Name
    AnsiString GetName(){ return Name;}
    void SetName(AnsiString name){ Name = name; }

    // set get Description
    AnsiString GetDescription(){ return Description; }
    void SetDescription(AnsiString desc){ Description = desc; }

    //set get Material
    AnsiString GetMaterial(){ return Material;}
    void SetMaterial(AnsiString mat){ Material = mat; }

    void LoadFromDatabase(TQuery* query, int recno);
    void InsertToDatabase(TQuery* query);
    void UpdateToDatabase(TQuery* query);
    void LogRecord( AnsiString Text );

private:
    void FormatSQL( TQuery* query );

    // persistent parameters
    AnsiString Name;
    AnsiString Description;
    AnsiString Material;
};

#endif


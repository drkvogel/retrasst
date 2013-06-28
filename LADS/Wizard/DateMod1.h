//---------------------------------------------------------------------------

#ifndef DateMod1H
#define DateMod1H
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
//---------------------------------------------------------------------------
class TDateMod : public TDataModule
{
__published:	// IDE-managed Components

private:	// User declarations
    void PutDateQuery( TQuery* query, AnsiString param, AnsiString date );
    void PutDateTimeQuery( TQuery* query, AnsiString param, AnsiString date );

public:		// User declarations
    __fastcall TDateMod(TComponent* Owner);
    TDateTime GetToday();
    TDateTime GetDefaultDate();
    void PutDateToDatabase( TQuery* query, AnsiString param, AnsiString date );
    void PutTimeStampToDatabase( TQuery* query, AnsiString param );
    bool ValidateDate( AnsiString date );
};
//---------------------------------------------------------------------------
extern PACKAGE TDateMod *DateMod;
//---------------------------------------------------------------------------
#endif


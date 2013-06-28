//---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop

#include <Dbtables.hpp>
#include "DateMod1.h"
#include "DataMod1.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"
TDateMod *DateMod;
//---------------------------------------------------------------------------
__fastcall TDateMod::TDateMod(TComponent* Owner)
    : TDataModule(Owner)
{
}
//---------------------------------------------------------------------------
TDateTime TDateMod::GetToday()
{
// Get current date and time in TDateTime format.
    TDateTime Today;
    Today = Today.CurrentDateTime();
    return Today;
}

//---------------------------------------------------------------------------
TDateTime TDateMod::GetDefaultDate()
{
// Get default date and time in TDateTime format.
    TDateTime DefaultDate;
    DefaultDate = StrToDate("30/12/1899");
    return DefaultDate;
}
//---------------------------------------------------------------------------
bool TDateMod::ValidateDate( AnsiString date )
{
    bool validdate = true;
    try
    {
        StrToDate( date );
    }
    catch(Exception& e)
    {
        AnsiString strError = "Date error\n" + e.Message;
        Application->MessageBox(
            strError.c_str(),
            "Date Handling Error",MB_OK);
        validdate = false;
    }
    return validdate;
}

//---------------------------------------------------------------------------
void TDateMod::PutDateQuery( TQuery* query, AnsiString param, AnsiString date )
{
    query->ParamByName( param )->AsString = date;
}

//---------------------------------------------------------------------------
void TDateMod::PutDateTimeQuery( TQuery* query, AnsiString param, AnsiString date )
{
    query->ParamByName( param )->AsDateTime = date;
}

//---------------------------------------------------------------------------

void TDateMod::PutDateToDatabase( TQuery* query, AnsiString param, AnsiString date )
{
//  put date to database
    PutDateQuery( query, param, date );
}

//---------------------------------------------------------------------------

void TDateMod::PutTimeStampToDatabase( TQuery* query, AnsiString param )
{
    query->ParamByName( param )->AsString = "now";
}

//---------------------------------------------------------------------------



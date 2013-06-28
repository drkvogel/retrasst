//---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop

#include "AllAliquotClass.h"
#include "LogMod1.h"

//---------------------------------------------------------------------------

#pragma package(smart_init)
AllAliquotClass::AllAliquotClass()
: AliquotName("")
{
    AliquotTypeID = 0;
    Status = 0;
}

AllAliquotClass::AllAliquotClass(AnsiString aName)
: AliquotName(aName)
{
    AliquotTypeID = 0;
    Status = 0;
}


__fastcall AllAliquotClass::~AllAliquotClass()
{
}

void AllAliquotClass::LoadFromDatabase(TQuery* query, int recno)
{
    if(query -> RecordCount <= recno)
    {
        //error, index out of range
        return;
    }
    else
    {
        query -> First();
        for (int x = 0; x < recno; x++)
        {
            query -> Next();
        }

        try
        {
            AliquotTypeID = query -> FieldByName("object_cid") -> AsInteger;
            AliquotName = query -> FieldByName("external_name") -> AsString;
            Status = (short)query -> FieldByName("status") -> AsInteger;
        }
        catch(Exception& e)
        {
            AnsiString strError =
                "Error loading Aliquot from database.\n" + e.Message;
            Application->MessageBox(
                strError.c_str(),
                "AllAliquotClass Error",MB_OK);
            LogMod->LogCentralDb( TLogMod::LOGLOCAL, strError );
        }
    }
}



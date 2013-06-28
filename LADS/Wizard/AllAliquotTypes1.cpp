//---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop

#include "AllAliquotTypes1.h"
#include "LogMod1.h"
#include "AllAliquotClass.h"

//---------------------------------------------------------------------------

#pragma package(smart_init)
TAllAliquotTypes *AllAliquotTypes;
//---------------------------------------------------------------------------
TAllAliquotTypes::TAllAliquotTypes()
    : numAllAliquots(-1)
{
}
//---------------------------------------------------------------------------
__fastcall TAllAliquotTypes::~TAllAliquotTypes()
{
    DeleteAllAliquotClass();
}
//---------------------------------------------------------------------------

void TAllAliquotTypes::CopyFromDatabaseToClass( TQuery* query )
{
    DeleteAllAliquotClass();

    query -> SQL -> Clear();
    query -> SQL -> Add(
        "select * "
        "  from c_object_name "
        "  where object_type = 6 "
        "  and   status != 99 "
        "  order by external_name ");

    try
    {
        query -> Open();
    }
    catch(Exception& e)
    {
        if(e.Message == "Error creating cursor handle")
        {
            //do nothing
        }
        else
        {
            AnsiString strError =
                "Error reading aliquot type\n" + e.Message;
            Application->MessageBox(
                strError.c_str(),
                "AllAliquotTypes Error",MB_OK);
            LogMod->LogCentralDb( TLogMod::LOGLOCAL, strError );
        }
    }
    numAllAliquots = query->RecordCount;
    theAllAliquot = new AllAliquotClass*[numAllAliquots];
    for (int record=0; record < query->RecordCount; record++)
    {
        theAllAliquot[record] = new AllAliquotClass;
        theAllAliquot[record]->LoadFromDatabase(query,record);
    }
    LogMod->Commit( query );
    query -> Active = false;
}
//---------------------------------------------------------------------------
void TAllAliquotTypes::DeleteAllAliquotClass()
{
    if ( theAllAliquot != NULL )
    {
        for (int index=0; index < numAllAliquots; index++)
        {
            delete theAllAliquot[index];
        }
        delete[] theAllAliquot;
    }
    theAllAliquot = NULL;
    numAllAliquots = -1;
}
//---------------------------------------------------------------------------
// Find the index into the aliquot array for `name', return ID
// If not found, return 0
int TAllAliquotTypes::GetAliquotTypeIDFromName(AnsiString name)
{
    int id = 0;
    for (int index=0; index < numAllAliquots; index++)
    {
        if (name == theAllAliquot[index]->GetAliquotName())
        {
            id = theAllAliquot[index]->GetAliquotTypeID();
            break;
        }
    }
    return id;
}
//---------------------------------------------------------------------------
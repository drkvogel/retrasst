//---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop

#include "AllBoxTypes1.h"
#include "BoxTypeClass.h"
#include "LogMod1.h"
#include "ProjectClass.h"

//---------------------------------------------------------------------------

#pragma package(smart_init)
TAllBoxTypes *AllBoxTypes;
//---------------------------------------------------------------------------
TAllBoxTypes::TAllBoxTypes()
    : numBoxTypes(-1)
{
}
//---------------------------------------------------------------------------
__fastcall TAllBoxTypes::~TAllBoxTypes()
{
    DeleteBoxTypeClass();
}
//---------------------------------------------------------------------------

void TAllBoxTypes::CopyFromDatabaseToClass( TQuery* query )
{
    DeleteBoxTypeClass();

    query -> SQL -> Clear();
    query -> SQL -> Add(
        "select * "
        "  from c_box_type "
        "  where project_cid = :projID "
        "  and status != 99 "
        "  order by external_name ");
    query->ParamByName("projID")->AsInteger = theProject->GetProjectID();

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
                "Error reading box type\n" + e.Message;
            Application->MessageBox(
                strError.c_str(),
                "AllBoxTypes Error",MB_OK);
            LogMod->LogCentralDb( TLogMod::LOGLOCAL, strError );
        }
    }
    numBoxTypes = query->RecordCount;
    theBoxType = new BoxTypeClass*[numBoxTypes];
    for (int record=0; record < query->RecordCount; record++)
    {
        theBoxType[record] = new BoxTypeClass;
        theBoxType[record]->LoadFromDatabase(query,record);
    }
    LogMod->Commit( query );
    query -> Active = false;
}
//---------------------------------------------------------------------------
// Find the index into the box type array for `name', return ID
// If not found, return 0
int TAllBoxTypes::GetBoxTypeIDFromName(AnsiString name)
{
    int id = 0;
    for (int index=0; index < numBoxTypes; index++)
    {
        if (name == theBoxType[index]->GetBoxName())
        {
            id = theBoxType[index]->GetBoxTypeID();
            break;
        }
    }
    return id;
}
//---------------------------------------------------------------------------
void TAllBoxTypes::DeleteBoxTypeClass()
{
    // delete all box types
    for (int index=0; index < numBoxTypes; index++)
    {
        delete theBoxType[index];
    }
    delete[] theBoxType;
    theBoxType = NULL;
    numBoxTypes = -1;
}
//---------------------------------------------------------------------------

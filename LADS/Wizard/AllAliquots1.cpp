//---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop

#include "AllAliquots1.h"
#include "AliquotClass.h"
#include "Login1.h"
#include "LogMod1.h"

//---------------------------------------------------------------------------

#pragma package(smart_init)
TAllAliquots *AllAliquots;
//---------------------------------------------------------------------------
TAllAliquots::TAllAliquots()
    : numAliquots(-1)
{
}
//---------------------------------------------------------------------------
__fastcall TAllAliquots::~TAllAliquots()
{
    DeleteAliquotClass();
}
//---------------------------------------------------------------------------

void TAllAliquots::CopyFromDatabaseToClass( TQuery* query )
{
    // delete all tests, create anew from database
    DeleteAliquotClass();

    query -> SQL -> Clear();
    query -> SQL -> Add(
        "select external_name, box_type_cid "
        "  from c_box_type "
        "  where project_cid = :projID "
        "  and status != 99 "
        "  order by external_name ");
    query->ParamByName("projID")->AsInteger = Login->GetProjectID();

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
                "Error reading aliquot\n" + e.Message;
            Application->MessageBox(
                strError.c_str(),
                "AllAliquots Error",MB_OK);
            LogMod->LogProjectDb( LOGLOCAL, strError );
        }
    }
    numAliquots = query->RecordCount;
    theAliquot = new AliquotClass*[numAliquots];
    for (int record=0; record < query->RecordCount; record++)
    {
        theAliquot[record] = new AliquotClass;
        theAliquot[record]->LoadFromDatabase(query,record);
    }
    LogMod->Commit( query );
    query -> Active = false;
}
//---------------------------------------------------------------------------
void TAllAliquots::DeleteAliquotClass()
{
    // delete all aliquots
    for (int index=0; index < numAliquots; index++)
    {
        delete theAliquot[index];
    }
    delete[] theAliquot;
    theAliquot = NULL;
    numAliquots = -1;
}
//---------------------------------------------------------------------------

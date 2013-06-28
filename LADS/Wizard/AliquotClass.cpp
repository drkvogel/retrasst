//---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop

#include "AliquotClass.h"
#include "LogMod1.h"

//---------------------------------------------------------------------------

#pragma package(smart_init)
AliquotClass::AliquotClass()
:theName("default construction")
{
    BoxTypeID = 0;
}

AliquotClass::AliquotClass(AnsiString aName)
:theName(aName)
{
    BoxTypeID = 0;
}

AliquotClass::AliquotClass(AliquotClass& aTPAliquot)

{
    theName = aTPAliquot.theName;
    BoxTypeID = aTPAliquot.BoxTypeID;
}

// default destructor - will eventually implement clean up code
 __fastcall AliquotClass::~AliquotClass()
{
}

void AliquotClass::LoadFromDatabase(TQuery* query, int recno)
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
            BoxTypeID = query -> FieldByName("box_type_cid") -> AsInteger;
            theName = query -> FieldByName("external_name") -> AsString;
        }
        catch(Exception& e)
        {
            AnsiString strError =
                "Error loading Aliquot from database\n" + e.Message;
            Application->MessageBox(
                strError.c_str(),
                "AliquotClass Error",MB_OK);
            LogMod->
            LogProjectDb( LOGLOCAL, strError );
        }
    }
}


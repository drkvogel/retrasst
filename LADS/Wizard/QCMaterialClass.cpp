//---------------------------------------------------------------------------
#include <vcl.h>

#include "QCMaterialClass.h"
#include "QCLevelClass.h"

#pragma hdrstop

//---------------------------------------------------------------------------

#pragma package(smart_init)

// use QCMaterialClass to provide a named container for instances of QCLevelClass

QCMaterialClass::QCMaterialClass()
: Name(""), numQCLevels(0)
{
	theQCLevels = NULL;
}

QCMaterialClass::QCMaterialClass(AnsiString aName)
: Name(aName), numQCLevels(0)
{
	theQCLevels = NULL;
}


__fastcall QCMaterialClass::~QCMaterialClass()
{
}


void QCMaterialClass::LoadQCLevelsFromDatabase(TQuery* query)
{
    DeleteQCLevelClass();
    query -> ParamByName( "mat" )->AsString = Name;
    query -> Open();
    query -> First();
    numQCLevels = query -> RecordCount;
    theQCLevels = new QCLevelClass*[numQCLevels];

    for (int x = 0; x < numQCLevels; x++)
    {
        theQCLevels[x] = new QCLevelClass;;
        theQCLevels[x]->SetName(query -> FieldByName("qc_level") -> AsString);
        theQCLevels[x]->SetDescription(query -> FieldByName("level_descrip") -> AsString);
        theQCLevels[x]->SetMaterial(query -> FieldByName("material") -> AsString);
        query -> Next();
    }
    query -> Close();
}


void QCMaterialClass::LoadFromDatabase(TQuery* query, int recno)
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
            Name = query -> FieldByName("material") -> AsString;
            Description = query -> FieldByName("material_descrip") -> AsString;
        }
        catch(Exception& e)
        {
            AnsiString strError =
                "Error loading QCMaterial from database.\n" + e.Message;
            Application->MessageBox(
                strError.c_str(),
                "QCMaterialClass Error",MB_OK);
		}
    }
}

void QCMaterialClass::InsertToDatabase(TQuery* query)
{
//  write new record with NEW status
    query -> Close();
    query -> SQL -> Clear();
    query->SQL->Add(
        "Insert into qc_material(material, material_descrip) "
        "values ( :mat, :descrip) ");
    FormatSQL( query );

    try
    {
        query -> ExecSQL();
    }
    catch(Exception& e)
    {
        if(e.Message != "Error creating cursor handle")
        {
            AnsiString strError =
                "Error inserting new qc material.\n" + e.Message;
            Application->MessageBox(
                strError.c_str(),
                "QCMaterialClass Error",MB_OK);
		}
	}
	}


void QCMaterialClass::UpdateToDatabase(TQuery* query)
{
    // write updated record
    query -> SQL -> Clear();
    query->SQL->Add(
        "Update qc_material set "
        "material_descrip = :descrip "
        " where material = :mat ");
    FormatSQL( query );
    query -> Prepare();
    try
    {
        query -> ExecSQL();
    }
    catch(Exception& e)
    {
        if(e.Message != "Error creating cursor handle")
        {
            AnsiString strError =
                "Error updating qc material.\n" + e.Message;
            Application->MessageBox(
                strError.c_str(),
                "QCMaterialClass Error",MB_OK);
		}
	}
}

void QCMaterialClass::FormatSQL( TQuery* query )
{
    query -> ParamByName( "mat" )->AsString = Name;
    query -> ParamByName( "descrip" )->AsString = Description;
}

// get a pointer to the instance of QCLevelClass indexed by index from theQCLevels
// array held by this QCMaterialClass. This is a pointer to the actual instance,
// so changes may be made and Profiles added. This method returns NULL if index is
// greater than the number of codes held by this instance of QCMaterialClass
QCLevelClass* QCMaterialClass::GetQCLevel( int index )
{

    if(index < numQCLevels)
        return theQCLevels[index];
    else
        return NULL;
}

// get a pointer to the instance of QCLevelClass from theQCLevels array held by this
// QCMaterialClass whose name is IDENTICAL to QCLevelName. This is a pointer to the
// actual instance, so changes may be made and Profiles added. This method returns
// NULL if none of the QCLevelClasses held by this QCMaterialClass have names which
// are equal ( == ) to QCLevelName

QCLevelClass* QCMaterialClass::GetQCLevel( AnsiString QCLevelName )
{
    int index = 0;

    while(index < numQCLevels && QCLevelName != theQCLevels[index] -> GetName())
    {
        index++;
    }

    if(index < numQCLevels)
        return theQCLevels[index];
    else
        return NULL;
}

void QCMaterialClass::DeleteQCLevelClass()
{
// delete all QC tests, create anew from database

    if ( theQCLevels != NULL )
    {
        for (int index=0; index < numQCLevels; index++)
        {
            if ( theQCLevels[index] != NULL )
                delete theQCLevels[index];
        }
        delete[] theQCLevels;
        numQCLevels = 0;
        theQCLevels = NULL;
    }
}

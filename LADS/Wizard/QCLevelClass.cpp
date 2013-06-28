//---------------------------------------------------------------------------
#include <vcl.h>
#pragma hdrstop

#include "QCLevelClass.h"

//---------------------------------------------------------------------------
#pragma package(smart_init)

// use QCLevelClass to provide a named container for instances of QCTestClass

QCLevelClass::QCLevelClass()
{}

QCLevelClass::QCLevelClass(AnsiString aName)
: Name(aName)
{}

// This constructor instantiates an object of LevelClass which is an exact copy
// of aLevelClass
QCLevelClass::QCLevelClass(QCLevelClass& aLevelClass)
{
    Name = aLevelClass.Name;
    Description = aLevelClass.Description;
    Material = aLevelClass.Material;
}

__fastcall QCLevelClass::~QCLevelClass()
{
}


void QCLevelClass::LoadFromDatabase(TQuery* query, int recno)
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
            Name = query -> FieldByName("qc_level") -> AsString;
            Description = query -> FieldByName("lot_descrip") -> AsString;
            Material = query -> FieldByName("material") -> AsString;
        }
        catch(Exception& e)
        {
            AnsiString strError =
                "Error loading QCLevel from database.\n" + e.Message;
            Application->MessageBox(
                strError.c_str(),
                "QCLevelClass Error",MB_OK);
        }
    }
}

void QCLevelClass::LogRecord( AnsiString Text )
{
    AnsiString LogText;

    LogText = Text + " qc_level: ";
    LogText += "qc_level=" + Name + ",";
    LogText += "material=" + Material + ",";
    LogText += "material_descrip=" + Description + ",";
}

void QCLevelClass::InsertToDatabase(TQuery* query)
{
//  write new record with NEW status
    query -> Close();
    query -> SQL -> Clear();
    query->SQL->Add(
        "Insert into qc_level(qc_level, level_descrip, material) "
        "values ( :lev, :descrip, :mat) ");
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
                "Error inserting new qc level.\n" + e.Message;
            Application->MessageBox(
                strError.c_str(),
                "QCLevelClass Error",MB_OK);
		}
	}
}


void QCLevelClass::UpdateToDatabase(TQuery* query)
{
	//      write updated record
	query -> SQL -> Clear();
	query->SQL->Add(
		"Update qc_level set "
		"level_descrip = :descrip "
		" where qc_level = :lev and "
		"       material = :mat ");
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
				"Error updating qc level.\n" + e.Message;
			Application->MessageBox(
				strError.c_str(),
				"QCLevelClass Error",MB_OK);
		}
	}
}

void QCLevelClass::FormatSQL( TQuery* query )
{
	query -> ParamByName( "lev" )->AsString = Name;
	query -> ParamByName( "descrip" )->AsString = Description;
	query -> ParamByName( "mat" )->AsString = Material;
}

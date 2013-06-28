//---------------------------------------------------------------------------
#include <vcl.h>

#include "QCLotClass.h"
#include "QCTestClass.h"
#include "CentralTest1.h"
#include "LCDbTest.h"

#pragma hdrstop

//---------------------------------------------------------------------------
#pragma package(smart_init)

// use QCLotClass to provide a named container for instances of QCTestClass

QCLotClass::QCLotClass()
 : numQCTests( -1 ), theQCTests( NULL )
{
    QCID = 0;
}

QCLotClass::QCLotClass(AnsiString aName)
 : Name( aName ), numQCTests( -1 ), theQCTests( NULL )
{
	QCID = 0;
}

__fastcall QCLotClass::~QCLotClass()
{
    DeleteQCTestClass();
}

void QCLotClass::LoadTestsFromDatabase(TQuery* query)
{
	DeleteQCTestClass();
    numQCTests = query->RecordCount;
	theQCTests = new QCTestClass*[numQCTests]; // allow for adding one

    query -> First();

    float Mean;
    float StdDev;
	char chrMean[15], chrStdDev[15];
    for (int x = 0; x < numQCTests; x++)
	{
		theQCTests[x] = new QCTestClass;
        if(query -> FieldByName("qc_id") -> AsInteger == QCID)
        {
            int testID = query->FieldByName("test_cid") -> AsInteger;
            const String & testName = LCDbTests::records().get( testID ).getName();
                theQCTests[x]->SetName(testName);
                theQCTests[x]->SetQCTestCID( testID );
                theQCTests[x]->SetQCID(query -> FieldByName("qc_id") -> AsInteger);
                Mean = query -> FieldByName("packet_mean") -> AsFloat;
				std::sprintf( chrMean, "%.2f", Mean );
                theQCTests[x]->SetPacketMean( chrMean );
                StdDev = query -> FieldByName("packet_stddev") -> AsFloat;
                std::sprintf( chrStdDev, "%.2f", StdDev );
                theQCTests[x]->SetPacketStdDev( chrStdDev );
        }

        query -> Next();
     }
}

void QCLotClass::SortTestsAlphabetically()
{
    qsort( theQCTests, numQCTests, sizeof(QCTestClass*),
        SortByTest);
}

int SortByTest( const void *a, const void *b )
{
    QCTestClass** theQCTestA;
    QCTestClass** theQCTestB;
    AnsiString TestA = "";
    AnsiString TestB = "";

    theQCTestA = (QCTestClass**) a;
    theQCTestB = (QCTestClass**) b;

    TestA = (*theQCTestA)->GetName();
    TestB = (*theQCTestB)->GetName();

    return strcmp( TestA.c_str(), TestB.c_str() );
}

void QCLotClass::LoadFromDatabase(TQuery* query, int recno)
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
            QCID = query -> FieldByName("qc_id") -> AsInteger;
            Material = query -> FieldByName("material") -> AsString;
            Level = query -> FieldByName("qc_level") -> AsString;
            Name = query -> FieldByName("lot") -> AsString;
            Description = query -> FieldByName("lot_descrip") -> AsString;
            ArrivalDate = query -> FieldByName("arrival_date") -> AsString;
        }
        catch(Exception& e)
        {
            AnsiString strError =
                "Error loading QCLot from database.\n" + e.Message;
            Application->MessageBox(
                strError.c_str(),
                "QCLotClass Error",MB_OK);
		}
    }
}


void QCLotClass::InsertToDatabase(TQuery* query)
{
//  write new record with NEW status
    query -> Close();
    query -> SQL -> Clear();
    query->SQL->Add(
        "Insert into qc_lot(qc_id, material, qc_level, lot,"
        "lot_descrip, arrival_date )"
        "values ( :qcid, :mat, :lev, :lot, :descrip, :adate) ");
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
                "Error inserting new qc lot.\n" + e.Message;
            Application->MessageBox(
                strError.c_str(),
                "QCLotClass Error",MB_OK);
		}
	}
}


void QCLotClass::UpdateToDatabase(TQuery* query)
{
    //      write updated record
    query -> SQL -> Clear();
    query->SQL->Add(
        "Update qc_lot set "
        "material = :mat, "
        "qc_level = :lev, "
        "lot = :lot, "
        "lot_descrip = :descrip, "
        "arrival_date = :adate "
        " where qc_id = :qcid ");
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
                "Error updating qc lot.\n" + e.Message;
            Application->MessageBox(
                strError.c_str(),
                "QCLotClass Error",MB_OK);
					}
	}
	}

void QCLotClass::FormatSQL( TQuery* query )
{
    query -> ParamByName( "qcid" )->AsInteger = QCID;
    query -> ParamByName( "mat" )->AsString = Material;
    query -> ParamByName( "lev" )->AsString = Level;
    query -> ParamByName( "lot" )->AsString = Name;
	query -> ParamByName( "descrip" )->AsString = Description;
	query -> ParamByName( "adate" )->AsDateTime = ArrivalDate;
}


// get a pointer to the instance of QCTestClass indexed by index from theQCTests
// array held by this QCLotClass. This is a pointer to the actual instance,
// so changes may be made and Profiles added. This method returns NULL if index is
// greater than the number of codes held by this instance of QCLotClass
QCTestClass* QCLotClass::GetQCTest( int index )
{

    if(index < numQCTests)
        return theQCTests[index];
    else
        return NULL;
}

// get a pointer to the instance of QCTestClass from theQCTests array held by this
// QCLotClass whose name is IDENTICAL to QCTestName. This is a pointer to the
// actual instance, so changes may be made and Profiles added. This method returns
// NULL if none of the QCTestClasses held by this QCLotClass have names which
// are equal ( == ) to QCTestName

QCTestClass* QCLotClass::GetQCTest( AnsiString QCTestName )
{
    int index = 0;

    while(index < numQCTests && QCTestName != theQCTests[index] -> GetName())
    {
        index++;
    }

    if(index < numQCTests)
        return theQCTests[index];
    else
        return NULL;
}


// get a pointer to the instance of QCTestClass from theQCTests array held by this
// QCLotClass whose index is IDENTICAL to id. This is a pointer to the
// actual instance, so changes may be made and Profiles added. This method returns
// NULL if none of the QCTestClasses held by this QCLotClass have names which
// are equal ( == ) to QCTestName

QCTestClass* QCLotClass::GetQCTestByID( int id  )
{
    int index = 0;

    while(index < numQCTests && id != theQCTests[index] -> GetQCTestCID())
    {
        index++;
    }

    if(index < numQCTests)
        return theQCTests[index];
    else
        return NULL;
}

	// delete all QC tests, create a new from database

void QCLotClass::DeleteQCTestClass()
{
	if( theQCTests != NULL )
	{
		for (int index=0; index < numQCTests; index++)
				delete theQCTests[index];

		delete[] theQCTests;
		numQCTests = -1;
		theQCTests = NULL;
	}
}


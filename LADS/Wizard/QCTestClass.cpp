//---------------------------------------------------------------------------
#include <vcl.h>
#pragma hdrstop

#include "QCTestClass.h"

//---------------------------------------------------------------------------
#pragma package(smart_init)
#include <vcl.h>
#include <stdlib.h>

QCTestClass::QCTestClass()
:theName("default construction")
{
    QCTestCID = 0;
    QCID = 0;
    PacketMean = "0.00";
    PacketStdDev = "0.00";
}

QCTestClass::QCTestClass(AnsiString aName)
:theName(aName)
//:theName(aName),numProfiles(0),maxProfiles(5)
{

    QCTestCID = 0;
    QCID = 0;
    PacketMean = "0.00";
    PacketStdDev = "0.00";
}

QCTestClass::QCTestClass(QCTestClass& aQCTestClass)
{
    theName = aQCTestClass.theName;
    QCTestCID = aQCTestClass.QCTestCID;
    QCID = aQCTestClass.QCID;
    PacketMean = aQCTestClass.PacketMean;
    PacketStdDev = aQCTestClass.PacketStdDev;
}

// default destructor - will eventually implement clean up code
 __fastcall QCTestClass::~QCTestClass()
{
}

void QCTestClass::LogRecord( AnsiString Text )
{
    AnsiString LogText;

    LogText = Text + " qc_test: ";
    LogText += "qc_id=" + IntToStr( QCID ) + ",";
    LogText += "test_cid=" + IntToStr( QCTestCID ) + ",";
    LogText += "packet_mean=" + PacketMean + ",";
    LogText += "packet_stddev=" + PacketStdDev + ",";
}

void QCTestClass::InsertToDatabase(TQuery* query)
{
    query -> Close();
    query -> SQL -> Clear();
    query -> SQL -> Add(
        "Insert into qc_test(qc_id, test_cid, "
        "packet_mean, packet_stddev) "
        "values (:qcid, :tcid, :pm, :pstd)");
    FormatSQL( query );

    try
    {
        query -> ExecSQL();
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
                "Error inserting new QC test\n" + e.Message;
            Application->MessageBox(
                strError.c_str(),
                "QCTestClass Error",MB_OK);
		}
	}
}

void QCTestClass::UpdateToDatabase(TQuery* query)
{
    // write updated record
    query -> SQL -> Clear();
    query -> SQL -> Add(
        "Update qc_test set "
        "packet_mean = :pm, "
        "packet_stddev = :pstd "
        " where qc_id = :qcid and "
        "       test_cid = :tcid ");
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
                "Error updating QC test\n" + e.Message;
            Application->MessageBox(
                strError.c_str(),
                "QCTestClass Error",MB_OK);
		}
	}
}


void QCTestClass::DeleteFromDatabase(TQuery* query)
{
	LogRecord( "Deleting: " );

	// delete record
	query -> Close();
	query -> SQL -> Clear();
	query -> SQL -> Add(
			"delete from qc_test "\
			" where qc_id = :qcid and "
			"       test_cid = :tcid ");
	query -> ParamByName( "qcid" )->AsInteger = QCID;
	query -> ParamByName( "tcid" )->AsInteger = QCTestCID;

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
				"Error deleting QC test\n" + e.Message;
			Application->MessageBox(
				strError.c_str(),
				"QCTestClass Error",MB_OK);
		}
	}
}

void QCTestClass::FormatSQL( TQuery* query )
{
    query -> ParamByName( "qcid" )->AsInteger = QCID;
    query -> ParamByName( "tcid" )->AsInteger = QCTestCID;
    query -> ParamByName( "pm" )->AsFloat = StrToFloat( PacketMean );
    query -> ParamByName( "pstd" )->AsFloat = StrToFloat( PacketStdDev );
}


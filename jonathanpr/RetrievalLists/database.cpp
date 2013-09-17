#include "database.h"
#include "xdb.h"
#include "xquery.h"
#include "xexec.h"
#include <sstream>
#include "utils.h"
#include <Soap.XSBuiltIns.hpp>
#include "mainListDialog.h"
#pragma package(smart_init)

const String database::DEFINE_RETRIEVAL_CID = "retrieval_cid";
const String database::DEFINE_RETRIEVAL_DISCRIPTION = "retrieval_discription";
const String database::DEFINE_PROJECT_CID = "project_cid";
const String database::DEFINE_PROJECT_EXTERNAL_NAME = "project_externalname";
const String database::DEFINE_PROJECT_EXTERNAL_FULL = "project_externalfull";

const String database::DEFINE_DB_NAME = "db_name";

const String database::DEFINE_CRYOVIAL_ID = "cryovial_id";
const String database::DEFINE_CRYOVIAL_BOX_ID = "box_cid";
const String database::DEFINE_CRYOVIAL_POSITION = "cryovial_position";
const String database::DEFINE_CRYOVIAL_BARCODE = "cryovial_barcode";
const String database::DEFINE_CRYOVIAL_SPECIMEN_ID = "cryovial_sample_id";
const String database::DEFINE_CRYOVIAL_ALIQUOT_CID = "cryovial_aliquot_cid";
const String database::DEFINE_CRYOVIAL_TIMESTAMP = "cryovial_timestamp";
const String database::DEFINE_CRYOVIAL_STATUS = "cryovial_status";

const String database::DEFINE_BOXSTORE_RACK_CID = "rack_cid";
const String database::DEFINE_BOXSTORE_SLOT_POSITION = "slot_position";

const String database::DEFINE_BOXNAME_EXTERNAL_NAME = "external_name";

const String database::DEFINE_RESULT_ID = "result_id";
const String database::DEFINE_RESULT_TIME_STAMP = "result_time_stamp";
const String database::DEFINE_RESULT_STRING_VALUE = "result_string_value";
const String database::DEFINE_RESULT_NOTE_EXISTS = "result_note_exists";
const String database::DEFINE_RESULT_TEST_CID = "results_test_cid";

const String database::DEFINE_CRYOVIAL_STORE_BOXID = "cryovial_store_BoxID";
const String database::DEFINE_CRYOVIAL_STORE_RECORD_ID = "cryovial_store_recordid";
const String database::DEFINE_CRYOVIAL_RETRIEVAL_CID  = "cryovial_store_retrieval_cid";
const String database::DEFINE_RACK_EXTERNAL_NAME = "rack_externalname";
const String database::DEFINE_RACK_POSITION = "rack_position";

const String database::DEFINE_OBJECT_NAME_CID = "object_name_cid";
const String database::DEFINE_OBJECT_NAME_EXTERNAL_NAME = "object_name_ext_name";
const String database::DEFINE_OBJECT_NAME_EXTERNAL_FULL = "object_name_ext_full";
const String database::DEFINE_STORAGE_EXTERNAL_NAME = "storage_ext_name";
const String database::DEFINE_LOCATION_EXTERNAL_NAME = "location_ext_name";
const String database::DEFINE_ALIQUOT_EXTERNAL_NAME = "aliquot_ext_name";
const String database::DEFINE_TANK_EXTERNAL_NAME = "tank_ext_name";
const String database::DEFINE_TANK_CID = "tank_cid";

const String database::DEFINE_NOTE_TIME_STAMP = "note_timestamp";
const String database::DEFINE_NOTE_CONTENTS = "note_contents";
const String database::DEFINE_NOTE_ID = "note_id";

const String database::DEFINE_HISTORY_STATUS = "history_status";
const String database::DEFINE_SAMPLE_SOURCE_NAME = "sample_src_name";

const String database::DEFINE_TANK_MAP_POSITION = "tankmap_pos";
const String database::DEFINE_TANK_MAP_SHELF_NUMBER = "tankmap_shelf_num";

const String database::DEFINE_SPECIMEN_SAMPLE_ID = "specimen_sampleid";

const String database::DEFINE_STORAGE_CID = "storage_cid";
const String database::DEFINE_LOCATION_CID = "location_cid";

const String database::DEFINE_CRYOVIAL_STORE_STATUS_TEXT = "cry_store_status_text";
const String database::DEFINE_CRYOVIAL_STORE_DATETIME = "cry_store_dt";
const String database::DEFINE_CRYOVIAL_STORE_REMOVED = "cry_store_removed";
const String database::DEFINE_CRYOVIAL_STORE_STATUS = "cry_store_status";

const String database::DEFINE_BOX_STORE_DATETIME = "box_store_dt";
const String database::DEFINE_BOX_STORE_REMOVED = "box_store_removed";
const String database::DEFINE_BOXNAME_NOTES_EXISTS = "box_store_note_exists";

const String database::DEFINE_CRYOVIAL_NOTES_EXISTS = "cryovial_notes_exists";
const String database::DEFINE_CRYOVIAL_STORE_NOTES_EXISTS = "cryo_store_notes_exists";
const String database::DEFINE_RESULTS_NOTES_COUNT = "results_notes_count";
const String database::DEFINE_SPECIMEN_NOTES_EXISTS = "specimen_notes_exists";

database::database()
{
	m_bBackgroundProcessing = false;
}
//---------------------------------------------------------------------------

database::~database()
{

}
//---------------------------------------------------------------------------

void database::connect(String &selectDB)
{
	try
	{
		String dbName = "vnode_vlab::" + selectDB;
		m_dbCentral = std::auto_ptr<XDB>( new XDB( AnsiString(dbName.c_str()).c_str() ) );
		m_dbCentral->setErrorCallBack( dbErrorCallback );
		throwUnless ( m_dbCentral->open(), "Failed to connect!" );
	}
	catch( const std::string& msg )
	{
		onError( msg );
	}
	catch( const std::runtime_error& e )
	{
		onError( e.what() );
	}
}
//---------------------------------------------------------------------------

void database::connectProject(String &projectName)
{
	std::string connectionString = std::string("vnode_vlab::") + AnsiString(projectName.c_str()).c_str();
	m_dbProject = std::auto_ptr<XDB>( new XDB( connectionString ) );
	try
	{
		m_dbProject->setErrorCallBack( dbErrorCallback );
		throwUnless ( m_dbProject->open(), "Failed to connect!" );
        //new database. so clear the objectname mapping
		m_ObjectNameList.clear();
	}
	catch( const std::string& msg )
	{
		onError( msg );
	}
	catch( const std::runtime_error& e )
	{
		onError( e.what() );
	}
}
//---------------------------------------------------------------------------

void database::getProject(int project_cid,std::map<String,String> &data)
{
	std::wstringstream sqlquery;
	sqlquery << "select db_name from c_project where project_cid ='" << project_cid << "'";

	XQUERY query( m_dbCentral.get(), AnsiString(sqlquery.str().c_str()).c_str() );
	throwUnless( query.open(), "Failed to retreve retrival lists" );
	while ( query.fetch() )    //any data?
	{
		std::map<String,String> listInfo;
		data.insert(std::pair<String,String> (DEFINE_DB_NAME,String(query.result.getString(0).c_str())));
	}
	query.close();
}
 //---------------------------------------------------------------------------

void database::getAliquotLists(std::map<int,std::map<String,String> > &AliquotLists)
{
	AliquotLists.clear();
	std::wstringstream sqlquery;
	sqlquery << "select object_cid,external_name, external_full from c_object_name where object_type = '6' AND status <> '99'";

	XQUERY query( m_dbCentral.get(), AnsiString(sqlquery.str().c_str()).c_str() );
	throwUnless( query.open(), "Failed to retreve Aliquot lists" );
	while ( query.fetch() )    //any data?
	{
		std::map<String,String> listInfo;
		std::wstringstream ss;
		ss << query.result.getInt(0);
		listInfo.insert(std::pair<String,String> (DEFINE_OBJECT_NAME_CID,String(ss.str().c_str())));
		listInfo.insert(std::pair<String,String> (DEFINE_OBJECT_NAME_EXTERNAL_NAME,String(query.result.getString(1).c_str())));
		listInfo.insert(std::pair<String,String> (DEFINE_OBJECT_NAME_EXTERNAL_FULL,String(query.result.getString(2).c_str())));
		AliquotLists.insert(std::pair<int,std::map<String,String> > (query.result.getInt(0),listInfo));
	}
	query.close();
}
//---------------------------------------------------------------------------

void database::getExerciseLists(std::map<int,std::map<String,String> > &exerciseLists)
{
	exerciseLists.clear();
	std::wstringstream sqlquery;
	sqlquery << "select object_cid,external_name, external_full from c_object_name where object_type = '20' AND status <> '99'";

	XQUERY query( m_dbCentral.get(), AnsiString(sqlquery.str().c_str()).c_str() );
	throwUnless( query.open(), "Failed to retreve exercise lists" );
	while ( query.fetch() )    //any data?
	{
		std::map<String,String> listInfo;
		std::wstringstream ss;
		int project_cid = query.result.getInt(0);
		ss << query.result.getInt(0);
		listInfo.insert(std::pair<String,String> (DEFINE_OBJECT_NAME_CID,String(ss.str().c_str())));
		listInfo.insert(std::pair<String,String> (DEFINE_OBJECT_NAME_EXTERNAL_NAME,String(query.result.getString(1).c_str())));
		listInfo.insert(std::pair<String,String> (DEFINE_OBJECT_NAME_EXTERNAL_FULL,String(query.result.getString(2).c_str())));
		exerciseLists.insert(std::pair<int,std::map<String,String> > (query.result.getInt(0),listInfo));
	}
	query.close();
}

//---------------------------------------------------------------------------

void database::getProjectLists(std::map<int,std::map<String,String> > &ProjectLists)
{
	ProjectLists.clear();
	std::wstringstream sqlquery;
	sqlquery << "select project_cid,external_name, external_full from c_project where status <> '99'";

	XQUERY query( m_dbCentral.get(), AnsiString(sqlquery.str().c_str()).c_str() );
	throwUnless( query.open(), "Failed to retreve project lists" );
	while ( query.fetch() )    //any data?
	{
		std::map<String,String> listInfo;
		std::wstringstream ss;
		int project_cid = query.result.getInt(0);
		ss << query.result.getInt(0);
		listInfo.insert(std::pair<String,String> (DEFINE_PROJECT_CID,String(ss.str().c_str())));
		listInfo.insert(std::pair<String,String> (DEFINE_PROJECT_EXTERNAL_NAME,String(query.result.getString(1).c_str())));
		listInfo.insert(std::pair<String,String> (DEFINE_PROJECT_EXTERNAL_FULL,String(query.result.getString(2).c_str())));
		ProjectLists.insert(std::pair<int,std::map<String,String> > (query.result.getInt(0),listInfo));
	}
	query.close();
}
//---------------------------------------------------------------------------
void database::getRetrivalLists(std::map<int,std::map<String,String> > &newRetrivalLists, int exercise_cid)
{
	newRetrivalLists.clear();
	std::wstringstream sqlquery;
	sqlquery << "select retrieval_cid,description,project_cid from c_retrieval_job WHERE status <> '99' AND exercise_cid = '" << exercise_cid << "'";

	XQUERY query( m_dbCentral.get(), AnsiString(sqlquery.str().c_str()).c_str() );
	throwUnless( query.open(), "Failed to retreve retrival lists" );
	while ( query.fetch() )    //any data?
	{
		std::map<String,String> listInfo;
		std::wstringstream ss;
		int retrieval_cid = query.result.getInt(0);
		ss << query.result.getInt(0); //retrieval_cid
		listInfo.insert(std::pair<String,String> (DEFINE_RETRIEVAL_CID,String(ss.str().c_str())));
		ss.str( std::wstring() );
		ss.clear();
		listInfo.insert(std::pair<String,String> (DEFINE_RETRIEVAL_DISCRIPTION,String(query.result.getString(1).c_str())));
		ss << query.result.getInt(2); //project_cid
		listInfo.insert(std::pair<String,String> (DEFINE_PROJECT_CID,String(ss.str().c_str())));
		newRetrivalLists.insert(std::pair<int,std::map<String,String> > (retrieval_cid,listInfo));
	}
	query.close();
}

//---------------------------------------------------------------------------
void database::getRetrivalLists(std::map<int,std::map<String,String> > &newRetrivalLists)
{
	newRetrivalLists.clear();
	std::wstringstream sqlquery;
	sqlquery << "select retrieval_cid,description,project_cid from c_retrieval_job WHERE status <> '99'";

	XQUERY query( m_dbCentral.get(), AnsiString(sqlquery.str().c_str()).c_str() );
	throwUnless( query.open(), "Failed to retreve retrival lists" );
	while ( query.fetch() )    //any data?
	{
		std::map<String,String> listInfo;
		std::wstringstream ss;
		int retrieval_cid = query.result.getInt(0);
		ss << query.result.getInt(0); //retrieval_cid
		listInfo.insert(std::pair<String,String> (DEFINE_RETRIEVAL_CID,String(ss.str().c_str())));
		ss.str( std::wstring() );
		ss.clear();
		listInfo.insert(std::pair<String,String> (DEFINE_RETRIEVAL_DISCRIPTION,String(query.result.getString(1).c_str())));
		ss << query.result.getInt(2); //project_cid
		listInfo.insert(std::pair<String,String> (DEFINE_PROJECT_CID,String(ss.str().c_str())));
		newRetrivalLists.insert(std::pair<int,std::map<String,String> > (retrieval_cid,listInfo));
	}
	query.close();
}

//---------------------------------------------------------------------------

void database::getSampleNotes(String &sql_notes_id_list,std::map<int,std::map<String,String> > &results)
{
	std::wstringstream sqlquery;
	sqlquery << "select time_stamp,contents,object_id  ";
	sqlquery << "from note ";
	sqlquery << "where object_id IN (" << sql_notes_id_list.c_str() << ")";

	XQUERY query( m_dbProject.get(), AnsiString(sqlquery.str().c_str()).c_str() );
	throwUnless( query.open(), "Failed to retreve notes lists" );
	while ( query.fetch() )    //any data?
	{
		std::map<String,String> listInfo;
		std::wstringstream ss;
		ss << query.result.getDate(0).iso().c_str(); //time_stamp
		listInfo.insert(std::pair<String,String> (DEFINE_NOTE_TIME_STAMP,String(ss.str().c_str())));
		ss.str( std::wstring() );
		ss.clear();
		listInfo.insert(std::pair<String,String> (DEFINE_NOTE_CONTENTS,String(query.result.getString(1).c_str())));
		ss << query.result.getInt(2); //object_id
		listInfo.insert(std::pair<String,String> (DEFINE_NOTE_ID,String(ss.str().c_str())));
		ss.str( std::wstring() );
		ss.clear();
		results.insert(std::pair<int,std::map<String,String> > (results.size(),listInfo));
	}
	query.close();
}

//---------------------------------------------------------------------------

void database::getSampleNotes(int note_id,std::map<int,std::map<String,String> > &results)
{
	std::wstringstream sqlquery;
	String noteidString = System::Sysutils::IntToStr(note_id);
	sqlquery << "select time_stamp,contents  ";
	sqlquery << "from note ";
	sqlquery << "where object_id = '" << note_id << "'";

	XQUERY query( m_dbProject.get(), AnsiString(sqlquery.str().c_str()).c_str() );
	throwUnless( query.open(), "Failed to retreve notes lists" );
	while ( query.fetch() )    //any data?
	{
		std::map<String,String> listInfo;
		std::wstringstream ss;
		ss << query.result.getDate(0).iso().c_str(); //time_stamp
		listInfo.insert(std::pair<String,String> (DEFINE_NOTE_TIME_STAMP,String(ss.str().c_str())));
		ss.str( std::wstring() );
		ss.clear();
		listInfo.insert(std::pair<String,String> (DEFINE_NOTE_CONTENTS,String(query.result.getString(1).c_str())));
		listInfo.insert(std::pair<String,String> (DEFINE_NOTE_ID,noteidString));
		results.insert(std::pair<int,std::map<String,String> > (results.size(),listInfo));
	}
	query.close();
}
 //---------------------------------------------------------------------------
void database::runSampleResultSQL(std::wstring &sql,std::map<int,std::map<String,String> > &results,std::map<int,String>  &testnameResults)
{
	std::wstringstream test_cidList;
	{
		XQUERY query( m_dbProject.get(), AnsiString(sql.c_str()).c_str() );
		throwUnless( query.open(), "Failed to retreve sample lists" );
		int counter = 0;
		while ( query.fetch() )    //any data?
		{
			std::map<String,String> listInfo;
			std::wstringstream ss;
			ss << query.result.getInt(0); //result_id
			listInfo.insert(std::pair<String,String> (DEFINE_RESULT_ID,String(ss.str().c_str())));
			ss.str( std::wstring() );
			ss.clear();
			ss << query.result.getDate(1).iso().c_str(); //time_stamp
			listInfo.insert(std::pair<String,String> (DEFINE_RESULT_TIME_STAMP,String(ss.str().c_str())));
			ss.str( std::wstring() );
			ss.clear();
			listInfo.insert(std::pair<String,String> (DEFINE_RESULT_STRING_VALUE,String(query.result.getString(2).c_str())));
			ss << query.result.getInt(3); //result_id
			listInfo.insert(std::pair<String,String> (DEFINE_RESULT_NOTE_EXISTS,String(ss.str().c_str())));
			ss.str( std::wstring() );
			ss.clear();
			ss << query.result.getInt(4); //test_cid
			if (counter > 0)
				test_cidList << ",";
			test_cidList << "'" << ss.str().c_str() << "'";
			counter++;
			listInfo.insert(std::pair<String,String> (DEFINE_RESULT_TEST_CID,String(ss.str().c_str())));
			ss.str( std::wstring() );
			ss.clear();
			results.insert(std::pair<int,std::map<String,String> > (query.result.getInt(0),listInfo));
		}
		query.close();
	}

	if (test_cidList.str().empty())
		return;

	std::wstringstream sqlquery;
	sqlquery << "select test_cid,external_full from c_test where test_cid IN (";
	sqlquery << test_cidList.str() << ")";

	//now get the test name
	XQUERY query( m_dbCentral.get(), AnsiString(sqlquery.str().c_str()).c_str() );
	throwUnless( query.open(), "Failed to retreve retrival lists" );
	while ( query.fetch() )    //any data?
	{
		testnameResults.insert(std::pair<int,String> (query.result.getInt(0),String(query.result.getString(1).c_str())));
	}
	query.close();
}
//---------------------------------------------------------------------------

void database::getSampleResultForSameSource(String &sample_id,std::map<int,std::map<String,String> > &results,std::map<int,String>  &testnameResults)
{
	String discrType = "source_name";
	String source_nameColName = getDiscripColumnsFor(discrType);

	std::wstringstream sqlquery;
	sqlquery << "select result_id, time_stamp, string_value,note_exists,test_cid  ";
	sqlquery << " from result";
	sqlquery << " where result.sample_id IN (";
	sqlquery << " select specimen.sample_id from specimen where specimen." << source_nameColName.c_str() << " IN";
	sqlquery << " (select specimen." << source_nameColName.c_str() << " from specimen,cryovial where specimen.sample_id ='";
	sqlquery << sample_id.c_str();
	sqlquery << "'))";

	std::wstring sql = sqlquery.str();
	runSampleResultSQL(sql,results,testnameResults);
}


//---------------------------------------------------------------------------

void database::getSampleResult(String &cryovialBarcode,String &aliquot_cid,String &sample_cid,std::map<int,std::map<String,String> > &results,std::map<int,String>  &testnameResults)
{
//check frist with aliquot and barcode
	String discrType = "aliquot_type";
	String source_nameColName = getDiscripColumnsFor(discrType);
	String aliquotString = getExternalNameFromC_ObjectName(aliquot_cid, m_ObjectNameList);
	std::wstringstream sqlquery;
	sqlquery << "select result_id, time_stamp, string_value,note_exists,test_cid  ";
	sqlquery << " from result ";
	sqlquery << " where sample_id ";
	sqlquery << " IN (select sample_id from specimen where barcode = '" << cryovialBarcode.c_str() << "' AND ";
	sqlquery <<  source_nameColName.c_str() << "=" << "'" << aliquotString.c_str() << "')";

	std::wstring sql = sqlquery.str();
	runSampleResultSQL(sql,results,testnameResults);

	if (results.size() > 0)
		return; //found some results.

//now check with sampleid
	std::wstringstream sqlquery2;
	sqlquery2 << "select result_id, time_stamp, string_value,note_exists,test_cid  ";
	sqlquery2 << " from result ";
	sqlquery2 << " where sample_id = ";
	sqlquery2 << " '" << sample_cid.c_str() << "'";

	sql = sqlquery2.str();
	runSampleResultSQL(sql,results,testnameResults);
}

//---------------------------------------------------------------------------

void database::getSampleResult(String &sample_id,std::map<int,std::map<String,String> > &results,std::map<int,String>  &testnameResults)
{
	std::wstringstream sqlquery;
	sqlquery << "select result_id, time_stamp, string_value,note_exists,test_cid  ";
	sqlquery << "from result ";
	sqlquery << "where sample_id = '" << sample_id.c_str() << "'";
	std::wstring sql = sqlquery.str();
	runSampleResultSQL(sql,results,testnameResults);
}

//---------------------------------------------------------------------------
String database::retrieval_job_StatusToString(int iStatus)
{
	if (iStatus == 0)
		return L"new job";
	else if (iStatus == 1)
		return L"in progress";
	else if (iStatus == 2)
		return L"completed";
	else if (iStatus == 3)
		return L"invalid/obsolete";
	return L"unknown";
}

String database::retrieval_job_JobTypeToString(int iStatus)
{
	if (iStatus == 0)
		return L"unknown";
	else if (iStatus == 1)
		return L"box move";
	else if (iStatus == 2)
		return L"box retrieval";
	else if (iStatus == 3)
		return L"box disposal";
	else if (iStatus == 4)
		return L"cryovial retrieval";
	else if (iStatus == 5)
		return L"cryovial disposal";
	return L"unknown";
}
String database::cryovial_store_StatusToString(int iStatus)
{
	if (iStatus == 0)
		return L"Space Allocated";
	else if (iStatus == 1)
		return L"position confirmed";
	else if (iStatus == 2)
		return L"removal expected";
	else if (iStatus == 3)
		return L"removed for disposal";
	else if (iStatus == 4)
		return L"removed for analysis";
	else if (iStatus == 5)
		return L"moved to another box";
	else if (iStatus == 99)
		return L"record invalid/deleted";
	return L"unknown";
}
String database::cryovial_StatusToString(int iStatus)
{
	if (iStatus == 0)
		return L"New entery";
	else if (iStatus == 1)
		return L"stored/confirmed";
	else if (iStatus == 2)
		return L"contents transferred to other cryovials";
	else if (iStatus == 3)
		return L"disposed of";
	else if (iStatus == 99)
		return L"record invalid/deleted ";
	return L"unknown";
}

void database::outputRetrieval_jobInfo(String &retrieval_cid,TRichEdit *RichEdit)
{
	//Get cryovail_store enteries which aren't in the current list
	std::wstringstream sqlcenteralquery;
	sqlcenteralquery << "select ";
	sqlcenteralquery << "description ,job_type,status,start_date,finish_date ";
	sqlcenteralquery << " from C_RETRIEVAL_JOB";
	sqlcenteralquery << " where ";
	sqlcenteralquery << " retrieval_cid   ='" <<  retrieval_cid.c_str() << "'";
	XQUERY central_query( m_dbCentral.get(), AnsiString(sqlcenteralquery.str().c_str()).c_str() );
	throwUnless( central_query.open(), "Failed to retreve retrival jon info" );
	if ( central_query.fetch() )    //any data?
	{
		Utils::setRichTextType(Utils::FONT_NORMAL,RichEdit);
		RichEdit->SelText = L"\nRetrieval job has the discription ";
		Utils::setRichTextType(Utils::FONT_HI_LIGHT,RichEdit);
		RichEdit->SelText = String(central_query.result.getString(0).c_str());
		Utils::setRichTextType(Utils::FONT_NORMAL,RichEdit);
		RichEdit->SelText = L" and is of type ";
		Utils::setRichTextType(Utils::FONT_HI_LIGHT,RichEdit);
		RichEdit->SelText = retrieval_job_JobTypeToString(central_query.result.getInt(1));
		Utils::setRichTextType(Utils::FONT_NORMAL,RichEdit);
		RichEdit->SelText = L" and status is ";
		Utils::setRichTextType(Utils::FONT_HI_LIGHT,RichEdit);
		RichEdit->SelText = retrieval_job_StatusToString(central_query.result.getInt(2));
		if (central_query.result.getDate(3).isValid())
			RichEdit->SelText = L"Retrieval job started on " + String(central_query.result.getDate(3).iso().c_str());
		if (central_query.result.getDate(4).isValid())
			RichEdit->SelText = L" and finished " + String(central_query.result.getDate(4).iso().c_str());
		RichEdit->SelText =  L"\n";
	}
	central_query.close();
}


void database::toReadableDateString(String &dateString)
{
	if (dateString == "ERROR")
	{
		dateString = "unknown";
    	return;
	}
	TXSDateTime *DT;
	try
	{
		try
		{
			DT = new TXSDateTime();
			DT->XSToNative(dateString);
			TDateTime TTDT = DT->AsDateTime;
			dateString = DateToStr(TTDT);
		}
		catch (EConvertError &E)
		{
			//leave as it is
		}
		catch (...)
		{
			//some other problem...
			dateString = "unknown";
		}
	}
	__finally
	{
		delete DT;
	}
}

void database::fillRichEditInfo(std::map<String,String> &cryovialInfo,TRichEdit *RichEdit)
{
	RichEdit->Clear();
	//move to end
	RichEdit->SelStart = RichEdit->GetTextLen();

	Utils::setRichTextType(Utils::FONT_H1 ,RichEdit);
	RichEdit->SelText = L"Extended Information\n\n";

	Utils::setRichTextType(Utils::FONT_NORMAL,RichEdit);
	RichEdit->SelText = L"The Cryovial with the Barcode ";
	Utils::setRichTextType(Utils::FONT_HI_LIGHT,RichEdit);
	RichEdit->SelText =  cryovialInfo[DEFINE_CRYOVIAL_BARCODE];
	Utils::setRichTextType(Utils::FONT_NORMAL,RichEdit);
	RichEdit->SelText = L" and Aliquot ";
	Utils::setRichTextType(Utils::FONT_HI_LIGHT,RichEdit);
	RichEdit->SelText = cryovialInfo[DEFINE_ALIQUOT_EXTERNAL_NAME];
	Utils::setRichTextType(Utils::FONT_NORMAL,RichEdit);
	RichEdit->SelText = L" it has the status of ";
	Utils::setRichTextType(Utils::FONT_HI_LIGHT,RichEdit);
	RichEdit->SelText = cryovial_StatusToString(cryovialInfo[DEFINE_CRYOVIAL_STATUS].ToInt());
	Utils::setRichTextType(Utils::FONT_NORMAL,RichEdit);
	RichEdit->SelText = L".\n\nIts last storage entery states it was put into box ";
	Utils::setRichTextType(Utils::FONT_HI_LIGHT,RichEdit);
	RichEdit->SelText = cryovialInfo[DEFINE_BOXNAME_EXTERNAL_NAME];
	Utils::setRichTextType(Utils::FONT_NORMAL,RichEdit);
	RichEdit->SelText = L" in position ";
	Utils::setRichTextType(Utils::FONT_HI_LIGHT,RichEdit);
	RichEdit->SelText = cryovialInfo[DEFINE_CRYOVIAL_POSITION];
	Utils::setRichTextType(Utils::FONT_NORMAL,RichEdit);
	RichEdit->SelText = L" on ";
	Utils::setRichTextType(Utils::FONT_HI_LIGHT,RichEdit);
	if (cryovialInfo[DEFINE_CRYOVIAL_STORE_DATETIME].IsEmpty())
		RichEdit->SelText = L"Unknown date";
	else
		RichEdit->SelText = cryovialInfo[DEFINE_CRYOVIAL_STORE_DATETIME];
	Utils::setRichTextType(Utils::FONT_NORMAL,RichEdit);
	if ((!cryovialInfo[DEFINE_CRYOVIAL_STORE_REMOVED].IsEmpty()) &&
		(cryovialInfo[DEFINE_CRYOVIAL_STORE_REMOVED] != L"ERROR"))
	{
		RichEdit->SelText = L" and removed on ";
		Utils::setRichTextType(Utils::FONT_HI_LIGHT,RichEdit);
		RichEdit->SelText = cryovialInfo[DEFINE_CRYOVIAL_STORE_REMOVED];
	}
	Utils::setRichTextType(Utils::FONT_NORMAL,RichEdit);
	RichEdit->SelText = L" with the status ";
	Utils::setRichTextType(Utils::FONT_HI_LIGHT,RichEdit);
	RichEdit->SelText = cryovial_store_StatusToString(cryovialInfo[DEFINE_CRYOVIAL_STORE_STATUS].ToInt());
	RichEdit->SelText = L".\n";
	Utils::setRichTextType(Utils::FONT_NORMAL,RichEdit);
	if (cryovialInfo[DEFINE_BOXSTORE_RACK_CID].IsEmpty())
	{
		if (cryovialInfo[DEFINE_CRYOVIAL_STORE_STATUS].ToInt()==3)
			RichEdit->SelText = L" The box isn't registered in a location as it's been marked as disposed.\n";
		else
			RichEdit->SelText = L" The box isn't registered in a location.\n";
	}
	else
	{
		RichEdit->SelText = L"The box was/is at site ";
		Utils::setRichTextType(Utils::FONT_HI_LIGHT,RichEdit);
		RichEdit->SelText = cryovialInfo[DEFINE_LOCATION_EXTERNAL_NAME];
		Utils::setRichTextType(Utils::FONT_NORMAL,RichEdit);
		RichEdit->SelText = L" at the position ";
		Utils::setRichTextType(Utils::FONT_HI_LIGHT,RichEdit);
		RichEdit->SelText = cryovialInfo[DEFINE_TANK_MAP_POSITION] + L".";
		Utils::setRichTextType(Utils::FONT_NORMAL,RichEdit);
		RichEdit->SelText = L" The vessel name is ";
		Utils::setRichTextType(Utils::FONT_HI_LIGHT,RichEdit);
		RichEdit->SelText = cryovialInfo[DEFINE_STORAGE_EXTERNAL_NAME] + L", ";
		Utils::setRichTextType(Utils::FONT_NORMAL,RichEdit);
		RichEdit->SelText = L"it is on the rack ";
		Utils::setRichTextType(Utils::FONT_HI_LIGHT,RichEdit);
		RichEdit->SelText = cryovialInfo[DEFINE_RACK_EXTERNAL_NAME];
		Utils::setRichTextType(Utils::FONT_NORMAL,RichEdit);
		RichEdit->SelText = L", in postion ";
		Utils::setRichTextType(Utils::FONT_HI_LIGHT,RichEdit);
		RichEdit->SelText = cryovialInfo[DEFINE_BOXSTORE_SLOT_POSITION];
		Utils::setRichTextType(Utils::FONT_NORMAL,RichEdit);
		RichEdit->SelText = L" and on shelf ";
		Utils::setRichTextType(Utils::FONT_HI_LIGHT,RichEdit);
		RichEdit->SelText = cryovialInfo[DEFINE_TANK_MAP_SHELF_NUMBER];
		Utils::setRichTextType(Utils::FONT_NORMAL,RichEdit);
		RichEdit->SelText = L".\n";
	}
	if (cryovialInfo[DEFINE_CRYOVIAL_RETRIEVAL_CID] == L"0")
	{
 //		RichEdit->SelText = L"\nThere is no retreival job associated with it.\n";
	}
	else
	{
		RichEdit->SelText = L"\nIt was put in that box due to a retreival job.\n";
		outputRetrieval_jobInfo(cryovialInfo[DEFINE_CRYOVIAL_RETRIEVAL_CID],RichEdit);
	}
	int location_count = 0;
	std::map<int,std::map<String,String> > HistoryData;
	{
/*
select cryovial_store.record_id,cryovial_store.box_cid,cryovial_store.cryovial_position,cryovial_store.status,
char(cryovial_store.time_stamp),IFNULL(char(cryovial_store.removed),char('ERROR')) AS removed,
cryovial_store.retrieval_cid  from cryovial_store where cryovial_store.record_id <> '4116129'
AND cryovial_id  ='2987' ORDER BY time_stamp DESC
  */
		//Get cryovail_store enteries which aren't in the current list
		std::wstringstream sqlquery;
		sqlquery << "select ";
		sqlquery << "cryovial_store.record_id,cryovial_store.box_cid,cryovial_store.cryovial_position,cryovial_store.status,char(cryovial_store.time_stamp),IFNULL(char(cryovial_store.removed),char('ERROR')) AS removed,cryovial_store.retrieval_cid ";
		sqlquery << " from cryovial_store";
		sqlquery << " where cryovial_store.record_id <> '" << cryovialInfo[DEFINE_CRYOVIAL_STORE_RECORD_ID].c_str();
		sqlquery << "' AND cryovial_id  ='" << cryovialInfo[DEFINE_CRYOVIAL_ID].c_str() << L"' ORDER BY time_stamp DESC";
		XQUERY project_query( m_dbProject.get(), AnsiString(sqlquery.str().c_str()).c_str() );
		throwUnless( project_query.open(), "Failed to retreve retrival lists" );
		while ( project_query.fetch() )    //any data?
		{
			location_count++;
			std::map<String,String> temp;
			String cryovial_store_record_id = System::Sysutils::IntToStr(project_query.result.getInt(0));
			String cryovial_store_box_cid = System::Sysutils::IntToStr(project_query.result.getInt(1));
			String cryovial_store_cryovial_position = System::Sysutils::IntToStr(project_query.result.getInt(2));
			String cryovial_store_status = System::Sysutils::IntToStr(project_query.result.getInt(3));
			String cryovial_store_time_stamp = String(project_query.result.getString(4).c_str());
			String cryovial_store_removed =  String(project_query.result.getString(5).c_str());
			String cryovial_store_retrieval_cid = System::Sysutils::IntToStr(project_query.result.getInt(6));
//THRIVE LP(1) analasys
//show extended information/cryovile
			cryovial_store_time_stamp=cryovial_store_time_stamp.Trim();
			cryovial_store_removed=cryovial_store_removed.Trim();
			toReadableDateString(cryovial_store_time_stamp);
			toReadableDateString(cryovial_store_removed);
			temp.insert(std::pair<String,String> ("cryovial_store_record_id",cryovial_store_record_id));
			temp.insert(std::pair<String,String> ("cryovial_store_box_cid",cryovial_store_box_cid));
			temp.insert(std::pair<String,String> ("cryovial_store_cryovial_position",cryovial_store_cryovial_position));
			temp.insert(std::pair<String,String> ("cryovial_store_status",cryovial_store_status));
			temp.insert(std::pair<String,String> ("cryovial_store_time_stamp",cryovial_store_time_stamp));
			temp.insert(std::pair<String,String> ("cryovial_store_removed",cryovial_store_removed));
			temp.insert(std::pair<String,String> ("cryovial_store_retrieval_cid",cryovial_store_retrieval_cid));
			HistoryData.insert(std::pair<int,std::map<String,String> > (project_query.result.getInt(0),temp));
		}
		project_query.close();
	}
	std::wstringstream sqlquery;
	sqlquery << "select box_name.external_name,box_store.rack_cid,box_store.slot_position from box_name LEFT JOIN box_store ON box_name.box_cid = box_store.box_cid where box_name.box_cid = '";//-34179'

	std::map	<int,std::map<String,String> > tempData;

	for (std::map<int,std::map<String,String> > ::iterator it = HistoryData.begin(); it != HistoryData.end(); ++it)
	{
		int key = it->first;
		std::map<String,String> temp = it->second;
		String sql = String(sqlquery.str().c_str()) + temp["cryovial_store_box_cid"] + L"'";

		XQUERY project_query( m_dbProject.get(), AnsiString(sql.c_str()).c_str() );
		throwUnless( project_query.open(), "Failed to retreve retrival lists" );
		try{
			while ( project_query.fetch() )    //any data?
			{
				String box_name_external_name = String(project_query.result.getString(0).c_str());
				String box_store_rack_cid = System::Sysutils::IntToStr(project_query.result.getInt(1));
				String box_store_slot_position = System::Sysutils::IntToStr(project_query.result.getInt(2));
				temp.insert(std::pair<String,String> ("box_name_external_name",box_name_external_name));
				temp.insert(std::pair<String,String> ("box_store_rack_cid",box_store_rack_cid));
				temp.insert(std::pair<String,String> ("box_store_slot_position",box_store_slot_position));
	//			HistoryData.erase(key);
	//			HistoryData.insert(std::pair<int,std::map<String,String> > (key,temp));
				tempData.insert(std::pair<int,std::map<String,String> > (key,temp));
			}
		}
		catch(...)
		{ //FETCH() THROWS WHEN IT GETS A NULL VALUE. NOT SURE WHY IT DOES THIS
		}
		project_query.close();
	}
 //update the new data
	for (std::map<int,std::map<String,String> >  ::iterator it = tempData.begin(); it != tempData.end(); ++it)
	{
		HistoryData[it->first] = it->second;
	}
	if (location_count == 0)
		RichEdit->SelText = L"\nThe cryovial has no details of other previous locations.";
	else if (location_count == 1)
		RichEdit->SelText = L"\nThe cryovial has details of a prevoious storage location.";
	else
		RichEdit->SelText = L"\nThe cryovial has " + System::Sysutils::IntToStr(location_count)  + " storage locations in its history.";
	for (std::map<int,std::map<String,String> > ::iterator it = HistoryData.begin(); it != HistoryData.end(); ++it)
	{
		String key = it->first;
		std::map<String,String> temp = it->second;
		{
			RichEdit->SelText = L"\n\nIt was in box ";
			Utils::setRichTextType(Utils::FONT_HI_LIGHT,RichEdit);
			RichEdit->SelText = temp["box_name_external_name"];
			Utils::setRichTextType(Utils::FONT_NORMAL,RichEdit);
			RichEdit->SelText = L" in position ";
			Utils::setRichTextType(Utils::FONT_HI_LIGHT,RichEdit);
			RichEdit->SelText = temp["cryovial_store_cryovial_position"];
			Utils::setRichTextType(Utils::FONT_NORMAL,RichEdit);
			RichEdit->SelText = L" on ";
			Utils::setRichTextType(Utils::FONT_HI_LIGHT,RichEdit);
			if (temp["cryovial_store_time_stamp"].IsEmpty())
				RichEdit->SelText = L"Unknown date";
			else
				RichEdit->SelText = temp["cryovial_store_time_stamp"];
			Utils::setRichTextType(Utils::FONT_NORMAL,RichEdit);
			if (!String(temp["cryovial_store_removed"]).IsEmpty())
			{
				RichEdit->SelText = L" and removed on ";
				Utils::setRichTextType(Utils::FONT_HI_LIGHT,RichEdit);
				RichEdit->SelText = temp["cryovial_store_removed"];
			}
			Utils::setRichTextType(Utils::FONT_NORMAL,RichEdit);
			RichEdit->SelText = L" with the status ";
			Utils::setRichTextType(Utils::FONT_HI_LIGHT,RichEdit);
			RichEdit->SelText = cryovial_store_StatusToString(temp["cryovial_store_status"].ToInt()) + L".\n";
			Utils::setRichTextType(Utils::FONT_NORMAL,RichEdit);
			if (temp["cryovial_store_box_cid"].IsEmpty())
			{
				RichEdit->SelText = L" The box isn't registered in a location.\n";
			}
			else
			{
				std::map<String,String> tempTankData;
	  //			String TempString = IntToStr(project_query.result.getInt(1));
				extractDataFromRack_Number(temp["box_store_rack_cid"],tempTankData);
				String tank_cid = tempTankData[database::DEFINE_TANK_CID];
				extractDataFromTank_Map(tank_cid,tempTankData);
				String storage_cid = tempTankData[database::DEFINE_STORAGE_CID];
				String location_cid = tempTankData[database::DEFINE_LOCATION_CID];

				{
					std::wstringstream sqlquery;
					sqlquery << "select object_cid,external_name from c_object_name where object_cid IN ('";
					sqlquery << storage_cid.c_str() << "','" << location_cid.c_str() << "','" << tank_cid.c_str() << "')";
					XQUERY central_query( m_dbCentral.get(), AnsiString(sqlquery.str().c_str()).c_str() );
					throwUnless( central_query.open(), "Failed to retreve retrival lists" );
					while ( central_query.fetch() )    //any data?
					{
						if (!storage_cid.IsEmpty())
						{
							if (central_query.result.getInt(0) == storage_cid.ToInt())
								tempTankData.insert(std::pair<String,String> (DEFINE_STORAGE_EXTERNAL_NAME,central_query.result.getString(1).c_str()));
						}
						if (!location_cid.IsEmpty())
						{
							if (central_query.result.getInt(0) == location_cid.ToInt())
								tempTankData.insert(std::pair<String,String> (DEFINE_LOCATION_EXTERNAL_NAME,central_query.result.getString(1).c_str()));
						}
						if (!tank_cid.IsEmpty())
						{
							if (central_query.result.getInt(0) == tank_cid.ToInt())
								tempTankData.insert(std::pair<String,String> (DEFINE_TANK_EXTERNAL_NAME,central_query.result.getString(1).c_str()));
						}
					}
					central_query.close();
				}
				if (tempTankData[DEFINE_LOCATION_EXTERNAL_NAME].IsEmpty())
				{
					//unable to pick up on this information
				}
				else
				{
					RichEdit->SelText = L" The box was at site ";
					Utils::setRichTextType(Utils::FONT_HI_LIGHT,RichEdit);
					RichEdit->SelText = tempTankData[DEFINE_LOCATION_EXTERNAL_NAME];
					Utils::setRichTextType(Utils::FONT_NORMAL,RichEdit);
					RichEdit->SelText = L" at the position ";
					Utils::setRichTextType(Utils::FONT_HI_LIGHT,RichEdit);
					RichEdit->SelText = tempTankData[DEFINE_TANK_MAP_POSITION] + L".";
					Utils::setRichTextType(Utils::FONT_NORMAL,RichEdit);
					RichEdit->SelText = L" The vessel name is ";
					Utils::setRichTextType(Utils::FONT_HI_LIGHT,RichEdit);
					RichEdit->SelText = tempTankData[DEFINE_STORAGE_EXTERNAL_NAME] + L", ";
					Utils::setRichTextType(Utils::FONT_NORMAL,RichEdit);
					RichEdit->SelText = L"it is on the rack ";
					Utils::setRichTextType(Utils::FONT_HI_LIGHT,RichEdit);
					RichEdit->SelText = tempTankData[DEFINE_RACK_EXTERNAL_NAME];
					Utils::setRichTextType(Utils::FONT_NORMAL,RichEdit);
					RichEdit->SelText = L", in postion ";
					Utils::setRichTextType(Utils::FONT_HI_LIGHT,RichEdit);
					RichEdit->SelText = temp["box_store_slot_position"];
					Utils::setRichTextType(Utils::FONT_NORMAL,RichEdit);
					RichEdit->SelText = L" and on shelf ";
					Utils::setRichTextType(Utils::FONT_HI_LIGHT,RichEdit);
					RichEdit->SelText = tempTankData[DEFINE_TANK_MAP_SHELF_NUMBER];
					RichEdit->SelText = L".";
					Utils::setRichTextType(Utils::FONT_NORMAL,RichEdit);
				}
			}
		}
		if (temp["cryovial_store_retrieval_cid"].ToInt() == 0)
		{
			RichEdit->SelText = L" There is no retreival job associated with it.\n";
		}
		else
		{
			RichEdit->SelText = L" It was put in that box due to a retreival job.\n";
			String s = temp["cryovial_store_retrieval_cid"];
			outputRetrieval_jobInfo(s,RichEdit);
		}
	}
}
//---------------------------------------------------------------------------
void database::runcryovialSQL(std::wstring &sqlQuery,std::map<int,std::map<String,String> > &results)
{
	{
		int cryoid = 0;
		std::map<String,String> listInfo;
		XQUERY query( m_dbProject.get(), AnsiString(sqlQuery.c_str()).c_str() );
		throwUnless( query.open(), "Failed to retreve retrival lists" );
		while ( query.fetch() )    //any data?
		{
			listInfo.clear();
			std::wstringstream ss;
			cryoid = query.result.getInt(0);
			ss << query.result.getInt(0); //cryovial_store.cryovial_id
			listInfo.insert(std::pair<String,String> (DEFINE_CRYOVIAL_ID,String(ss.str().c_str())));
			ss.str( std::wstring() ); ss.clear();
			//cryovial.cryovial_barcode
			listInfo.insert(std::pair<String,String> (DEFINE_CRYOVIAL_BARCODE,String(query.result.getString(1).c_str())));
			ss << query.result.getInt(2); //cryovial.aliquot_type_cid
			listInfo.insert(std::pair<String,String> (DEFINE_CRYOVIAL_ALIQUOT_CID,String(ss.str().c_str())));
			ss.str( std::wstring() ); ss.clear();
			ss << query.result.getInt(3); //cryovial.sample_id
			listInfo.insert(std::pair<String,String> (DEFINE_CRYOVIAL_SPECIMEN_ID,String(ss.str().c_str())));
			ss.str( std::wstring() ); ss.clear();
			//cryovial.note_exists,
			listInfo.insert(std::pair<String,String> (DEFINE_CRYOVIAL_NOTES_EXISTS,System::Sysutils::IntToStr(query.result.getInt(4)).c_str()));
//			listInfo.insert(std::pair<String,String> (DEFINE_CRYOVIAL_TIMESTAMP,query.result.getDate(5).iso().c_str()));
			listInfo.insert(std::pair<String,String> (DEFINE_CRYOVIAL_TIMESTAMP,String(query.result.getString(5).c_str())));
			ss << query.result.getInt(6); //cryovial.sample_id
			listInfo.insert(std::pair<String,String> (DEFINE_CRYOVIAL_STATUS,String(ss.str().c_str())));
			ss.str( std::wstring() ); ss.clear();
			//map key is cryovialID
			results.insert(std::pair<int,std::map<String,String> > (cryoid,listInfo));
		}
		query.close();

		//AS WE MAY HAVE A HISTORY OF CRYOVAIL_STORE, WE NEED THE LASTEST ONE
		std::wstringstream sqlquery;
		sqlquery << "select TOP 1 ";
		sqlquery << "record_id,box_cid,cryovial_position,note_exists,status,retrieval_cid,time_stamp";
		sqlquery << " from cryovial_store";
		sqlquery << " where cryovial_id  ='";

		std::map<int,std::map<String,String> > newMap;

		for (std::map<int,std::map<String,String> >::iterator it = results.begin(); it != results.end(); ++it)
		{
			int cryoid = it->first;
			std::map<String,String> listInfo(it->second);

			std::wstring temp =  sqlquery.str() + listInfo[DEFINE_CRYOVIAL_ID].c_str() + L"' ORDER BY time_stamp DESC";

			XQUERY query2( m_dbProject.get(), AnsiString(temp.c_str()).c_str() );
			throwUnless( query2.open(), "Failed to retreve retrival lists" );
			if ( query2.fetch() )    //any data?
			{
				std::wstringstream ss;
				ss << query2.result.getInt(0); //cryovial_store.record_id
				listInfo.insert(std::pair<String,String> (DEFINE_CRYOVIAL_STORE_RECORD_ID,String(ss.str().c_str())));
				ss.str( std::wstring() ); ss.clear();
				ss << query2.result.getInt(1); //cryovial_store.box_cid
				listInfo.insert(std::pair<String,String> (DEFINE_CRYOVIAL_BOX_ID,String(ss.str().c_str())));
				ss.str( std::wstring() ); ss.clear();
				ss << query2.result.getInt(2); //cryovial_store.cryovial_position
				listInfo.insert(std::pair<String,String> (DEFINE_CRYOVIAL_POSITION,String(ss.str().c_str())));
				ss.str( std::wstring() ); ss.clear();
				listInfo.insert(std::pair<String,String> (DEFINE_CRYOVIAL_STORE_NOTES_EXISTS,System::Sysutils::IntToStr(query2.result.getInt(3)).c_str()));
				ss << query2.result.getInt(4); //cryovial_store.cryovial_position
				listInfo.insert(std::pair<String,String> (DEFINE_CRYOVIAL_STORE_STATUS,String(ss.str().c_str())));
				ss.str( std::wstring() ); ss.clear();
				ss << query2.result.getInt(5);
				listInfo.insert(std::pair<String,String> (DEFINE_CRYOVIAL_RETRIEVAL_CID,String(ss.str().c_str())));
				ss.str( std::wstring() ); ss.clear();
				String S = String(query2.result.getDate(6).iso().c_str());
				toReadableDateString(S);
				listInfo.insert(std::pair<String,String> (DEFINE_CRYOVIAL_STORE_DATETIME,S));
			}
			query2.close();
			newMap.insert(std::pair<int,std::map<String,String> > (cryoid,listInfo));
		}
		results = newMap;
	}
}

//---------------------------------------------------------------------------

void database::getCryovialListsWithSampleID(String &selectedCryovialSampleID,std::map<int,std::map<String,String> > &cryoLists)
{
	std::wstringstream sqlquery;
	sqlquery << "select ";
	sqlquery << "cryovial.cryovial_id, cryovial.cryovial_barcode,cryovial.aliquot_type_cid,cryovial.sample_id,cryovial.note_exists,char(cryovial.time_stamp),cryovial.status";
	sqlquery << " from cryovial";
	sqlquery << " where cryovial.sample_id ='";
	sqlquery << selectedCryovialSampleID.c_str() << "'";
	std::wstring temp = sqlquery.str();

	runcryovialSQL(temp,cryoLists);
}

//---------------------------------------------------------------------------

void database::getCryovialListsWithCryovialid(String &Cryovialid,std::map<int,std::map<String,String> > &cryoLists)
{
	std::wstringstream sqlquery;
	sqlquery << "select ";
	sqlquery << "cryovial.cryovial_id,cryovial.cryovial_barcode,cryovial.aliquot_type_cid,cryovial.sample_id,cryovial.note_exists,char(cryovial.time_stamp),cryovial.status";
	sqlquery << " from cryovial";
	sqlquery << " where cryovial.cryovial_id ='";
	sqlquery << Cryovialid.c_str() << "'";
	std::wstring temp = sqlquery.str();

	runcryovialSQL(temp,cryoLists);
}

//---------------------------------------------------------------------------

void database::getCryovialListsWithBarcodeAndAliquot(String &CryoBarCode,String &Alquot_cid,std::map<int,std::map<String,String> > &cryoLists)
{
	//generate list of barcodes for the SQL
	std::wstringstream barcodesql;

	barcodesql << " cryovial.sample_id IN (select sample_id from specimen where barcode = '";
	barcodesql << CryoBarCode.c_str();
	barcodesql << "') AND";
	barcodesql <<  " cryovial.aliquot_type_cid = '" << Alquot_cid.c_str() << "'";

	cryoLists.clear();
	std::wstringstream sqlquery;
	sqlquery << "select ";
	sqlquery << "cryovial.cryovial_id,cryovial.cryovial_barcode,cryovial.aliquot_type_cid,cryovial.sample_id,cryovial.note_exists,char(cryovial.time_stamp),cryovial.status";
	sqlquery << " from cryovial";
	sqlquery << " where ";
	sqlquery << barcodesql.str();
	std::wstring temp = sqlquery.str();

	runcryovialSQL(temp,cryoLists);
}


//---------------------------------------------------------------------------

void database::getCryovialListsWithSrcName(String &srcName,std::map<int,std::map<String,String> > &cryoLists)
{
	String discrType = "source_name";
	String source_nameColName = getDiscripColumnsFor(discrType);

	cryoLists.clear();
	std::wstringstream sqlquery;
	std::wstringstream sqlquery_barcode;

//BUGFIX - X library throws when it gets a timestamp as YYYY/MM/DD rather then YYYY/MM/DD HH/MM/SS
//it can't convert it. Adding char(TIMESTAMP) turns it into a string, usful if you want to display it
//only and not use it as a datetime.

	sqlquery << "select ";
	sqlquery << "cryovial.cryovial_id,cryovial.cryovial_barcode,cryovial.aliquot_type_cid,cryovial.sample_id,cryovial.note_exists,char(cryovial.time_stamp),cryovial.status";
	sqlquery << " from cryovial";
	sqlquery << " where ";
	sqlquery_barcode << sqlquery.str() << " cryovial.cryovial_barcode IN (SELECT barcode FROM specimen WHERE " << source_nameColName.c_str() << " = '" << srcName.c_str() << "')";
	std::wstring temp = sqlquery_barcode.str();

	runcryovialSQL(temp,cryoLists);

	if (cryoLists.size() > 0) //did we find some using the barcode?
	   return;

	//lets try sample_id.. this works for Feb 2005 retreavl list
	sqlquery << " cryovial.sample_id IN (SELECT sample_id FROM specimen WHERE " << source_nameColName.c_str() << " = '" << srcName.c_str() << "')";
	temp = sqlquery.str();

	runcryovialSQL(temp,cryoLists);
}

//---------------------------------------------------------------------------


void database::getCryovialListsWithBarCode(String &CryoBarCode,std::map<int,std::map<String,String> > &cryoLists)
{
	//generate list of barcodes for the SQL
	std::wstringstream barcodesql;

	barcodesql << " cryovial.sample_id IN (select sample_id from specimen where barcode = '";
	barcodesql << CryoBarCode.c_str();
	barcodesql << "')";

	cryoLists.clear();
	std::wstringstream sqlquery;
	sqlquery << "select ";
	sqlquery << "cryovial.cryovial_id,cryovial.cryovial_barcode,cryovial.aliquot_type_cid,cryovial.sample_id,char(cryovial.time_stamp),cryovial.status";
	sqlquery << " from cryovial";
	sqlquery << " where ";
	sqlquery << barcodesql.str();
	std::wstring temp = sqlquery.str();

	runcryovialSQL(temp,cryoLists);
}
//---------------------------------------------------------------------------


void database::getCryovialLists(String &project_id,std::vector<String> &sample_barcodes,std::map<int,std::map<String,String> > &cryoLists)
{
	//generate list of barcodes for the SQL
	std::wstringstream barcodesql;

	barcodesql << " cryovial.sample_id IN (select sample_id from specimen where barcode IN('";
	for (unsigned int i=0; i < sample_barcodes.size(); i++)
	{
		if (i > 0)
			barcodesql << "','";
	   barcodesql << sample_barcodes[i].c_str();
	}
	barcodesql << "'))";

	cryoLists.clear();
	std::wstringstream sqlquery;
	sqlquery << "select ";
	sqlquery << "cryovial.cryovial_id,cryovial.cryovial_barcode,cryovial.aliquot_type_cid,cryovial.sample_id,cryovial.note_exists,char(cryovial.time_stamp),cryovial.status";
	sqlquery << " from cryovial";
	sqlquery << " where ";
	sqlquery << barcodesql.str();
	std::wstring temp = sqlquery.str();

	runcryovialSQL(temp,cryoLists);
}
//---------------------------------------------------------------------------

String database::getDiscripColumnsFor(String &type)
{
	String endResult = "";

	std::wstringstream sqlquery;
	sqlquery << "select specimen_field_name from descrip where descriptor_name = '";
	sqlquery << type.c_str() << "'";
	XQUERY query( m_dbProject.get(), AnsiString(sqlquery.str().c_str()).c_str() );
	throwUnless( query.open(), "Failed to retreve retrival lists" );
	if ( query.fetch() )    //any data?
	{
		std::wstringstream ss;
		endResult = query.result.getString(0).c_str();
	}
	query.close();
	return  endResult;
}
//---------------------------------------------------------------------------

void database::getDiscripStringsFor(String &type, std::vector<String> &stringList)
{
	std::wstringstream sqlquery;
	//descriptor_value.value_name looks like a full name, where is they seam to use min_value/max_value for the text
	sqlquery << "select min_value from descriptor_value where descriptor_id IN (select descriptor_id from descrip where descriptor_name = '";
	sqlquery << type.c_str() << "')";

	XQUERY query( m_dbProject.get(), AnsiString(sqlquery.str().c_str()).c_str() );
	throwUnless( query.open(), "Failed to retreve retrival lists" );
	while ( query.fetch() )    //any data?
	{
		stringList.push_back(query.result.getString(0).c_str());
	}
	query.close();
}
//---------------------------------------------------------------------------

String database::getExternalNameFromC_ObjectName(String &object_cid, std::map<String,String> &List)
{
	if (List[object_cid].Length() > 0)
	{
		return List[object_cid];
	}

	std::wstringstream sqlquery;
	//descriptor_value.value_name looks like a full name, where is they seam to use min_value/max_value for the text
	sqlquery << "select external_name from c_object_name where object_cid = '";
	sqlquery << object_cid.c_str() << "'";

	XQUERY query( m_dbCentral.get(), AnsiString(sqlquery.str().c_str()).c_str() );
	throwUnless( query.open(), "Failed to retreve retrival lists" );
	if ( query.fetch() )    //any data?
	{
		List[object_cid] = query.result.getString(0).c_str();
	}
	query.close();
	return List[object_cid];
}

//---------------------------------------------------------------------------
bool database::hasAlquotTypeInSpecimenDescr(String &AliquotName)
{
	std::wstringstream sqlquery;
	sqlquery << "select TOP 1 sample_id";
	sqlquery << " from specimen where ";
	for (int i=1; i <= 25; i++)
	{
		if (i>1)
		{
			sqlquery <<  " OR";
		}
		sqlquery << " value" << i << "='" << AliquotName.c_str() << "'";
	}

	XQUERY project_query( m_dbProject.get(), AnsiString(sqlquery.str().c_str()).c_str() );
	throwUnless( project_query.open(), "Failed to retreve retrival lists" );
	while ( project_query.fetch() )    //any data?
	{
		project_query.close();
		return true;
	}
	project_query.close();
	return false;
}

//---------------------------------------------------------------------------
void database::extractDataFromRack_Number(String &boxStore_id,std::map<String,String> &temp)
{
	if (boxStore_id.Length() > 0)
	{
		std::wstringstream sqlquery;
		sqlquery << "select external_name,tank_cid from c_rack_number where rack_cid = '" << boxStore_id.c_str() << "'";
		XQUERY central_query( m_dbCentral.get(), AnsiString(sqlquery.str().c_str()).c_str() );
		throwUnless( central_query.open(), "Failed to retreve retrival lists" );
		if ( central_query.fetch() )    //any data?
		{
			std::wstringstream ss;
			temp.insert(std::pair<String,String> (DEFINE_RACK_EXTERNAL_NAME,central_query.result.getString(0).c_str()));
			ss << central_query.result.getInt(1);
			temp.insert(std::pair<String,String> (DEFINE_TANK_CID,ss.str().c_str()));
		}
		central_query.close();
	}
}
//---------------------------------------------------------------------------
void database::extractDataFromTank_Map(String &tank_cid,std::map<String,String> &temp)
{
	if (tank_cid.Length() > 0)
	{
		std::wstringstream sqlquery;
		sqlquery << "select storage_cid,location_cid,position,shelf_number from c_tank_map where tank_cid = '" << tank_cid.c_str() << "'";
		XQUERY central_query( m_dbCentral.get(), AnsiString(sqlquery.str().c_str()).c_str());
		throwUnless( central_query.open(), "Failed to retreve retrival lists" );
		if ( central_query.fetch() )    //any data?
		{
			std::wstringstream ss;
			ss << central_query.result.getInt(0);
			temp.insert(std::pair<String,String> (DEFINE_STORAGE_CID,ss.str().c_str()));
			ss.str( std::wstring() );
			ss.clear();
			ss << central_query.result.getInt(1);
			temp.insert(std::pair<String,String> (DEFINE_LOCATION_CID,ss.str().c_str()));
			ss.str( std::wstring() );
			ss.clear();
			ss << central_query.result.getInt(2);
			temp.insert(std::pair<String,String> (DEFINE_TANK_MAP_POSITION,ss.str().c_str()));
			ss.str( std::wstring() );
			ss.clear();
			ss << central_query.result.getInt(3);
			temp.insert(std::pair<String,String> (DEFINE_TANK_MAP_SHELF_NUMBER,ss.str().c_str()));
			ss.str( std::wstring() );
			ss.clear();
		}
		central_query.close();
	}
}
//---------------------------------------------------------------------------

void database::fillSampleNotesStatus(std::map<String,String> &cryoivalData)
{
	String sample_id = cryoivalData[DEFINE_CRYOVIAL_SPECIMEN_ID];
	String aliquot_cid = cryoivalData[DEFINE_CRYOVIAL_ALIQUOT_CID];
	String cryovialBarcode = cryoivalData[DEFINE_CRYOVIAL_BARCODE];

	String discrType = "aliquot_type";
	String source_nameColName = getDiscripColumnsFor(discrType);
	String aliquotString = getExternalNameFromC_ObjectName(aliquot_cid, m_ObjectNameList);

	bool hasResults = false;
	int resultNotes = 0;
	{
		std::wstringstream sqlquery;
		sqlquery << "select note_exists  ";
		sqlquery << " from result ";
		sqlquery << " where ";
		sqlquery << " sample_id IN (select sample_id from specimen where barcode = '" << cryovialBarcode.c_str() << "' AND ";
		sqlquery <<  source_nameColName.c_str() << "=" << "'" << aliquotString.c_str() << "')";

		XQUERY project_query( m_dbProject.get(), AnsiString(sqlquery.str().c_str()).c_str() );
		throwUnless( project_query.open(), "Failed to retreve retrival lists" );
		while ( project_query.fetch() )    //any data?
		{
			hasResults = true;
			resultNotes += project_query.result.getInt(0);
		}
		project_query.close();
	}
	if (!hasResults)
	{
		std::wstringstream sqlquery;
		sqlquery << "select note_exists  ";
		sqlquery << " from result ";
		sqlquery << " where ";
		sqlquery << " sample_id = '" << sample_id.c_str() << "'";
		std::wstring sql = sqlquery.str();
		{
			XQUERY project_query( m_dbProject.get(), AnsiString(sqlquery.str().c_str()).c_str() );
			throwUnless( project_query.open(), "Failed to retreve retrival lists" );
			while ( project_query.fetch() )    //any data?
			{
				resultNotes += project_query.result.getInt(0);
			}
			project_query.close();
		}
	}
	cryoivalData.insert(std::pair<String,String> (DEFINE_RESULTS_NOTES_COUNT,System::Sysutils::IntToStr(resultNotes).c_str()));
}

//---------------------------------------------------------------------------
void database::fillCryovialStatus(std::map<int,std::map<String,String> > &cryoLists,int *ProgessCounter,bgWorkerThreadTask *bgWorkerThreadTask)
{
	m_bBackgroundProcessing = true;

 //for getting source name
	String discrType = "source_name";
	String source_nameColName = getDiscripColumnsFor(discrType);
 //for analises
	discrType = "analysis";
	String analysisColName = getDiscripColumnsFor(discrType);
	std::vector<String> analysisStrings;
	getDiscripStringsFor(discrType, analysisStrings);

	std::map<String,String> alquotCheckedList;
	bool hasAliquotInSpecimen = false;
	*ProgessCounter = 0;
	std::map<int,std::map<String,String> > newMap;
//FILL THE REST OF THE TABLE..
	for (std::map<int,std::map<String,String> >::reverse_iterator it = cryoLists.rbegin(); it != cryoLists.rend(); ++it)
	{
		int cryoid = it->first;
		std::map<String,String> temp = it->second;
//////////////////////////////////////////////////////////////////////////////
		if (bgWorkerThreadTask->getCancellationFlag())
		{
			m_bBackgroundProcessing = false;
			(*ProgessCounter) = -1;
			return;
		}

		{
			String Box_id = temp[database::DEFINE_CRYOVIAL_BOX_ID];

			std::wstringstream sqlquery_boxname;
			std::wstringstream sqlquery_boxstore;

			sqlquery_boxname << "select box_name.external_name,box_name.note_exists from box_name where box_name.box_cid = '" << Box_id.c_str() << "'";
			sqlquery_boxstore << "select box_store.rack_cid,box_store.slot_position from box_store where box_store.box_cid = '" << Box_id.c_str() << "'";
			{
				XQUERY project_query( m_dbProject.get(), AnsiString(sqlquery_boxname.str().c_str()).c_str() );
				throwUnless( project_query.open(), "Failed to retreve retrival lists" );
				if ( project_query.fetch() )    //any data?
				{
					temp.insert(std::pair<String,String> (DEFINE_BOXNAME_EXTERNAL_NAME,project_query.result.getString(0).c_str()));
					temp.insert(std::pair<String,String> (DEFINE_BOXNAME_NOTES_EXISTS,System::Sysutils::IntToStr(project_query.result.getInt(1)).c_str()));
				}
				project_query.close();
			}
			{
				XQUERY project_query( m_dbProject.get(), AnsiString(sqlquery_boxstore.str().c_str()).c_str() );
				throwUnless( project_query.open(), "Failed to retreve retrival lists" );
				if ( project_query.fetch() )    //any data?
				{
					std::wstringstream ss;
					ss << project_query.result.getInt(0);
					temp.insert(std::pair<String,String> (DEFINE_BOXSTORE_RACK_CID,ss.str().c_str()));
					ss.str( std::wstring() );
					ss.clear();
					ss << project_query.result.getInt(1);
					temp.insert(std::pair<String,String> (DEFINE_BOXSTORE_SLOT_POSITION,ss.str().c_str()));
					ss.str( std::wstring() );
					ss.clear();
				}
				project_query.close();
			}
		}

 /////////////////////////////////////////////////////////////////////////////
		String BoxStore_id = temp[database::DEFINE_BOXSTORE_RACK_CID];
		extractDataFromRack_Number(BoxStore_id,temp);

		String tank_cid = temp[database::DEFINE_TANK_CID];
		extractDataFromTank_Map(tank_cid,temp);

		String storage_cid = temp[database::DEFINE_STORAGE_CID];
		String location_cid = temp[database::DEFINE_LOCATION_CID];
//////////////////////////////////////////////////////////////////////////////
		{
			String aliquotType_cid = temp[database::DEFINE_CRYOVIAL_ALIQUOT_CID];

			std::wstringstream sqlquery;
			sqlquery << "select object_cid,external_name from c_object_name where object_cid IN ('";
			sqlquery << storage_cid.c_str() << "','" << location_cid.c_str() << "','" << aliquotType_cid.c_str() << "','" << tank_cid.c_str() << "')";
			XQUERY central_query( m_dbCentral.get(), AnsiString(sqlquery.str().c_str()).c_str() );
			throwUnless( central_query.open(), "Failed to retreve retrival lists" );
			while ( central_query.fetch() )    //any data?
			{
				if (!storage_cid.IsEmpty())
				{
					if (central_query.result.getInt(0) == storage_cid.ToInt())
						temp.insert(std::pair<String,String> (DEFINE_STORAGE_EXTERNAL_NAME,central_query.result.getString(1).c_str()));
				}
				if (!location_cid.IsEmpty())
				{
					if (central_query.result.getInt(0) == location_cid.ToInt())
						temp.insert(std::pair<String,String> (DEFINE_LOCATION_EXTERNAL_NAME,central_query.result.getString(1).c_str()));
				}
				if (!aliquotType_cid.IsEmpty())
				{
					if (central_query.result.getInt(0) == aliquotType_cid.ToInt())
						temp.insert(std::pair<String,String> (DEFINE_ALIQUOT_EXTERNAL_NAME,central_query.result.getString(1).c_str()));
				}
				if (!tank_cid.IsEmpty())
				{
					if (central_query.result.getInt(0) == tank_cid.ToInt())
						temp.insert(std::pair<String,String> (DEFINE_TANK_EXTERNAL_NAME,central_query.result.getString(1).c_str()));
				}
			}
			central_query.close();
		}
// ADD THE HISTORY STATUS HERE!

		std::wstringstream historyNotes;
		String aliquotName = temp[database::DEFINE_ALIQUOT_EXTERNAL_NAME];
		String CryoBarCode = temp[database::DEFINE_CRYOVIAL_BARCODE];

		String discrType = "aliquot_type";
		String aliquot_nameColName = getDiscripColumnsFor(discrType);
		String aliquotString = getExternalNameFromC_ObjectName(aliquot_nameColName, m_ObjectNameList);

		bool foundWithBarCode = false;
		if (aliquotString.Length() > 0)
		{
			{
				std::wstringstream sqlquery;
				sqlquery << "select sample_id,note_exists,";
				sqlquery << source_nameColName.c_str();
				sqlquery << " from specimen where barcode = '" << CryoBarCode.c_str() << "' AND ";
				sqlquery <<  source_nameColName.c_str() << "=" << "'" << aliquotString.c_str() << "')";

				XQUERY project_query( m_dbProject.get(), AnsiString(sqlquery.str().c_str()).c_str() );
				throwUnless( project_query.open(), "Failed to retreve retrival lists" );
				historyNotes << "[ ";
				while ( project_query.fetch() )    //any data?
				{
					foundWithBarCode = true;
					int sample_id = project_query.result.getInt(0);
					std::wstringstream ss;
					ss << project_query.result.getInt(0);
					temp.insert(std::pair<String,String> (DEFINE_SPECIMEN_SAMPLE_ID,ss.str().c_str()));
					ss.str( std::wstring() ); ss.clear();
					ss << project_query.result.getInt(1);
					temp.insert(std::pair<String,String> (DEFINE_SPECIMEN_NOTES_EXISTS,ss.str().c_str()));
					String SourceNameString = project_query.result.getString(2).c_str();
					temp.insert(std::pair<String,String> (DEFINE_SAMPLE_SOURCE_NAME,SourceNameString.c_str()));
				}
				historyNotes << "] ";
				project_query.close();
			}
		}

		if (!foundWithBarCode)
		{
//try looking with sample_id (cryo.specimen_id)
			String cryospecimenid = temp[database::DEFINE_CRYOVIAL_SPECIMEN_ID];

			std::wstringstream sqlquery;
			sqlquery << "select sample_id,note_exists,";
			sqlquery << source_nameColName.c_str();
			sqlquery << " from specimen where sample_id = '" << cryospecimenid.c_str() << "'";
			XQUERY project_query( m_dbProject.get(), AnsiString(sqlquery.str().c_str()).c_str() );
			throwUnless( project_query.open(), "Failed to retreve retrival lists" );
			while ( project_query.fetch() )    //any data?
			{
				foundWithBarCode = true;
				int sample_id = project_query.result.getInt(0);
				std::wstringstream ss;
				ss << project_query.result.getInt(0);
				temp.insert(std::pair<String,String> (DEFINE_SPECIMEN_SAMPLE_ID,ss.str().c_str()));
				ss.str( std::wstring() ); ss.clear();
				ss << project_query.result.getInt(1);
				temp.insert(std::pair<String,String> (DEFINE_SPECIMEN_NOTES_EXISTS,ss.str().c_str()));
				String SourceNameString = project_query.result.getString(2).c_str();
				temp.insert(std::pair<String,String> (DEFINE_SAMPLE_SOURCE_NAME,SourceNameString.c_str()));
			}
			project_query.close();
		}
		fillSampleNotesStatus(temp);

		if ((temp[DEFINE_SPECIMEN_NOTES_EXISTS] == "0") &&
			(temp[DEFINE_BOXNAME_NOTES_EXISTS] == "0") &&
			(temp[DEFINE_CRYOVIAL_NOTES_EXISTS] == "0") &&
			(temp[DEFINE_CRYOVIAL_STORE_NOTES_EXISTS] == "0") &&
			(temp[DEFINE_RESULTS_NOTES_COUNT] == "0"))
		{
			historyNotes << "";
		}
		else
		{
			int note_types_count = 0;
			if (temp[DEFINE_SPECIMEN_NOTES_EXISTS] != "0")
				note_types_count++;
			if (temp[DEFINE_BOXNAME_NOTES_EXISTS] != "0")
				note_types_count++;
			if (temp[DEFINE_CRYOVIAL_NOTES_EXISTS] != "0")
				note_types_count++;
			if (temp[DEFINE_CRYOVIAL_STORE_NOTES_EXISTS] != "0")
				note_types_count++;
			if (temp[DEFINE_RESULTS_NOTES_COUNT] != "0")
				note_types_count++;

			if (note_types_count == 1)
			{
				if (temp[DEFINE_SPECIMEN_NOTES_EXISTS] != "0")
					historyNotes << "Specimen notes available";
				else if (temp[DEFINE_BOXNAME_NOTES_EXISTS] != "0")
					historyNotes << "Box notes available";
				else if (temp[DEFINE_CRYOVIAL_NOTES_EXISTS] != "0")
					historyNotes << "Cryovial notes available";
				else if (temp[DEFINE_CRYOVIAL_STORE_NOTES_EXISTS] != "0")
					historyNotes << "Cryovial store notes available";
				else if (temp[DEFINE_RESULTS_NOTES_COUNT] != "0")
					historyNotes << "Result notes available";
			}
			else
			{
				historyNotes << "Multiple notes available";
			}
		}

		temp[database::DEFINE_HISTORY_STATUS] = historyNotes.str().c_str();

		//erase from the reverse_iterator.
		//cryoLists.erase(cryoid);
		//cryoLists.insert(std::pair<int,std::map<String,String> > (cryoid,temp));
		newMap.insert(std::pair<int,std::map<String,String> > (cryoid,temp));
		(*ProgessCounter)++;
	}
	cryoLists = newMap;
	m_bBackgroundProcessing = false;
}

//---------------------------------------------------------------------------


void database::getCryovialLists(String &project_id,String &aliquit_id,std::vector<String> &barcodes,std::map<int,std::map<String,String> > &cryoLists)
{
	cryoLists.clear();

	//generate list of barcodes for the SQL
	std::wstringstream barcodesql;

	barcodesql << " cryovial.cryovial_barcode IN ('";
	for (unsigned int i=0; i < barcodes.size(); i++)
	{
		if (i > 0)
			barcodesql << "','";
	   barcodesql << barcodes[i].c_str();
	}
	barcodesql << "') AND cryovial.aliquot_type_cid = '" << aliquit_id.c_str() <<"' ";

	std::wstringstream sqlquery;
	sqlquery << "select ";
	sqlquery << "cryovial.cryovial_id, cryovial.cryovial_barcode,cryovial.aliquot_type_cid,cryovial.sample_id,cryovial.note_exists,char(cryovial.time_stamp),cryovial.status ";
	sqlquery << " from cryovial_store, cryovial";
	sqlquery << " where cryovial.cryovial_id = cryovial_store.cryovial_id AND ";
	sqlquery << barcodesql.str();
	std::wstring temp = sqlquery.str();

	runcryovialSQL(temp,cryoLists);
}
//---------------------------------------------------------------------------

void database::getCryovialLists(int retrieval_cid, std::map<int,std::map<String,String> > &cryoLists)
{
	cryoLists.clear();

	{
		std::wstringstream sqlquery;

		sqlquery << "select ";
		sqlquery << "cryovial.cryovial_id, cryovial.cryovial_barcode,cryovial.aliquot_type_cid,cryovial.sample_id,cryovial.note_exists,char(cryovial.time_stamp),cryovial.status ";
		sqlquery << " from cryovial,CRYOVIAL_STORE";
		sqlquery << " where CRYOVIAL_STORE.cryovial_id = cryovial.cryovial_id AND retrieval_cid = '";
		sqlquery << retrieval_cid;
		sqlquery << "'";
		std::wstring temp = sqlquery.str();

		runcryovialSQL(temp,cryoLists);
	}
	if (cryoLists.size() > 0)
		return; //we have results.

	std::wstringstream sqlquery;
	sqlquery << "select cryovial.cryovial_id, cryovial.cryovial_barcode,cryovial.aliquot_type_cid,cryovial.sample_id,cryovial.note_exists,char(cryovial.time_stamp),cryovial.status from cryovial_store,cryovial WHERE CRYOVIAL_STORE.cryovial_id = cryovial.cryovial_id AND box_cid IN (";
	sqlquery << "select box_cid from box_store where retrieval_cid = '";
	sqlquery << retrieval_cid;
	sqlquery << "')";
	std::wstring temp = sqlquery.str();
	runcryovialSQL(temp,cryoLists);
}

//---------------------------------------------------------------------------

void database::getNotesFromResult(std::map<String,String> &data,std::map<int,std::map<String,String> > &results)
{
	String sample_id = data[DEFINE_CRYOVIAL_SPECIMEN_ID];
	String aliquot_cid = data[DEFINE_CRYOVIAL_ALIQUOT_CID];
	String cryovialBarcode = data[DEFINE_CRYOVIAL_BARCODE];

	String discrType = "aliquot_type";
	String source_nameColName = getDiscripColumnsFor(discrType);
	String aliquotString = getExternalNameFromC_ObjectName(aliquot_cid, m_ObjectNameList);
	std::wstringstream sqlNoteIDs;
	bool hasResults = false;
	int resultNotes = 0;
	{
		std::wstringstream sqlquery;
		sqlquery << "select result_id from result where ";
		sqlquery << " sample_id IN (select sample_id from specimen where barcode = '" << cryovialBarcode.c_str() << "' AND ";
		sqlquery <<  source_nameColName.c_str() << "=" << "'" << aliquotString.c_str() << "')";

		XQUERY project_query( m_dbProject.get(), AnsiString(sqlquery.str().c_str()).c_str() );
		throwUnless( project_query.open(), "Failed to retreve retrival lists" );
		while ( project_query.fetch() )    //any data?
		{
			hasResults = true;
			if (!sqlNoteIDs.str().empty()) {
				sqlNoteIDs << ",";
			}
			sqlNoteIDs << project_query.result.getInt(0);
		}
		project_query.close();
	}
	if (!hasResults)
	{
		std::wstringstream sqlquery;
		sqlquery << "select result_id from result where ";
		sqlquery << " sample_id = '" << sample_id.c_str() << "'";
		std::wstring sql = sqlquery.str();
		{
			XQUERY project_query( m_dbProject.get(), AnsiString(sqlquery.str().c_str()).c_str() );
			throwUnless( project_query.open(), "Failed to retreve retrival lists" );
			while ( project_query.fetch() )    //any data?
			{
				if (!sqlNoteIDs.str().empty())
					sqlNoteIDs << ",";
				sqlNoteIDs << project_query.result.getInt(0);
			}
			project_query.close();
		}
	}
	String notes = String(sqlNoteIDs.str().c_str());
	getSampleNotes(notes,results);
}
//-----------------------------------------------------------------------------
void database::getNotesFromCryovial(std::map<String,String> &data,std::map<int,std::map<String,String> > &results)
{
	int cryovial_id = data[DEFINE_CRYOVIAL_ID].ToInt();
	getSampleNotes(cryovial_id,results);
}
//-----------------------------------------------------------------------------
void database::getNotesFromCryoStore(std::map<String,String> &data,std::map<int,std::map<String,String> > &results)
{
	int cryovialstore_id = data[DEFINE_CRYOVIAL_STORE_RECORD_ID].ToInt();
	getSampleNotes(cryovialstore_id,results);
}
//-----------------------------------------------------------------------------
void database::getNotesFromSpecimen(std::map<String,String> &data,std::map<int,std::map<String,String> > &results)
{
	int specimen_id = data[DEFINE_CRYOVIAL_SPECIMEN_ID].ToInt();
	getSampleNotes(specimen_id,results);
}
//-----------------------------------------------------------------------------
void database::getNotesFromBoxName(std::map<String,String> &data,std::map<int,std::map<String,String> > &results)
{
	int boxname_id = data[DEFINE_CRYOVIAL_BOX_ID].ToInt();
	getSampleNotes(boxname_id,results);
}
//---------------------------------------------------------------------------

void database::throwUnless( bool condition, const String& msg )
{
	if ( ! condition )
	{
		throw msg;
	}
}
//---------------------------------------------------------------------------

void onError( const std::string& msg )
{
	throw String(msg.c_str());
}
//---------------------------------------------------------------------------

bool dbErrorCallback( const std::string object, const int instance,const int ecount, const int ecode, const std::string error_txt )
{
	onError( error_txt );
	return( true );
}


#ifndef __database__HPP_
#define __database__HPP_

#include "xdb.h"
#include "xquery.h"


#include <Vcl.ComCtrls.hpp>


#include <vector>
#include <map>

void onError( const std::string& msg );
bool dbErrorCallback( const std::string object, const int instance,const int ecount, const int ecode, const std::string error_txt );
class bgWorkerThreadTask;

class database
{
public:
	static const String DEFINE_RETRIEVAL_CID;
	static const String DEFINE_RETRIEVAL_DISCRIPTION; // from c_retrieval_job
	static const String DEFINE_PROJECT_CID;          // from c_retrieval_job

	static const String DEFINE_DB_NAME;    //database name from c_project
	static const String DEFINE_PROJECT_EXTERNAL_NAME;    //c_project
	static const String DEFINE_PROJECT_EXTERNAL_FULL;    //c_project

	static const String DEFINE_CRYOVIAL_ID;
	static const String DEFINE_CRYOVIAL_BOX_ID;
	static const String DEFINE_CRYOVIAL_POSITION;
	static const String DEFINE_CRYOVIAL_BARCODE;
	static const String DEFINE_CRYOVIAL_SPECIMEN_ID;   //FK of specimen
	static const String DEFINE_CRYOVIAL_ALIQUOT_CID;
	static const String DEFINE_CRYOVIAL_TIMESTAMP;
	static const String DEFINE_CRYOVIAL_STATUS;

	static const String DEFINE_SAMPLE_SOURCE_NAME;

	static const String DEFINE_TANK_MAP_POSITION;
	static const String DEFINE_TANK_MAP_SHELF_NUMBER;

	static const String DEFINE_STORAGE_CID;
	static const String DEFINE_LOCATION_CID;

	static const String DEFINE_BOXSTORE_RACK_CID;
	static const String DEFINE_BOXSTORE_SLOT_POSITION;
	static const String DEFINE_BOXNAME_EXTERNAL_NAME;

	static const String DEFINE_RESULT_ID;
	static const String DEFINE_RESULT_TIME_STAMP;
	static const String DEFINE_RESULT_STRING_VALUE;
	static const String DEFINE_RESULT_NOTE_EXISTS;
	static const String DEFINE_RESULT_TEST_CID;
	static const String DEFINE_CRYOVIAL_STORE_BOXID;
	static const String DEFINE_CRYOVIAL_STORE_RECORD_ID;
	static const String DEFINE_CRYOVIAL_STORE_STATUS_TEXT;
	static const String DEFINE_CRYOVIAL_STORE_STATUS;
	static const String DEFINE_CRYOVIAL_RETRIEVAL_CID;

	static const String DEFINE_RACK_EXTERNAL_NAME;
	static const String DEFINE_RACK_POSITION;

	static const String DEFINE_OBJECT_NAME_CID;
	static const String DEFINE_OBJECT_NAME_EXTERNAL_NAME;
	static const String DEFINE_OBJECT_NAME_EXTERNAL_FULL;
	static const String DEFINE_STORAGE_EXTERNAL_NAME;
	static const String DEFINE_LOCATION_EXTERNAL_NAME;
	static const String DEFINE_ALIQUOT_EXTERNAL_NAME;
	static const String DEFINE_TANK_EXTERNAL_NAME;
	static const String DEFINE_TANK_CID;
	static const String DEFINE_NOTE_ID;
	static const String DEFINE_NOTE_TIME_STAMP;
	static const String DEFINE_NOTE_CONTENTS;
	static const String DEFINE_HISTORY_STATUS;
	static const String DEFINE_SPECIMEN_SAMPLE_ID;
	static const String DEFINE_CRYOVIAL_STORE_DATETIME;
	static const String DEFINE_CRYOVIAL_STORE_REMOVED;
	static const String DEFINE_BOX_STORE_DATETIME;
	static const String DEFINE_BOX_STORE_REMOVED;

	static const String DEFINE_BOXNAME_NOTES_EXISTS;
	static const String DEFINE_CRYOVIAL_NOTES_EXISTS;
	static const String DEFINE_CRYOVIAL_STORE_NOTES_EXISTS;
	static const String DEFINE_RESULTS_NOTES_COUNT;
	static const String DEFINE_SPECIMEN_NOTES_EXISTS;
public:
	database();
	~database();

//setup the database connections
	void connect(String &selectDB);
	void connectProject(String &projectName);
//query database
	void getRetrivalLists(std::map<int,std::map<String,String> > &newRetrivalLists, int exercise_cid);
	void getRetrivalLists(std::map<int,std::map<String,String> > &newRetrivalLists);
	void getExerciseLists(std::map<int,std::map<String,String> > &exerciseLists);

	void getProjectLists(std::map<int,std::map<String,String> > &ProjectLists);
	void getProject(int project_cid,std::map<String,String> &data);

	void getCryovialLists(int retrieval_cid, std::map<int,std::map<String,String> > &cryoLists);
	void getCryovialLists(String &project_id,String &aliquit_id,std::vector<String> &barcodes,std::map<int,std::map<String,String> > &cryoLists);
	void getCryovialLists(String &project_id,std::vector<String> &sample_barcodes,std::map<int,std::map<String,String> > &cryoLists);
	void getCryovialListsWithCryovialid(String &Cryovialid,std::map<int,std::map<String,String> > &cryoLists);

	void fillRichEditInfo(std::map<String,String> &cryovialInfo,TRichEdit *RichEdit);

	void getSampleResult(String &sample_id,std::map<int,std::map<String,String> > &results,std::map<int,String>  &testnameResults);
	void getSampleResult(String &cryovialBarcode,String &aliquot_cid,String &sample_cid,std::map<int,std::map<String,String> > &results,std::map<int,String>  &testnameResults);
	void getSampleResultForSameSource(String &sample_id,std::map<int,std::map<String,String> > &results,std::map<int,String>  &testnameResults);

	void fillCryovialStatus(std::map<int,std::map<String,String> > &cryoLists,int *ProgessCounter,bgWorkerThreadTask *bgWorkerThreadTask);

	void getAliquotLists(std::map<int,std::map<String,String> > &AliquotLists);

	void getCryovialListsWithSampleID(String &selectedCryovialSampleID,std::map<int,std::map<String,String> > &cryoLists);
	void getCryovialListsWithBarcodeAndAliquot(String &CryoBarCode,String &Alquot_cid,std::map<int,std::map<String,String> > &cryoLists);
	void getCryovialListsWithBarCode(String &CryoBarCode,std::map<int,std::map<String,String> > &cryoLists);
	void getCryovialListsWithSrcName(String &srcName,std::map<int,std::map<String,String> > &cryoLists);

	bool isRunningInBackgroundThread() {return m_bBackgroundProcessing;}
	void setRunningInBackgroundThead(bool running) {m_bBackgroundProcessing = running;}

	void getNotesFromResult(std::map<String,String> &data,std::map<int,std::map<String,String> > &results);
	void getNotesFromCryovial(std::map<String,String> &data,std::map<int,std::map<String,String> > &results);
	void getNotesFromCryoStore(std::map<String,String> &data,std::map<int,std::map<String,String> > &results);
	void getNotesFromSpecimen(std::map<String,String> &data,std::map<int,std::map<String,String> > &results);
	void getNotesFromBoxName(std::map<String,String> &data,std::map<int,std::map<String,String> > &results);

	void getSampleNotes(int note_id,std::map<int,std::map<String,String> > &results);
    bool isBackgroundProcessing() {return m_bBackgroundProcessing;}
private:
	void toReadableDateString(String &dateString);

	String retrieval_job_JobTypeToString(int iStatus);
	String retrieval_job_StatusToString(int iStatus);
	String cryovial_StatusToString(int iStatus);
	String cryovial_store_StatusToString(int iStatus);

	void getSampleNotes(String &sql_note_id_list,std::map<int,std::map<String,String> > &results);

	void outputRetrieval_jobInfo(String &retrieval_cid,TRichEdit *RichEdit);

	void fillSampleNotesStatus(std::map<String,String> &cryoivalData);

	void extractDataFromTank_Map(String &tank_cid,std::map<String,String> &temp);
	void extractDataFromRack_Number(String &boxStore_id,std::map<String,String> &temp);

	void runSampleResultSQL(std::wstring &sql,std::map<int,std::map<String,String> > &results,std::map<int,String> &runSampleResultSQL);
	bool hasAlquotTypeInSpecimenDescr(String &AliquotName);
	void runcryovialSQL(std::wstring &sqlQuery,std::map<int,std::map<String,String> > &results);
//	void runCryovalHistorySql(std::wstring &sql,std::map<int,std::map<String,String> > &results);
	String getDiscripColumnsFor(String &type);
	void getDiscripStringsFor(String &type, std::vector<String> &stringList);

	std::auto_ptr<XDB> m_dbCentral;
	std::auto_ptr<XDB> m_dbProject;
	void throwUnless( bool condition, const String& msg );

	bool m_bBackgroundProcessing;

	String getExternalNameFromC_ObjectName(String &object_cid, std::map<String,String> &List);
	std::map<String,String> m_ObjectNameList;
};

#endif

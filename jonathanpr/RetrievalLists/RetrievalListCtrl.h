//---------------------------------------------------------------------------

#ifndef RetrievalListCtrlH
#define RetrievalListCtrlH
//---------------------------------------------------------------------------

#include "RetrievalListDatabase.h"

class bgWorkerThreadTask;

class retrievalList
{
public:
	retrievalList();
	~retrievalList();

	void initalise(String &selectedBD);
	void connectToProject(String &databaseName);
	bool hasSelectedDB() {return !m_selectedDB.IsEmpty();}

	bool isDatabaseRunningInBackgroundThread();

	void getExerciseLists(std::map<int,std::map<String,String> > &exerciseLists);
	void getProjectLists(std::map<int,std::map<String,String> > &ProjectLists);
	void getAliquotLists(std::map<int,std::map<String,String> > &AliquotLists);

	void getRetrivalLists(std::map<int,std::map<String,String> > &newRetrivalLists, int exercise_cid);
	void getRetrivalLists(std::map<int,std::map<String,String> > &newRetrivalLists);
	void getProject(int project_cid,std::map<String,String> &data);

	void getCryovialLists(int retrieval_cid, std::map<int,std::map<String,String> > &cryoLists);
	void getCryovialLists(String &project_id,String &aliquit_id,std::vector<String> &barcodes,std::map<int,std::map<String,String> > &cryoLists);
	void getCryovialLists(String &project_id,std::vector<String> &sample_barcodes,std::map<int,std::map<String,String> > &cryoLists);

	void getCryovialListsForSameSource(String &selectedCryovialSampleID,std::map<int,std::map<String,String> > &cryoLists,std::map<int,String>  &testnameResults);
	void getCryovialListsWithSampleID(String &selectedCryovialSampleID,std::map<int,std::map<String,String> > &cryoLists);
	void getCryovialListsWithBarcodeAndAliquot(String &CryoBarCode,String &Alquot_cid,std::map<int,std::map<String,String> > &cryoLists);
	void getCryovialListsWithBarCode(String &CryoBarCode,std::map<int,std::map<String,String> > &cryoLists);
	void getCryovialListsWithSrcName(String &srcName,std::map<int,std::map<String,String> > &cryoLists);
	void getCryovialListsWithCryovialid(String &Cryovialid,std::map<int,std::map<String,String> > &cryoLists);

	void fillCryovialStatus(std::map<int,std::map<String,String> > &cryoLists,int *ProgessCounter,bgWorkerThreadTask *bgWorkerThreadTask);

	void getSampleResult(String &barcode, String &aliquot_type,String &sample_cid,std::map<int,std::map<String,String> > &results,std::map<int,String>  &testnameResults);
	void getSampleResult(String &sample_id,std::map<int,std::map<String,String> > &results,std::map<int,String>  &testnameResults);

	void getSampleNotes(int sampleID,std::map<int,std::map<String,String> > &results);

	void fillRichEditInfo(std::map<String,String> &cryovialInfo,TRichEdit *RichEdit);

	void getNotesFromResult(std::map<String,String> &data,std::map<int,std::map<String,String> > &results);
	void getNotesFromCryovial(std::map<String,String> &data,std::map<int,std::map<String,String> > &results);
	void getNotesFromCryoStore(std::map<String,String> &data,std::map<int,std::map<String,String> > &results);
	void getNotesFromSpecimen(std::map<String,String> &data,std::map<int,std::map<String,String> > &results);
	void getNotesFromBoxName(std::map<String,String> &data,std::map<int,std::map<String,String> > &results);
	bool isBackgroundProcessing();

private:
	RetrievalListDatabase m_database;
	String m_selectedDB;
};

#endif

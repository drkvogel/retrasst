//---------------------------------------------------------------------------

#pragma hdrstop

#include "RetrievalListCtrl.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)


retrievalList::retrievalList()
{

}
//---------------------------------------------------------------------------

retrievalList::~retrievalList()
{

}
//---------------------------------------------------------------------------

void retrievalList::initalise(String &selectedDB)
{
	m_selectedDB = selectedDB;
	m_database.connect(m_selectedDB);
}
//---------------------------------------------------------------------------
bool retrievalList::isBackgroundProcessing()
{
	return m_database.isRunningInBackgroundThread();
}

//---------------------------------------------------------------------------

bool retrievalList::isDatabaseRunningInBackgroundThread()
{
	return m_database.isRunningInBackgroundThread();
}
//---------------------------------------------------------------------------

void retrievalList::fillCryovialStatus(std::map<int,std::map<String,String> > &cryoLists,int *ProgessCounter,bgWorkerThreadTask *bgWorkerThreadTask)
{
	m_database.fillCryovialStatus(cryoLists,ProgessCounter,bgWorkerThreadTask);
}
//---------------------------------------------------------------------------


void retrievalList::connectToProject(String &databaseName)
{
	m_database.connectProject(databaseName);
}

//---------------------------------------------------------------------------

void retrievalList::getExerciseLists(std::map<int,std::map<String,String> > &exerciseLists)
{
   m_database.getExerciseLists(exerciseLists);
}

//---------------------------------------------------------------------------

void retrievalList::getRetrivalLists(std::map<int,std::map<String,String> > &newRetrivalLists)
{
   m_database.getRetrivalLists(newRetrivalLists);
}

//---------------------------------------------------------------------------

void retrievalList::getRetrivalLists(std::map<int,std::map<String,String> > &newRetrivalLists, int exercise_cid)
{
   m_database.getRetrivalLists(newRetrivalLists, exercise_cid);
}
//---------------------------------------------------------------------------

void retrievalList::getAliquotLists(std::map<int,std::map<String,String> > &AliquotLists)
{
   m_database.getAliquotLists(AliquotLists);
}
//---------------------------------------------------------------------------

void retrievalList::getProjectLists(std::map<int,std::map<String,String> > &ProjectLists)
{
   m_database.getProjectLists(ProjectLists);
}
//---------------------------------------------------------------------------

void retrievalList::getProject(int project_cid,std::map<String,String> &data)
{
	m_database.getProject(project_cid,data);
}
//---------------------------------------------------------------------------

void retrievalList::getCryovialListsWithCryovialid(String &Cryovialid,std::map<int,std::map<String,String> > &cryoLists)
{
	m_database.getCryovialListsWithCryovialid(Cryovialid,cryoLists);
}
//---------------------------------------------------------------------------

void retrievalList::getCryovialLists(String &project_id,std::vector<String> &sample_barcodes,std::map<int,std::map<String,String> > &cryoLists)
{
	m_database.getCryovialLists(project_id,sample_barcodes,cryoLists);
}

//---------------------------------------------------------------------------

void retrievalList::getCryovialLists(String &project_id,String &aliquit_id,std::vector<String> &barcodes,std::map<int,std::map<String,String> > &cryoLists)
{
	m_database.getCryovialLists(project_id,aliquit_id,barcodes,cryoLists);
}

//---------------------------------------------------------------------------

void retrievalList::getCryovialLists(int retrieval_cid, std::map<int,std::map<String,String> > &cryoLists)
{
	m_database.getCryovialLists(retrieval_cid,cryoLists);
}
//---------------------------------------------------------------------------

void retrievalList::getSampleResult(String &barcode, String &aliquot_type,String &sample_cid,std::map<int,std::map<String,String> > &results,std::map<int,String>  &testnameResults)
{
   m_database.getSampleResult(barcode,aliquot_type,sample_cid,results,testnameResults);
}
//---------------------------------------------------------------------------

void retrievalList::getSampleResult(String &sample_id,std::map<int,std::map<String,String> > &results,std::map<int,String>  &testnameResults)
{
   m_database.getSampleResult(sample_id,results,testnameResults);
}
//---------------------------------------------------------------------------

void retrievalList::fillRichEditInfo(std::map<String,String> &cryovialInfo,TRichEdit *RichEdit)
{
   m_database.fillRichEditInfo(cryovialInfo,RichEdit);
}

//-----------------------------------------------------------------------------

void retrievalList::getSampleNotes(int sampleID,std::map<int,std::map<String,String> >  &results)
{
	m_database.getSampleNotes(sampleID,results);
}
//-----------------------------------------------------------------------------

void retrievalList::getCryovialListsWithSampleID(String &selectedCryovialSampleID,std::map<int,std::map<String,String> > &cryoLists)
{
	m_database.getCryovialListsWithSampleID(selectedCryovialSampleID,cryoLists);
}

//-----------------------------------------------------------------------------

void retrievalList::getCryovialListsWithBarcodeAndAliquot(String &CryoBarCode,String &Alquot_cid,std::map<int,std::map<String,String> > &cryoLists)
{
	m_database.getCryovialListsWithBarcodeAndAliquot(CryoBarCode,Alquot_cid,cryoLists);
}

//-----------------------------------------------------------------------------

void retrievalList::getCryovialListsWithBarCode(String &CryoBarCode,std::map<int,std::map<String,String> > &cryoLists)
{
	m_database.getCryovialListsWithBarCode(CryoBarCode,cryoLists);
}

//-----------------------------------------------------------------------------

void retrievalList::getCryovialListsWithSrcName(String &srcName,std::map<int,std::map<String,String> > &cryoLists)
{
	m_database.getCryovialListsWithSrcName(srcName,cryoLists);
}
//-----------------------------------------------------------------------------

void retrievalList::getCryovialListsForSameSource(String &selectedCryovialSampleID,std::map<int,std::map<String,String> > &cryoLists,std::map<int,String>  &testnameResults)
{
	m_database.getSampleResultForSameSource(selectedCryovialSampleID,cryoLists,testnameResults);
}

//-----------------------------------------------------------------------------

void retrievalList::getNotesFromResult(std::map<String,String> &data,std::map<int,std::map<String,String> > &results)
{
	m_database.getNotesFromResult(data,results);
}
//-----------------------------------------------------------------------------
void retrievalList::getNotesFromCryovial(std::map<String,String> &data,std::map<int,std::map<String,String> > &results)
{
	m_database.getNotesFromCryovial(data,results);
}
//-----------------------------------------------------------------------------
void retrievalList::getNotesFromCryoStore(std::map<String,String> &data,std::map<int,std::map<String,String> > &results)
{
	m_database.getNotesFromCryoStore(data,results);
}
//-----------------------------------------------------------------------------
void retrievalList::getNotesFromSpecimen(std::map<String,String> &data,std::map<int,std::map<String,String> > &results)
{
	m_database.getNotesFromSpecimen(data,results);
}
//-----------------------------------------------------------------------------
void retrievalList::getNotesFromBoxName(std::map<String,String> &data,std::map<int,std::map<String,String> > &results)
{
	m_database.getNotesFromBoxName(data,results);
}
//-----------------------------------------------------------------------------


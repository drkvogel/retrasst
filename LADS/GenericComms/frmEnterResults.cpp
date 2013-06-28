//---------------------------------------------------------------------------
#include <vcl.h>
#pragma hdrstop
#include "frmEnterResults.h"
#include "LQuery.h"
#include "BuddyLogin.h"
#include "LCDbAnalyser.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"
TformEnterResults *formEnterResults;
//---------------------------------------------------------------------------
__fastcall TformEnterResults::TformEnterResults(TComponent* Owner)
	: TForm(Owner)
{
	lines = new TStringList;
}

void __fastcall TformEnterResults::FormShow(TObject *Sender)
{
	// populate dropdown with output formats assigned to the current analyser
	comboFormat->Items->Clear();
	listboxInfo->Clear();
	lines->Clear();
	LQuery qc(frmBuddyLogin->qCentral);
	qc.setSQL("SELECT"
		" ft.filetype_cid,"
		" ft.name,"
		" ft.description,"
		" ft.type,"
		" ft.firstrow,"
		" ft.delim,"
		" ft.subdelim,"
		" ft.time_stamp"
		" FROM g_comms_filetype ft, g_comms_map map"
		" WHERE ft.filetype_cid = map.filetype_cid"
		" AND map.cluster_cid = :mch "
		" AND ft.status = 0 AND map.status = 0"
		" AND ft.type = :typ");
	qc.setParam("mch", LCDbAnalysers::getCurrentID()); // this machine
	qc.setParam("typ", OUTPUT_FORMAT);
	qc.open();
	while (!qc.eof()) {
		FileFormat * ff = new FileFormat(
			qc.readInt("filetype_cid"), qc.readString("name"), qc.readString("description"),
			(FormatType)qc.readInt("type"), qc.readInt("firstrow"), qc.readString("delim"),
			qc.readString("subdelim"));
		comboFormat->Items->AddObject(ff->name, (TObject *)ff);
		qc.next();
	}
	if (comboFormat->Items->Count > 0) {
		comboFormat->ItemIndex = 0; //???
	}

	if (MYDEBUG) {
		editFilename->Text = "C:\\Users\\cbird\\git\\lims-generic\\dat\\results-menarini-structured.csv";
		lines->LoadFromFile(editFilename->Text);
//		comboDelim->ItemIndex = 1;
//		comboDelimChange(NULL);
	}
}
//---------------------------------------------------------------------------
void __fastcall TformEnterResults::btnBrowseClick(TObject *Sender)
{
	if (openDialogChooseFile->Execute()) {
		editFilename->Text = openDialogChooseFile->FileName;
		lines->LoadFromFile(editFilename->Text);
	}
}

void __fastcall TformEnterResults::btnOKClick(TObject *Sender)
{
	// TODO
	pResult result = new Result;
	VecpResult results;

	if (NULL == lines) {
		Application->MessageBox("No file loaded", "Error", MB_OK);
		return;
	}

	// parse
//	int 		patientID;
//	String		barcode;
//	TDateTime	startDate;
//	String		sampleType;
//	int			testID;
//	String		unit;
//	String		result;
//	TDateTime	dateAnalysed;
//	String		cuvetteType;
//	int			reagentID;
//	String		reagentLot;
//	String		notes;

	// following code copied from EditFormat - needs adapting or merging
	LQuery qc(frmBuddyLogin->qCentral);
	qc.setSQL("SELECT"
		" field_cid,"
		" filetype_cid,"
		" tag,"
		" tagcol,"
		" valcol,"
		" subcol,"
		" type,"
		" pos"
		" FROM g_comms_field WHERE id_filetype = :idfiletype"
		" AND status = 0"
		" ORDER BY pos"); // XXX
	qc.setParam("idfiletype", format->id);
	int count = qc.open();
	if (!MYDEBUG) {
		assert(count > 1);
	}
	VecpDatum vecpDatum;
	while (!qc.eof()) {
		Datum *newdatum = new Datum;
		newdatum->datumType     = (Datum::DatumType)qc.readInt("type");
		newdatum->id            = qc.readInt("field_cid");
		newdatum->id_filetype   = qc.readInt("filetype_cid");
		//newdatum->name          = qc.readString("name");
		newdatum->tag           = qc.readString("tag");
		newdatum->tagCol        = qc.readInt("tagcol");
		newdatum->valCol        = qc.readInt("valcol");
		newdatum->valSubCol     = qc.readInt("subcol");
		newdatum->pos     		= qc.readInt("pos");
		vecpDatum.push_back(newdatum);
		qc.next();
	}

//	TTreeNode * rootNode = NULL;
//	TTreeNode * resultNode = NULL;
//	TTreeNode * reagentNode = NULL;
	Datum * curdatum = new Datum;
	for (VecpDatum::const_iterator it = vecpDatum.begin(); it != vecpDatum.end(); it++) {
		// save the current node if it is a container node
		Datum * datum = *it;
		switch (datum->datumType) {
			case Datum::DATUM_ROOT:
				//rootNode = curNode;
				break;
			case Datum::DATUM_RESULT:
				//resultNode = curNode;
				break;
			case Datum::DATUM_REAGENT:
				//reagentNode = curNode;
				break;
			case Datum::DATUM_PID:
			case Datum::DATUM_BARCODE:
			case Datum::DATUM_STARTDATE:
			case Datum::DATUM_SAMPLETYPE:
			case Datum::DATUM_TID:
			case Datum::DATUM_UNIT:
			case Datum::DATUM_RESULTSTRING:
			case Datum::DATUM_DATEANALYSED:
			case Datum::DATUM_CUVETTE:
			case Datum::DATUM_REAGENTID:
			case Datum::DATUM_REAGENTLOT:
			case Datum::DATUM_NOTES:
				// do nothing
				break;
			default:
				throw Exception("Invalid datum type");
		}

		// and add next it to appropriate saved node
		curdatum = *it;
		switch (curdatum->datumType) {
			case Datum::DATUM_ROOT:
				throw "DATUM_ROOT should not be stored in database";
			case Datum::DATUM_PID:
			case Datum::DATUM_BARCODE:
			case Datum::DATUM_STARTDATE:
			case Datum::DATUM_SAMPLETYPE:
			case Datum::DATUM_RESULT:
			case Datum::DATUM_NOTES:
				 //curNode = addDatum(curdatum, rootNode);
				break;
			case Datum::DATUM_TID:
			case Datum::DATUM_UNIT:
			case Datum::DATUM_RESULTSTRING:
			case Datum::DATUM_DATEANALYSED:
			case Datum::DATUM_CUVETTE:
			case Datum::DATUM_REAGENT:
				//curNode = addDatum(curdatum, resultNode);
				break;
			case Datum::DATUM_REAGENTID:
			case Datum::DATUM_REAGENTLOT:
				//curNode = addDatum(curdatum, reagentNode);
				break;
			default:
				throw Exception("Invalid datum type");
		}
	} // for vecpDatum

	// display the records
	for (VecpResult::const_iterator it = results.begin(); it != results.end(); it++) {
		result = *it;
		if (!cbDummyRun->Checked) {
			// actually store the records
		}

		if (cbGenerate->Checked) {
			// generate a CSV file
		}
	}

	// clean up
	delete_referenced< VecpResult > (results);
}

void __fastcall TformEnterResults::comboFormatChange(TObject *Sender)
{
	parseFile();
}

void __fastcall TformEnterResults::btnProcessClick(TObject *Sender)
{
	parseFile();	
}

//---------------------------------------------------------------------------
void TformEnterResults::parseFile() {
	// process file with selected format
	listboxInfo->Clear();
	if (lines->Text.IsEmpty()) {
		//editExample->Text = "NO FILE LOADED";
		listboxInfo->Items->Add("NO FILE LOADED");
		return;
	}
	listboxInfo->Items->Add("Load file: "+editFilename->Text);

	memoPreview->Clear();
	for (int i=0; i < lines->Count; i++) {
		memoPreview->Lines->Add(lines->Strings[i]);
	}
}


//---------------------------------------------------------------------------


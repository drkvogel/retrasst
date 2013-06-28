//---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop

#include "frmManage.h"
#include "frmMain.h"
#include "LCDbAnalyser.h"
#include "LCDbObject.h"
#include "TfrmLoginA.h"
#include "frmEditFormat.h"
#include "BuddyLogin.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"

TformManage *formManage;

//---------------------------------------------------------------------------
__fastcall TformManage::TformManage(TComponent* Owner)
    : TForm(Owner) {
}

void __fastcall TformManage::FormCreate(TObject *Sender)
{
	groupboxInputFormats->Color  = genericComms::formatColour[genericComms::INPUT_FORMAT]; //(inputFormat == genericComms::INPUT_FORMAT) ?
	groupboxOutputFormats->Color = genericComms::formatColour[genericComms::OUTPUT_FORMAT]; //(inputFormat == genericComms::INPUT_FORMAT) ?
//	panelInputActions->Color  = genericComms::formatColour[genericComms::INPUT_FORMAT];
//	Color = genericComms::formatColour[genericComms::INPUT_FORMAT];
// colours of controls seem to be inherited from form?
}

void __fastcall TformManage::FormShow(TObject *Sender) {
	groupboxInputFormats->Color  = genericComms::formatColour[genericComms::INPUT_FORMAT]; //(inputFormat == genericComms::INPUT_FORMAT) ?
	groupboxOutputFormats->Color = genericComms::formatColour[genericComms::OUTPUT_FORMAT]; //(inputFormat == genericComms::INPUT_FORMAT) ?
    refreshAll();
}
//---------------------------------------------------------------------------
void __fastcall TformManage::btnOutputRemoveClick(TObject *Sender) {
	fnDeassignFormat(OUTPUT_FORMAT);
}

void __fastcall TformManage::btnInputAddClick(TObject *Sender) {
	fnAssignFormat(INPUT_FORMAT);
}

void __fastcall TformManage::btnInputRemoveClick(TObject *Sender) {
	fnDeassignFormat(INPUT_FORMAT);	
}

void __fastcall TformManage::btnInputNewClick(TObject *Sender) {
	fnNewFormat(INPUT_FORMAT);	
}

void __fastcall TformManage::btnInputEditClick(TObject *Sender) {
	fnEditFormat(INPUT_FORMAT);	
}

void __fastcall TformManage::btnInputDelClick(TObject *Sender) {
	fnDelFormat(INPUT_FORMAT);	
}

void __fastcall TformManage::listboxInputAvailableDblClick(TObject *Sender) {
	fnEditFormat(INPUT_FORMAT);
}

void __fastcall TformManage::listboxAnalysersClick(TObject *Sender) {
	refreshOutputAssigned();
	refreshInputAssigned();
}

void __fastcall TformManage::btnOutputNewClick(TObject *Sender) {
	fnNewFormat(OUTPUT_FORMAT);
}

void __fastcall TformManage::listboxOutputAvailableDblClick(TObject *Sender) {
	fnEditFormat(OUTPUT_FORMAT);
}

void __fastcall TformManage::btnOutputEditClick(TObject *Sender) {
	fnEditFormat(OUTPUT_FORMAT);
}

void __fastcall TformManage::btnOutputDelClick(TObject *Sender) {
	fnDelFormat(OUTPUT_FORMAT);
}

void __fastcall TformManage::btnOutputAddClick(TObject *Sender) {
	fnAssignFormat(OUTPUT_FORMAT);
}
//---------------------------------------------------------------------------
void TformManage::refreshAnalysers() {
//    LCDbAnalysers & analysers = LCDbAnalysers::records();
//	analysers.read(frmAnalyserLogin->qCentral, false);
//    analysers.
	listboxAnalysers->Clear();
	for (Range< LCDbObject > ml = LCDbObjects::records(); ml.isValid(); ++ ml) {
		if (ml->isActive() && ml->getObjectType() == LCDbObject::CLUSTER) {
            listboxAnalysers->Items->AddObject(ml->getName(), (TObject *)ml->getID());
		}
	}
	for (Range< LCDbAnalyser > ml = LCDbAnalysers::records(); ml.isValid(); ++ ml) {
		if (ml->isActive()) {
			listboxAnalysers->Items->AddObject(ml->getName(), (TObject *)ml->getID());
		}
//		if (ml->getID() == LCDbAnalysers::getCurrentID()) {
//			listboxAnalysers->Text = ml->getName();
//		}
	}
}

void TformManage::refreshInputAssigned() {
	refreshAssigned(INPUT_FORMAT);
}

void TformManage::refreshOutputAssigned() {
	refreshAssigned(OUTPUT_FORMAT);
}

void TformManage::refreshAssigned(FormatType t) {
	TListBox * lb = (t == OUTPUT_FORMAT ? listboxOutputAssigned : listboxInputAssigned);
    String q;
	for (int i=0; i < listboxAnalysers->Items->Count; i++) {
		if (listboxAnalysers->Selected[i]) {
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
				" FROM g_comms_filetype ft, g_comms_map mp"
				" WHERE ft.filetype_cid = mp.filetype_cid AND mp.cluster_cid = :ftid"
				" AND ft.type = :ftyp");
			qc.setParam("ftid", (int)(listboxAnalysers->Items->Objects[i]));
			qc.setParam("ftyp", t);
			try {
				qc.open();
			} catch (Exception &e) {
				tmp = "Exception: " + e.Message;
				//OutputDebugString(tmp.c_str());
				throw (tmp.c_str()); // always throw
			}

			lb->Clear();
			while (!qc.eof()) {
				FileFormat *ff = new FileFormat(
					qc.readInt("filetype_cid"), qc.readString("name"), qc.readString("description"),
					(FormatType)qc.readInt("type"), qc.readInt("firstrow"), qc.readString("delim"),
					qc.readString("subdelim"));
				lb->Items->AddObject(ff->name, (TObject *)ff);
				qc.next();
			}
        }
    }
}

void TformManage::refreshInputAvailable() {
	refreshAvailable(INPUT_FORMAT);
}

void TformManage::refreshOutputAvailable() {
	refreshAvailable(OUTPUT_FORMAT);
}

void TformManage::refreshAvailable(genericComms::FormatType t) {
	TListBox * lb = (t == OUTPUT_FORMAT ? listboxOutputAvailable : listboxInputAvailable);
	lb->Clear();
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
		" FROM g_comms_filetype ft"
		" WHERE status = 0 AND type = :ftyp");
	qc.setParam("ftyp", t);
	try {
		qc.open();
	} catch (Exception &e) {
		// sometimes fails with "row count: -1" - but can still fetch rows as normal
		// caused by long varchar type or varchar(250+)
		// should report exception though as database should not contain these types
		// and we should report any other errors
		tmp = "Exception: " + e.Message;
		//OutputDebugString(tmp.c_str());
		throw Exception(tmp.c_str());
	}
	while (!qc.eof()) {
		FileFormat *ff = new FileFormat(
			qc.readInt("filetype_cid"), qc.readString("name"), qc.readString("description"),
			(FormatType)qc.readInt("type"), qc.readInt("firstrow"), qc.readString("delim"),
			qc.readString("subdelim"));
		lb->Items->AddObject(ff->name, (TObject *)ff);
		qc.next();
	}
}

void TformManage::refreshAll() {
	refreshAnalysers();
	refreshOutputAssigned();
	refreshInputAssigned();
	refreshOutputAvailable();
	refreshInputAvailable();
}

void TformManage::fnNewFormat(FormatType t) {
	formEditFormat->setFormatType(t);
	formEditFormat->clearFields();
	formEditFormat->ShowModal();
	refreshAll();
}

void TformManage::fnEditFormat(FormatType t) {
	TListBox * lb = (t == OUTPUT_FORMAT ? listboxOutputAvailable : listboxInputAvailable);
	for (int i=0; i < lb->Items->Count; i++) {
		if (lb->Selected[i]) {
			formEditFormat->setFormat((FileFormat *)lb->Items->Objects[i]);
			formEditFormat->ShowModal();
			refreshAll();
			return;
		}
	}
	refreshAll();
}

void TformManage::fnDelFormat(FormatType t) {
	TListBox * lb = (t == OUTPUT_FORMAT ? listboxOutputAvailable : listboxInputAvailable);
	Application->MessageBox("TODO - should check this format not assigned to an analyser", "TODO", MB_OK);
	// delete format
	for (int i=0; i < lb->Items->Count; i++) {
		if (lb->Selected[i]) {
			String msg;
			FileFormat * ff = (FileFormat *)lb->Items->Objects[i];
			msg.sprintf("Are you sure you want to delete format '%s'?", ff->name.c_str());
			if (CONFIRMATIONS_OFF || IDYES == Application->MessageBoxA(msg.c_str(), "Confirmation", MB_YESNO)) {
				LQuery qc(frmBuddyLogin->qCentral);
				qc.setSQL("DELETE FROM g_comms_filetype WHERE filetype_cid = :fid");
				qc.setParam("fid", ff->id);
				qc.execSQL();
				refreshAll();
				tmp.sprintf("Format '%s' deleted", ff->name.c_str());
				StatusBar1->SimpleText = tmp;
				return;
			}
		}
	}
}

void TformManage::fnAssignFormat(FormatType formatType) {
	TListBox * lb = (formatType == OUTPUT_FORMAT ? listboxOutputAvailable : listboxInputAvailable);
	// assign format
	bool oneSelected = false; // multiselect should be disabled
	for (int analyser=0; analyser < listboxAnalysers->Items->Count; analyser++) {
		if (listboxAnalysers->Selected[analyser]) {
			for (int format=0; format < lb->Items->Count; format++) {
				if (lb->Selected[format]) {
					oneSelected = true;
					FileFormat * ff = (FileFormat *)lb->Items->Objects[format];
					tmp.sprintf("Are you sure you want to assign the format '%s' to this analyser?", ff->name.c_str());
					if (CONFIRMATIONS_OFF || IDYES == Application->MessageBoxA(tmp.c_str(), "Confirmation", MB_YESNO)) {
						// check not existing
						LQuery qc(frmBuddyLogin->qCentral);
						qc.setSQL("SELECT COUNT(*) FROM g_comms_map WHERE filetype_cid = :ft AND cluster_cid = :idc");
						qc.setParam("ft", ff->id);
						qc.setParam("idc", (int)(listboxAnalysers->Items->Objects[analyser]));
						qc.open();
						if (qc.eof())
							throw "database error - no results for count";
						if (qc.readInt(0) > 0) {
							Application->MessageBoxA("Format already assigned!", "Info", MB_OK);
							// throw "Format name already exists!";
							return;
						}
						// get_next_cid???
						qc.setSQL( "SELECT next_cid from c_next_cid" );
						qc.open();
						int next_cid = qc.readInt(0); //???
						//return (query.readInt( 0 ) == expected ? id : 0);
						qc.setSQL("INSERT INTO g_comms_map (filetype_cid, cluster_cid, record_cid) VALUES (:ft, :idc, :rid)");
						qc.setParam("ft", ff->id);
						qc.setParam("idc", (int)(listboxAnalysers->Items->Objects[analyser]));
						//qc.setParam("rid", next_cid);
						qc.setParam("rid", 0);
						qc.execSQL();
					}
				}
			}
		}
	}
	if (!oneSelected) {
		Application->MessageBox("An analyser and a format to assign to it must be selected!", "Error", MB_OK);
		return;
	}
	refreshAssigned(formatType);
}

void TformManage::fnDeassignFormat(FormatType formatType) {
	TListBox * lb = (formatType == OUTPUT_FORMAT ? listboxOutputAssigned : listboxInputAssigned);
	// deassign format
	String q;
	for (int i=0; i < listboxAnalysers->Items->Count; i++) {
		if (listboxAnalysers->Selected[i]) {
			for (int j=0; j < lb->Items->Count; j++) {
				if (lb->Selected[j]) {
					FileFormat * ff = (FileFormat *)lb->Items->Objects[j];
					tmp.sprintf("Are you sure you want to deassign the format '%s' from this analyser?", ff->name.c_str());
					if (CONFIRMATIONS_OFF || IDYES == Application->MessageBoxA(tmp.c_str(), "Confirmation", MB_YESNO)) {
                    	LQuery qc(frmBuddyLogin->qCentral);
						qc.setSQL("DELETE FROM g_comms_map WHERE filetype_cid = :ft AND cluster_cid = :idc"),
						qc.setParam("ft", ff->id);
						qc.setParam("idc", (int)(listboxAnalysers->Items->Objects[i]));
						qc.execSQL();
					}
				}
			}
		}
	}
	refreshAssigned(formatType);
}
//---------------------------------------------------------------------------

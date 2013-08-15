#include <vcl.h>
#pragma hdrstop
#include "RetrievalAssistantProcess.h"
#pragma package(smart_init)
#pragma resource "*.dfm"
TfrmProcess *frmProcess;

__fastcall TfrmProcess::TfrmProcess(TComponent* Owner) : TForm(Owner) { }

void __fastcall TfrmProcess::FormCreate(TObject *Sender) {
//    sgRetrieval->ColCount = SGRETRIEVAL_NUMCOLS;
//    for (int i=0; i<SGRETRIEVAL_NUMCOLS; i++) {
//        sgRetrieval->Cells[i][0]    = sgRetrievalColName[i];
//        sgRetrieval->ColWidths[i]   = sgRetrievalColWidth[i];
//    }
}

void __fastcall TfrmProcess::FormShow(TObject *Sender) {
    //
}

void loadRows() {
/*
/* SELECT
   cs.Cryovial_id, cs.Note_Exists, cs.retrieval_cid, cs.box_cid, cs.status, cs.cryovial_position,
   cryovial_barcode, t.external_name AS aliquot, b.external_name AS box,
   s.external_name AS site, m.position, v.external_full AS vessel,
   shelf_number, r.external_name AS rack, bs.slot_position
 FROM
   cryovial c, cryovial_store cs, box_name b, box_store bs, c_rack_number r,
   c_tank_map m, c_object_name s,
   c_object_name v,
   c_object_name t
 WHERE
   c.cryovial_id = cs.cryovial_id AND
   b.box_cid = cs.box_cid AND
   b.box_cid = bs.box_cid AND
   bs.status = 6 AND
   t.object_cid = aliquot_type_cid AND
   bs.rack_cid = r.rack_cid AND
   r.tank_cid = m.tank_cid AND
   s.object_cid = location_cid AND
   v.object_cid = storage_cid AND
   cs.retrieval_cid = -1015 */

}

void showRows() {
//    if (vials.size() <= 0) {
//        clearSG(sgVials);
//    } else {
//        sgVials->RowCount = (-1 == maxRows) ? vials.size() + 1 : maxRows + 1;
//        sgVials->FixedRows = 1;
//    }
//    int row = 1;
//    vecpSampleRow::const_iterator it;
//    for (it = vials.begin(); it != vials.end(); it++, row++) {
//        pSampleRow sampleRow = *it;
//        LPDbCryovialStore * vial = sampleRow->store_record;
//        sgVials->Cells[SGVIALS_BARCODE][row]    = sampleRow->cryovial_barcode.c_str();
//        sgVials->Cells[SGVIALS_DESTBOX][row]    = "tba"; //sampleRow->;
//        sgVials->Cells[SGVIALS_DESTPOS][row]    = "tba"; //sampleRow->;
//        sgVials->Cells[SGVIALS_CURRBOX][row]    = sampleRow->box_name.c_str();
//        ostringstream loc;
//        loc << sampleRow->site_name << " " << sampleRow->position << " ["<< sampleRow->shelf_number<<"]:"
//        << sampleRow->vessel_name<<" [layout?], "<<sampleRow->rack_name<< " slot "<<sampleRow->slot_position;
//        sgVials->Cells[SGVIALS_LOCATION][row]   = loc.str().c_str();
//
//        sgVials->Objects[0][row] = (TObject *)sampleRow;
//        if (-1 != maxRows && row >= maxRows) break;
//    }
//    ostringstream oss; oss<<(-1 == maxRows) ? maxRows : vials.size()<<" of "<<vials.size()<<" vials";
//    groupVials->Caption = oss.str().c_str();
}

/*
 * Work through list or sub-section by giving the storage location and sample ID of each sample on the list in the order saved above (REQ 8.3.8);
 * As each sample is retrieved its barcode should be scanned, if the scanned barcode matches that on the list
the destination location should be displayed and the next ID/location should be displayed (REQ 8.3.9)
 * if the ID’s do not match a warning should be displayed and re-entry of the barcode required (REQ 8.3.10).
 * When working through the list the previous five successfully entered ID’s should always be visible (REQ 8.3.11).
 * The option to exit the process saving progress should be offered, with an “are you sure?” message in case of accidental selection (REQ 8.3.12).
*/

void __fastcall TfrmProcess::Exit1Click(TObject *Sender) {
    if (IDYES == Application->MessageBox(L"Are you sure you want to exit?\n\nCurrent progress will be saved.", L"Question", MB_YESNO)) {
        // save stuff
        Close();
    }
}


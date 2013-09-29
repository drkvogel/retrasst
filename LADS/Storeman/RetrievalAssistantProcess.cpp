#include <vcl.h>
#pragma hdrstop
#include "RetrievalAssistantProcess.h"
#pragma package(smart_init)
#pragma resource "*.dfm"
TfrmProcess *frmProcess;

__fastcall TfrmProcess::TfrmProcess(TComponent* Owner) : TForm(Owner) { 
/* enum {  SGRETRIEVAL_BARCODE, SGRETRIEVAL_DESTBOX, SGRETRIEVAL_DESTPOS, SGRETRIEVAL_CURRBOX, SGRETRIEVAL_CURRPOS,
        SGRETRIEVAL_SITE, SGRETRIEVAL_POSITION, SGRETRIEVAL_SHELF, SGRETRIEVAL_VESSEL, SGRETRIEVAL_STRUCTURE, SGRETRIEVAL_SLOT, // location in "Russian Doll order"
        SGRETRIEVAL_NUMCOLS};
static const char * sgRetrievalColName[SGRETRIEVAL_NUMCOLS] = {
    "Barcode", "Dest box", "Pos", "Curr box", "Pos", "Site", "Position", "Shelf", "Vessel", "Structure", "Slot"
};
static int sgRetrievalColWidth[SGRETRIEVAL_NUMCOLS] = {102, 147, 43, 275, 37, 64, 50, 43, 100, 121, 40 }; */
    sgwChunks = new StringGridWrapper< Chunk< SampleRow > >(sgChunks, &chunks);
    sgwChunks->addCol("section",  "Section",  200);
    sgwChunks->addCol("start",    "Start",    200);
    sgwChunks->addCol("end",      "End",      200);
    sgwChunks->addCol("size",     "Size",     200);
    sgwChunks->init();

    sgwVials = new StringGridWrapper<SampleRow>(sgRetrieval, &vials);
    sgwVials->addCol("barcode",  "Barcode",          102,   SampleRow::sort_asc_barcode);
    sgwVials->addCol("aliquot",  "Aliquot",          100,   SampleRow::sort_asc_aliquot);
    sgwVials->addCol("currbox",  "Current box",      275,   SampleRow::sort_asc_currbox);
    sgwVials->addCol("currpos",  "Pos",              43,    SampleRow::sort_asc_currpos);
    sgwVials->addCol("site",     "Site",             116,   SampleRow::sort_asc_site);
    sgwVials->addCol("vesspos",  "Position",         50,    SampleRow::sort_asc_position);
    sgwVials->addCol("shelf",    "Shelf",            100,   SampleRow::sort_asc_shelf);
    sgwVials->addCol("vessel",   "Vessel",           43,    SampleRow::sort_asc_vessel);
    sgwVials->addCol("struct",   "Structure",        121,   SampleRow::sort_asc_structure);
    sgwVials->addCol("boxpos",   "Pos",              40,    SampleRow::sort_asc_slot);
    sgwVials->addCol("destbox",  "Destination box",  213,   SampleRow::sort_asc_destbox);
    sgwVials->addCol("destpos",  "Pos",              37,    SampleRow::sort_asc_destpos);
    sgwVials->init();
}

void __fastcall TfrmProcess::FormCreate(TObject *Sender) {
    cbLog->Visible      = RETRASSTDEBUG;
    maxRows             = DEFAULT_NUMROWS;
    job                 = NULL;
    loadingMessage = "Loading retrieval list, please wait...";
}

void __fastcall TfrmProcess::FormShow(TObject *Sender) {
    loadRows();
    showRows();
    panelLoading->Caption = loadingMessage;
}

void __fastcall TfrmProcess::cbLogClick(TObject *Sender) {
    memoDebug->Visible = cbLog->Checked;
}

void __fastcall TfrmProcess::menuItemExitClick(TObject *Sender) {
    if (IDYES == Application->MessageBox(L"Are you sure you want to exit?\n\nCurrent progress will be saved.", L"Question", MB_YESNO)) {
        // save stuff
        Close();
    }
}

void TfrmProcess::loadRows() {
/*
For a box retrieval, the retrieval plan will be given by
Select * from c_box_retrieval b order by b.section, b.rj_box_cid

For a cryovial retrieval, the retrieval plan will be:
Select * from c_box_retrieval b, l_cryovial_retrieval c where b.rj_box_cid = c.rj_box_cid order by b.section, c.position
*/
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

void TfrmProcess::showRows() {
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
//        sgVials->Objects[0][row] = (TObject *)sampleRow;
//        if (-1 != maxRows && row >= maxRows) break;
//    }
//    ostringstream oss; oss<<(-1 == maxRows) ? maxRows : vials.size()<<" of "<<vials.size()<<" vials";
//    groupVials->Caption = oss.str().c_str();
        // sgVials->Cells[sgwVials->colNameToInt("destpos")] [row] = sampleRow->dest_cryo_pos;
        // sgVials->Objects[0][row] = (TObject *)sampleRow;
}

void TfrmProcess::process() {
/*
 * Work through list or sub-section by giving the storage location and sample ID of each sample on the list in the order saved above (REQ 8.3.8);
 * As each sample is retrieved its barcode should be scanned, if the scanned barcode matches that on the list
the destination location should be displayed and the next ID/location should be displayed (REQ 8.3.9)
 * if the ID’s do not match a warning should be displayed and re-entry of the barcode required (REQ 8.3.10).
 * When working through the list the previous five successfully entered ID’s should always be visible (REQ 8.3.11).
 * The option to exit the process saving progress should be offered, with an “are you sure?” message in case of accidental selection (REQ 8.3.12).

 destination box+position, cryovial barcode and current box+position+structure+location of the primary and secondary aliquots.
*/

}





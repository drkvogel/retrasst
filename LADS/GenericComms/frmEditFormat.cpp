//---------------------------------------------------------------------------
#include <vcl.h>
#pragma hdrstop
#include <sstream>
#include "frmEditFormat.h"
#include "frmMain.h"
#include "BuddyLogin.h"
#include "gcommsObjects.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"
TformEditFormat *formEditFormat;
//---------------------------------------------------------------------------
__fastcall TformEditFormat::TformEditFormat(TComponent* Owner)
	: TForm(Owner)
{
}

void __fastcall TformEditFormat::FormCreate(TObject *Sender)
{
	int i;

	// datum types
	//for (i=0; i < Datum::DATUM_COUNT; i++) {
	comboDataType->Items->Clear(); // cos examples loaded in form already
	for (i=1; i < Datum::DATUM_COUNT; i++) { // start at 1 to miss out root
	//for (i=0; i<Datum::numDatumIdentifiers; i++) {
		TMenuItem * menuItem = popupMenuActions->CreateMenuItem();
		menuItem->Tag = i; // store DatumType that this menu item is relevant to
		menuItem->Caption = Datum::datumInfo[i].description;
		menuItem->ImageIndex = Datum::datumInfo[i].imageIndex; // menuItem->SelectedIndex =
		menuItem->OnClick = menuItemClick;
		popupMenuActions->Items->Add(menuItem);
		comboDataType->Items->AddObject(
			Datum::datumInfo[i].description,
			(TObject *)Datum::datumInfo[i].type);
	}

	// other params
	for (i=0; i<numStrDelims; i++) {comboDelim->Items->Add(strDelims[i][0]);}
	for (i=0; i<numStrSubDelims; i++) {comboSubDelim->Items->Add(strSubDelims[i][0]);}
	for (i=0; i<numStrEscapeChars; i++) {comboEscape->Items->Add(strEscapeChars[i][0]);}
	for (i=0; i<numStrQuoteChars; i++) {comboQuote->Items->Add(strQuoteChars[i][0]);}
	setDefaults();
	lines = new TStringList;
	setFormatType(genericComms::OUTPUT_FORMAT);
}

void __fastcall TformEditFormat::FormShow(TObject *Sender) {
	loadItems();
	btnAdd->Enabled = false;
	btnRemove->Enabled = false;
	btnSave->Enabled = false;
	timerKey->Enabled = false;
	if (MYDEBUG) {
		editFile->Text = "C:\\Users\\cbird\\git\\lims-generic\\dat\\results-menarini-structured.csv";
		lines->LoadFromFile(editFile->Text);
		comboDelim->ItemIndex = 1;
		comboDelimChange(NULL);
	}
	treeviewParse->FullExpand();
}

void __fastcall TformEditFormat::FormClose(TObject *Sender, TCloseAction &Action) {
	format = NULL;
}

void __fastcall TformEditFormat::FormDestroy(TObject *Sender)
{
	delete lines;
}
//------ Event handlers ---------------------------------------------------------------------
void __fastcall TformEditFormat::btnRemoveClick(TObject *Sender)
{
	//
}

void __fastcall TformEditFormat::comboSubDelimChange(TObject *Sender)
{
    editSubDelim->Text = strSubDelims[comboSubDelim->ItemIndex][1];
    if (0 == strcmp(strSubDelims[comboSubDelim->ItemIndex][0], "Custom"))
        editSubDelim->Enabled = true;
	else
        editSubDelim->Enabled = false;
}

void __fastcall TformEditFormat::btnBrowseClick(TObject *Sender)
{
	if (openDialogChooseFile->Execute()) {
		editFile->Text = openDialogChooseFile->FileName;
		lines->LoadFromFile(editFile->Text);
	}
}

void __fastcall TformEditFormat::comboDelimChange(TObject *Sender)
{
	editDelim->Text = strDelims[comboDelim->ItemIndex][1];
	if (0 == strcmp(strDelims[comboDelim->ItemIndex][0], "Custom")) {
		editDelim->Enabled = true;
	} else {
		editDelim->Enabled = false;
	}
	if (0 == strcmp(strDelims[comboDelim->ItemIndex][0], "Fixed width")) {
		//editValLength->Enabled = true;
	} else {
		//editValLength->Enabled = false; // sub-columns could be fixed width, e.g. datetime parts,
			// even if major columns are delimited - so don't disable
	}
}

void __fastcall TformEditFormat::editValSubColChange(TObject *Sender)
{
	attributeChanged();
}

void __fastcall TformEditFormat::editTagChange(TObject *Sender)
{
	attributeChanged();
}

void __fastcall TformEditFormat::editTagColChange(TObject *Sender)
{
	attributeChanged();
}

void __fastcall TformEditFormat::editValColChange(TObject *Sender)
{
	attributeChanged();
}

void __fastcall TformEditFormat::comboEscapeChange(TObject *Sender)
{
	editEscape->Text = strEscapeChars[comboEscape->ItemIndex][1];
	if (0 == strcmp(strEscapeChars[comboEscape->ItemIndex][0], "Custom"))
		editEscape->Enabled = true;
	else
		editEscape->Enabled = false;
}

void __fastcall TformEditFormat::btnCancelClick(TObject *Sender)
{
	Close();
}

void __fastcall TformEditFormat::checkBoxCheckClick(TObject *Sender)
{
	if (checkBoxCheck->Checked) {
		btnBrowse->Enabled = true;
	} else {
		btnBrowse->Enabled = false;
		editExample->Text = "";
	}
}

void __fastcall TformEditFormat::comboDataTypeChange(TObject *Sender)
{
	OutputDebugString("TODO reset form or load from current fields");
}

void __fastcall TformEditFormat::timerKeyTimer(TObject *Sender)
{
	// are the current settings obviously not correct?
	// TODO could be more safely compared, convert to int and pass to findExample() as params
	if (editTagCol->Text == editValCol->Text) {
      	return;  
	}
	findExample();
	timerKey->Enabled = false; // reset
}

void __fastcall TformEditFormat::editValLengthChange(TObject *Sender)
{
	// TODO
	if (editValLength->Text.ToInt() > 0) {
		// fixed-length field
	}
	attributeChanged();
}
void __fastcall TformEditFormat::menuItemClick(TObject *Sender)
{
	TMenuItem * clicked = dynamic_cast< TMenuItem * >(Sender);
	itemAdd(Sender, (Datum::DatumType)(clicked->Tag));
}

void __fastcall TformEditFormat::menuItemDeleteClick(TObject *Sender)
{
	tmp.sprintf("Delete clicked on a '%s', TTreeNode: '%s'", String(Sender->ClassName()).c_str(), treeviewParse->Selected->Text.c_str());
	OutputDebugString(tmp.c_str());

	Datum::DatumType selectedType = ((Datum *)(treeviewParse->Selected->Data))->datumType;
	if (selectedType == Datum::DATUM_ROOT) {
		Application->MessageBoxA("Cannot delete root node", "Info", MB_OK);
		return;
	}
	if (treeviewParse->Selected->HasChildren) {
		Application->MessageBoxA("Selected node has child nodes, please delete these first", "Info", MB_OK);
		return;
	}
	treeviewParse->Selected->Delete();
}

void __fastcall TformEditFormat::btnAddClick(TObject *Sender)
{
	itemAdd(Sender, (Datum::DatumType)(comboDataType->Items->Objects[comboDataType->ItemIndex]));
}

void __fastcall TformEditFormat::comboQuoteChange(TObject *Sender)
{
	editQuote->Text = strQuoteChars[comboQuote->ItemIndex][1];
	if (0 == strcmp(strQuoteChars[comboQuote->ItemIndex][0], "Custom"))
		editQuote->Enabled = true;
	else
		editQuote->Enabled = false;
}

void __fastcall TformEditFormat::treeviewParseClick(TObject *Sender)
{
	Datum * datum = (Datum *)(treeviewParse->Selected->Data);
	if (NULL == datum) {
		throw "null datum";
	}
	tmp.sprintf("node text: '%s', type %d", treeviewParse->Selected->Text.c_str(), datum->datumType);
	OutputDebugString(tmp.c_str()); // String(Sender->ClassName()).c_str() //DatumType selectedType = (DatumType)treeviewParse->Selected->Data;
	// TODO load params into controls
//	datum->datumType
//	datum->id
//	datum->id_filetype
//	datum->tag
//	datum->tagCol
//	datum->valCol
//	datum->valSubCol
//	datum->pos

	comboDataType->ItemIndex = datum->datumType - 1; //???

	if (datum->isContainer()) {
		editTag->Text		= "";
		editTagCol->Text    = "0";
		editValCol->Text    = "0";
		editValSubCol->Text = "0";
		editValLength->Text = "0";
		editOffset->Text    = "0";
		editExample->Text   = "0";
	} else {
		editTag->Text		= datum->tag;
		editTagCol->Text	= datum->tagCol;
		editValCol->Text	= datum->valCol;
		editValSubCol->Text = datum->valSubCol;
		editValLength->Text	= datum->length;
		editOffset->Text	= datum->offset;
		// editExample->Text;
		btnSave->Enabled = false;
		findExample();
	}

//		comboDelim->ItemIndex = 0;
//		comboDelimChange(NULL);
//		comboSubDelim->ItemIndex = 0;
//		comboSubDelimChange(NULL);
//		comboEscape->ItemIndex = 0;
//		comboEscapeChange(NULL);
//		comboQuote->ItemIndex = 0;
//		comboQuoteChange(NULL);

	if (NULL != treeviewParse->Selected) {
		btnRemove->Enabled = true;
	}
}

void __fastcall TformEditFormat::treeviewParseDragDrop(TObject *Sender,
	  TObject *Source, int X, int Y)
{
	TTreeNode * target = treeviewParse->GetNodeAt(X,Y);
	THitTests ht = treeviewParse->GetHitTestInfoAt(X, Y);
	TNodeAttachMode attachMode;
	attachMode = naInsert;
	// TODO - kind of works, but could be better
//	if (ht.Contains(htOnItem) || ht.Contains(htOnIcon)) {
//		attachMode = naAddChild;
//	} else if (ht.Contains(htNowhere)) {
//		attachMode = naAdd;
//	} else if (ht.Contains(htOnIndent)) {
//		attachMode = naInsert;
//	} else {
//		return;
//	}
	treeviewParse->Selected->MoveTo(target, attachMode);
}

void __fastcall TformEditFormat::treeviewParseGetSelectedIndex(TObject *Sender,
	  TTreeNode *Node)
{
	Node->SelectedIndex = Node->ImageIndex; // make sure image doesn't change on select
}

void __fastcall TformEditFormat::treeviewParseDragOver(TObject *Sender,
	  TObject *Source, int X, int Y, TDragState State, bool &Accept)
{
	Accept = (Source == Sender);
}

void __fastcall TformEditFormat::treeviewParseCompare(TObject *Sender,
	  TTreeNode *Node1, TTreeNode *Node2, int Data, int &Compare)
{
 	// for sorting - which we don't want to do, therefore redundant
	Datum * datum1 = static_cast< Datum * >(Node1->Data);
	Datum * datum2 = static_cast< Datum * >(Node2->Data);
	Compare = Datum::datumInfo[datum1->datumType].order - Datum::datumInfo[datum2->datumType].order;
}

void __fastcall TformEditFormat::btnOKClick(TObject *Sender) {
	try {
		validateAndSave();
	} catch (Exception & e) {
		Application->MessageBoxA(e.Message.c_str(), "Info", MB_OK);
	}
}

//------General Functions---------------------------------------------------------------------
void TformEditFormat::setFormatType(FormatType formatType) {
	inputOrOutput = formatType;
	Color = genericComms::formatColour[inputOrOutput]; //(inputFormat == genericComms::INPUT_FORMAT) ? clRed : 0x00FFC0E0;
}

/** initialise file format editing screen with existing format */
void TformEditFormat::setFormat(FileFormat * ff) {
    format = ff;
    if (NULL == ff) {
        throw "null file format!";
    }
	editName->Text = format->name;
	memoDesc->Text = format->description;
	editDelim->Text = (String)format->delim;
	setFormatType(format->type);
}

void TformEditFormat::clearFields() {
    editName->Clear();
	memoDesc->Clear();
}

void TformEditFormat::setDefaults()
{
	comboDataType->ItemIndex = 0;
	comboDelim->ItemIndex = 0;
	comboDelimChange(NULL);
	comboSubDelim->ItemIndex = 0;
	comboSubDelimChange(NULL);
	comboEscape->ItemIndex = 0;
	comboEscapeChange(NULL);
	comboQuote->ItemIndex = 0;
	comboQuoteChange(NULL);
}

/** Check entries on form are correct and save to DB if OK */
void TformEditFormat::validateAndSave() {
	String tmp;
	LQuery qc(frmBuddyLogin->qCentral);

//	if (NULL != format && IDOK != Application->MessageBox("Replace format?", "Question", MB_OKCANCEL)) {
//    	return;
//	}

	// validate
	if (editName->Text.IsEmpty()) throw Exception("Name cannot be empty");
	if (editName->Text.IsEmpty()) throw Exception("Name cannot be empty");
	if (editDelim->Text.IsEmpty()) throw Exception("Delimiter cannot be empty");
	if (editSubDelim->Text.IsEmpty()) throw Exception("Sub-delimiter cannot be empty");
	if (editEscape->Text.IsEmpty()) throw Exception("Escape character cannot be empty");
	//xxxif (memoDesc->Text.IsEmpty()) throw Exception("Please add a description");

	if (NULL == format) { // new format
		qc.setSQL("SELECT COUNT(*) FROM g_comms_filetype WHERE name = :nm");
		tmp = editName->Text;
		qc.setParam("nm", tmp);
		qc.open();
		if (0 < qc.readInt(0)) {  // check not existing
			String msg = "There is already a format with the name '"+tmp+"', please choose another name";
			throw Exception(msg);
			return;
		}
        static LCDbID myLCDbID;
	    myLCDbID.claimNextID(qc);
        format = new FileFormat;
		format->id = myLCDbID.getID();
		qc.setSQL("INSERT INTO g_comms_filetype"
			" (filetype_cid, name, description, type, firstrow, delim, subdelim)"
			" VALUES (:id, :name, :descrip, :type, :firstrow, :delim, :subdelim)");
		tmp.sprintf("Format '%s' added", editName->Text.c_str());
	} else { // update existing
		qc.setSQL(
			"UPDATE g_comms_filetype"
			" SET name = :name, description = :descrip, type = :type, firstrow = :firstrow,"
			" delim = :delim, subdelim = :subdelim"
			" WHERE filetype_cid = :id");
		tmp.sprintf("Format '%s' updated", editName->Text.c_str());
	}
	qc.setParam("id", format->id);
	qc.setParam("name", editName->Text);
	qc.setParam("descrip", memoDesc->Text);
	qc.setParam("type", inputOrOutput);
	qc.setParam("firstrow", editRowBegin->Text);
	qc.setParam("delim", editDelim->Text);
	qc.setParam("subdelim", editSubDelim->Text);
	qc.execSQL();
	formManage->StatusBar1->SimpleText = tmp;
	//saveFields(format->id); // no need to separate saving of fields

	// wipe existing fields
	qc.setSQL("DELETE FROM g_comms_field WHERE filetype_cid = :id");
	qc.setParam("id", format->id);
	qc.execSQL();

	// read items from treeview - start at one to skip root node
	for (int i=1, pos =0; i < treeviewParse->Items->Count; i++, pos++) {
		static LCDbID myLCDbID;
		myLCDbID.claimNextID(qc);
//		TTreeNode * treeNode = treeviewParse->Items->Item[i];
//		Datum * datum = (Datum *)treeNode->Data;
		Datum * datum = (Datum *)treeviewParse->Items->Item[i]->Data;
		int id = myLCDbID.getID();
		tmp.sprintf("id: %d, format/filetype %d, tag %s, tagcol %d, valcol %d, subcol %d, type %d, pos %d",
			id, format->id, datum->tag.c_str(), datum->tagCol, datum->valCol, datum->valSubCol, datum->datumType, pos);
    	OutputDebugString(tmp.c_str());
		qc.setSQL(
			"INSERT INTO g_comms_field"
			" (field_cid, filetype_cid, parent_cid, "
			" datum_type, tag, tagcol, valcol, subcol, offset, length, pos)"
			" VALUES (:id, :id_filetype, :id_parent,"
			" :type, :tag, :tagcol, :valcol, :subcol, :offset, :length, :pos)");
		qc.setParam("id", id);
		qc.setParam("id_filetype", format->id); // XXX should rename filetype to format
		qc.setParam("id_parent", 0); //XXX TODO
		qc.setParam("type", datum->datumType);
		qc.setParam("tag", datum->tag);
		qc.setParam("tagcol",datum->tagCol );
		qc.setParam("valcol", datum->valCol);
		qc.setParam("subcol", datum->valSubCol);
		qc.setParam("offset", datum->offset);
		qc.setParam("length", datum->length);
		qc.setParam("pos", pos);
		qc.execSQL();

		if (datum->datumType == Datum::DATUM_DAY) {
			tmp.sprintf("datum pointer: %x", datum);
			OutputDebugString(tmp.c_str());
		}
	}
	tmp.sprintf("Format '%s' added", editName->Text.c_str());
	formManage->StatusBar1->SimpleText = tmp;
	Close();
}

void TformEditFormat::saveFields(int filetype) {
	String tmp;
	LQuery qc(frmBuddyLogin->qCentral);
}

void TformEditFormat::attributeChanged() {
	btnSave->Enabled = true;
	btnAdd->Enabled = true;
	btnRemove->Enabled = true;
	if (checkBoxCheck->Checked) {
		timerKey->Enabled = false; // XXX resets timer? yes
		timerKey->Enabled = true;
	}
}

void TformEditFormat::findExample() {
	//OutputDebugString("TODO find example of tag if check enabled");

	// OO style
//	FileFormat * format = new FileFormat(
//		//int _id,
//		"name", //String _name,
//		"desc", //String _desc,
//		?, //FormatType _type,
//		?, //int _firstrow,
//		editDelim->Text.c_str(), //String _delim,
//		editSubDelim->Text.c_str(), //String _subdelim, int _status)  ;
//
	// end OO style
	string ret;
	//if (editFile->Text.IsEmpty()) {
	if (lines->Text.IsEmpty()) {
		editExample->Text = "NO FILE LOADED";
		return;
	}
	string  escape(editEscape->Text.c_str()),
			delim(editDelim->Text.c_str()),
			subdelim(editSubDelim->Text.c_str()),
			quote(editQuote->Text.c_str()),
			tag(editTag->Text.c_str());
	int tagCol 		= editTagCol->Text.ToInt()-1, // -1 as controls are 1-indexed
		valCol 		= editValCol->Text.ToInt()-1,
		valSubCol 	= editValSubCol->Text.ToInt()-1,
		valLength	= editValLength->Text.ToInt(),
		valOffset 	= editOffset->Text.ToInt();
	escaped_list_separator<char> els(escape[0], delim[0], quote[0]);
	for (int i=0; i<lines->Count; i++) {
		mytokenizer tokenizr(string(lines->Strings[i].c_str()), els);
		if (findMatch(  ret,
						tokenizr,
						tag,
						tagCol,
						valCol,
						valSubCol,
						valLength,
						valOffset,
						escape,
						delim,
						subdelim,
						quote)) {
			editExample->Text = ret.c_str();
			return;
		}
	}
	editExample->Text = "[not found]";
}

bool TformEditFormat::findMatch(
	string & ret,
	mytokenizer tokenizr,
	const string tag,
	const int tagCol,
	const int valCol,
	const int valSubCol,
	const int length,
	const int offset,
	const string escape,
	const string delim,
	const string subdelim,
	const string quote)
{
	// TODO a lot of this boilerplate should move to before the main loop for efficiency
	// in fact, the tokenizer could be passed in
	String tmp;
	int count1 = 0;
	bool tagMatched = false;
	try {
		/** tokenizer throws an 'escape sequence not recognised' exception at this point
			on the second iteration, where the token is '^~\&'
			'\&' is not recognised as an escape sequence, so boost is telling me my input is malformed.
			Thing is, IIRC, this field is where the escape character and other special characters are
			defined, it's not an actual instance of the escape character being used.
			Can I tell boost::tokenizer to ingore this somehow? I haven't seen a way so far.
			Options:
			 1) Trawl the web for a way to configure boost::tokenizer to allow for this case
			 2) Forget boost and roll my own tokenizer
			 3) Hack the tokenizer code to ignore this 'error' (comment out exception?)
				is_escape() should return true perhaps?
			 2012-09-28 I hacked it. */
		for(mytokenizer::iterator it=tokenizr.begin(); it!=tokenizr.end(); ++it) {
			string tok(*it);
			if (valSubCol <= 0 && count1 == tagCol && 0 == tok.compare(tag)) {
				// we're at the column the tag is supposed to be, and the current token matches the tag
				tagMatched = true;
			} else if (count1 == valCol) { // we're at the column the value is at
				if (tagMatched) { // and we matched the tag already so are on the right line
					if (valSubCol > 0) { // handle sub tokens
						escaped_list_separator<char> els(escape[0], subdelim[0], quote[0]);
						mytokenizer subtokenizr(tok, els);
						return findMatch(ret, subtokenizr,
							"",
							0, 			// don't need no tag this time
							valSubCol,  // is now the col
							0, 			// no sub-sub-col
							length,
							offset,
							escape,
							subdelim, 	// is now the delim
							"",			// no sub-sub-delim
							quote);
					} else {
						if (length > 0) {
							// tok.length()
							try {
								ret = tok.substr(offset, length);
                            } catch(...) {
                                ret = "[invalid]";
                            }
						} else {
							ret = tok;
                        }
						return true;
					}
				} else {
					return false; //XXX should have matched tag before value - limitation/feature
				}
			}
			count1++;
		}
	} catch (boost::escaped_list_error & e) {
		String tmp;
        tmp.sprintf("escaped_list_error: '%s'", e.what());
		Application->MessageBoxA(tmp.c_str(), "Error", MB_OK);
    } catch (std::exception & e) {
        String tmp;
        tmp.sprintf("General Error: %s", e.what());
        Application->MessageBoxA(tmp.c_str(), "Error", MB_OK);
    }
    return false;
}

/** load field definitions from db into vector and refresh stringgrid
 */
void TformEditFormat::loadItems() {

	// create root datum
	treeviewParse->Items->Clear();
	Datum * curdatum = new Datum;
	curdatum->datumType	 = Datum::DATUM_ROOT;
	curdatum->id_filetype   = -1;
	tmp = Datum::datumInfo[Datum::DATUM_ROOT].description;
	TTreeNode * curNode = treeviewParse->Items->AddObject(NULL, tmp, (TObject *)curdatum);

	if (NULL == format) {
		// new format
		return;
	}
	LQuery qc(frmBuddyLogin->qCentral);
	qc.setSQL("SELECT"
		" field_cid,"
		" filetype_cid,"
		" parent_cid,"
		" datum_type,"
		" tag,"
		" tagcol,"
		" valcol,"
		" subcol,"
		" offset,"
		" length,"
		" pos"
		" FROM g_comms_field WHERE filetype_cid = :idfiletype"
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
		//XXX offset and length weren't being loaded - this is why you should use constructor methods
		//rather than updating individual members
		newdatum->datumType     = (Datum::DatumType)qc.readInt("datum_type");
		newdatum->id            = qc.readInt("field_cid");
		newdatum->id_filetype   = qc.readInt("filetype_cid");
		newdatum->tag           = qc.readString("tag");
		newdatum->tagCol        = qc.readInt("tagcol");
		newdatum->valCol        = qc.readInt("valcol");
		newdatum->valSubCol     = qc.readInt("subcol");
		newdatum->offset     	= qc.readInt("offset");
		newdatum->length     	= qc.readInt("length");
		newdatum->pos     		= qc.readInt("pos");
		vecpDatum.push_back(newdatum);
		qc.next();
	}

	TTreeNode * rootNode = NULL, * resultNode = NULL, * reagentNode = NULL, * dateNode = NULL;
	for (VecpDatum::const_iterator it = vecpDatum.begin(); it != vecpDatum.end(); it++) {
		// save the current node if it is a container node
		switch (((Datum *)curNode->Data)->datumType) {
			case Datum::DATUM_ROOT:
				rootNode = curNode;
				break;
			case Datum::DATUM_RESULT:
				 resultNode = curNode;
				break;
			case Datum::DATUM_REAGENT:
				reagentNode = curNode;
				break;
			case Datum::DATUM_DATEANALYSED: // should differenciate between start date and date analysed?
			case Datum::DATUM_STARTDATE:
				dateNode = curNode;
				break;
			case Datum::DATUM_PID:
			case Datum::DATUM_BARCODE:
			case Datum::DATUM_SAMPLETYPE:
			case Datum::DATUM_TID:
			case Datum::DATUM_UNIT:
			case Datum::DATUM_RESULTSTRING:
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
				 curNode = addDatum(curdatum, rootNode);
				break;
			case Datum::DATUM_TID:
			case Datum::DATUM_UNIT:
			case Datum::DATUM_RESULTSTRING:
			case Datum::DATUM_DATEANALYSED:
			case Datum::DATUM_CUVETTE:
			case Datum::DATUM_REAGENT:
				curNode = addDatum(curdatum, resultNode);
				break;
			case Datum::DATUM_REAGENTID:
			case Datum::DATUM_REAGENTLOT:
				curNode = addDatum(curdatum, reagentNode);
				break;
			case Datum::DATUM_YEAR:
			case Datum::DATUM_MONTH:
			case Datum::DATUM_DAY:
			case Datum::DATUM_HOUR:
			case Datum::DATUM_MINUTE:
			case Datum::DATUM_SECOND:
				curNode == addDatum(curdatum, dateNode);
				break;
			default:
				throw Exception("Invalid datum type");
		}
	} // for vecpDatum
}

TTreeNode * TformEditFormat::addDatum(Datum * add, TTreeNode * to) {
	// TODO - nothing to stop you adding e.g. PID twice
	// what you can add depends on parent object - check valid
	Datum::DatumType addType = add->datumType;    // ok
	Datum::DatumType toType  = ((Datum *)to->Data)->datumType; //??
//	tmp.sprintf("%s   [%d]:'%s' [%d][%d]:'%s'",

//    if (add->isContainer()) {
//		tmp = Datum::datumInfo[add->datumType].description;
//	} else {
//		tmp.sprintf("%s   [%d]:'%s' [col %d][sub %d][len %d][off %d]:'%s'",
//			Datum::datumInfo[add->datumType].description,
//			add->tagCol, add->tag.c_str(), add->valCol, add->valSubCol,
//			add->length, add->offset,
//			add->example.c_str());
//    }
//	switch (addType) {
//		case Datum::DATUM_ROOT:
//		case Datum::DATUM_STARTDATE:
//		case Datum::DATUM_RESULT:
//		case Datum::DATUM_DATEANALYSED:
//		case Datum::DATUM_REAGENT:
//			tmp = Datum::datumInfo[add->datumType].description;
//			break; // parameters meaningless for containers
//		default:
//			tmp.sprintf("%s   [%d]:'%s' [col %d][sub %d][len %d][off %d]:'%s'",
//				Datum::datumInfo[add->datumType].description,
//				add->tagCol, add->tag.c_str(), add->valCol, add->valSubCol,
//				add->length, add->offset,
//				add->example.c_str());
//	}

	TTreeNode * addedNode;
	switch (Datum::datumRelation[toType][addType]) {
		case 0:
			// invalid addition
			return NULL;
		case 1:
			// InsertObject adds sibling immediately before 'to' Node
			addedNode = treeviewParse->Items->InsertObject(to, add->infoString(), (TObject *)add);
			addedNode->DropTarget = true; //???
			break;
		case 2:                                               
			addedNode = treeviewParse->Items->AddChildObject(to, add->infoString(), (TObject *)add);
			addedNode->DropTarget = true;
			to->Expand(false); // bool parameter is whether to recurse
			break;
		case 3:
			// AddObject adds node as *last* sibling of common parent, not next node....
			addedNode = treeviewParse->Items->AddObject(to, add->infoString(), (TObject *)add);                       
			addedNode->DropTarget = true; //???
			break;
		default:
			throw "No relationship defined"; // shouldn't happen
	}
	addedNode->SelectedIndex = Datum::datumInfo[add->datumType].imageIndex;
	addedNode->StateIndex = Datum::datumInfo[add->datumType].imageIndex;
	addedNode->ImageIndex = Datum::datumInfo[add->datumType].imageIndex;
	//addedNode->AbsoluteIndex
	return addedNode;
}

void TformEditFormat::itemAdd(TObject *Sender, Datum::DatumType addType) {
	try { // validate parameters // TODO more validation
		if (editTag->Text.IsEmpty()) throw Exception("Tag cannot be empty");
		if (NULL == treeviewParse->Selected) throw Exception("No item selected");

		// what might we be adding, and to what
		Datum::DatumType selectedType = ((Datum *)treeviewParse->Selected->Data)->datumType;
		//Datum * datum = new Datum;
		Datum * datum = new Datum(
			addType,
			editTag->Text,
			editTagCol->Text.ToInt(),
			editValCol->Text.ToInt(),
			editValSubCol->Text.ToInt(),
			editValLength->Text.ToInt(),
			editOffset->Text.ToInt(),
			editExample->Text);
// 		datum->id_filetype   = -1; // will be format->id when id claimed

		// what you can add depends on parent object - check valid
		if (NULL == addDatum(datum, treeviewParse->Selected)) {
			tmp.sprintf("Cannot add %s to %s", Datum::datumInfo[addType].description, Datum::datumInfo[selectedType].description);
			throw Exception(tmp);
		}

		//TODO btnAdd->Enabled = false;
		//btnRemove->Enabled = false;
		//btnSave->Enabled = false;
	} catch (Exception & e) {
		Application->MessageBoxA(e.Message.c_str(), "Info", MB_OK);
	}
}

void __fastcall TformEditFormat::editOffsetChange(TObject *Sender)
{
	attributeChanged();
}

void __fastcall TformEditFormat::btnSaveClick(TObject *Sender)
{

	Datum * datum = ((Datum *)treeviewParse->Selected->Data);
	//Datum *& datum = ((Datum *&)treeviewParse->Selected->Data);
	if (datum->isContainer()) {
		return; //XXX 'Save' button shouldn't have been enabled on a container node
		// XXX how to log?
	}
	// changes to datum object seem to no effect beyond this function
	// because Data is a __property and we have a copy of the object it holds?
	// e.g.:
	datum->update(
		editTag->Text,
		editTagCol->Text.ToInt(),
		editValCol->Text.ToInt(),
		editValSubCol->Text.ToInt(),
		editValLength->Text.ToInt(),
		editOffset->Text.ToInt(),
		editExample->Text);
	treeviewParse->Selected->Text = datum->infoString();
	treeviewParse->Selected->Data = datum;
	// changes the treenode text, but the data seems to remain the same.
	// try this:


//	Datum * newDatum = new Datum(
//    	datum->datumType,
//		editTag->Text,
//		editTagCol->Text.ToInt(),
//		editValCol->Text.ToInt(),
//		editValSubCol->Text.ToInt(),
//		editValLength->Text.ToInt(),
//		editOffset->Text.ToInt(),
//		editExample->Text);
//	if (treeviewParse->Selected->HasChildren) {
//		// should never be able to get here
//		throw Exception("Tried to delete container node");
//	}
//	// get this node's parent
//	TTreeNode * parent = treeviewParse->Selected->Parent;
//	treeviewParse->Selected->Free(); // needed??
//	treeviewParse->Selected->Delete();
//	tmp = Datum::datumInfo[newDatum->datumType].description;
//	TTreeNode * newNode = treeviewParse->Items->AddObject(parent, tmp, (TObject *)newDatum);
//
//	//treeviewParse->Items->AddChildObject(parent, )->Item->
//	tmp.sprintf("datum pointer: %x. newDatum pointer: %x", datum, newDatum);
//	OutputDebugString(tmp.c_str());
	// still doesn't appear to have changed when trying to save into db!

	//delete datum; //!!
}


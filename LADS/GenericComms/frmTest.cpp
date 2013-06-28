//---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop

#include "frmTest.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"
TFormTest *FormTest;
//---------------------------------------------------------------------------
__fastcall TFormTest::TFormTest(TComponent* Owner)
	: TForm(Owner)
{
}
//---------------------------------------------------------------------------
/* code graveyard */

//treeviewParse->FullExpand();
//	switch (clicked->Tag) {
//		case
//
//	default:
//		;
//	}
//	tmp.sprintf("Delete clicked on a '%s', TTreeNode: '%s'", String(Sender->ClassName()).c_str(), treeviewParse->Selected->Text.c_str());
//	OutputDebugString(tmp.c_str());
//
//	DatumType selectedType = ((Datum *)(treeviewParse->Selected->Data))->datumType;
//	if (selectedType == DATUM_ROOT) {
//		Application->MessageBoxA("Cannot delete root node", "Info", MB_OK);
//		return;
//	}
//	if (treeviewParse->Selected->HasChildren) {
//		Application->MessageBoxA("Selected node has child nodes, please delete these first", "Info", MB_OK);
//		return;
//	}
//	treeviewParse->Selected->Delete();
//	//tmp.sprintf("no Children"); OutputDebugString(tmp.c_str());

//void __fastcall TformEditFormat::comboDateFormatChange(TObject *Sender)
//{
/*	try {
		// update example date using format
		std::string selected(comboDateFormat->Items->Strings[comboDateFormat->ItemIndex].c_str());
		unsigned short year, month, day, hour, minute, second, millisecond;
		TDateTime now = Now();
		DecodeDate(now, year, month, day);
		DecodeTime(now, hour, minute, second, millisecond);
		if (0 == selected.compare("DDMMYYhhmmss")) { // in case they get out of step
			tmp.sprintf(strDateTimeFormats[comboDateFormat->ItemIndex][1], day, month, String(year).SubString(3, 2).c_str(), hour, minute, second);
		} else if (0 == selected.compare("YYYYMMDDhhmmss")) {
			tmp.sprintf(strDateTimeFormats[comboDateFormat->ItemIndex][1], year, month, day, hour, minute, second);
		} else if (0 == selected.compare("DDMMYYYY")) {
			tmp.sprintf(strDateTimeFormats[comboDateFormat->ItemIndex][1], day, month, year);
		} else {
			throw new Exception("Unsupported date format");
		}
		labelDateExample->Caption = String("e.g. ")+tmp;
	} catch (Exception &e) {
		String tmp;
		tmp.sprintf("Error: %s", e.Message);
		Application->MessageBoxA(tmp.c_str(), "Error", MB_OK);
	} catch (...) {
		Application->MessageBoxA("Error: other", "Error", MB_OK);
	} */
//}

			//string tok("test"); throws exception even when *it not referenced
			//std::ostringstream ostrstrDebug, ostrstrTok;
			//ostrstrTok << *it;//s << endl;
			//ostrstrDebug << "tok: '" << *it << "'" << endl;
			//OutputDebugString(ostrstrDebug.str().c_str()); //memoDebug->Lines->Add(tmp.c_str());
			//OutputDebugString(tok.c_str());

	// input_facet
	// boost::date_time::date_input_facet
	//using namespace boost::date_time;
	//using namespace boost::gregorian;
	//using boost::gregorian::date;
	//using namespace boost::gregorian;
	//using boost::date_time::time_facet;
	//using namespace boost::date_time;
	//using namespace boost::posix_time;
//		std::string ts("2012-01-20 23:59:59.000");
//		ptime test_time(time_from_string(ts));
//		//time_facet *facet = new time_facet(comboDateFormat->Text.c_str());
//		boost::posix_time::time_facet *facet = new boost::posix_time::time_facet("%d-%b-%Y %H:%M:%S"%);
		//time_facet< ptime, char > facet("%d-%b-%Y %H:%M:%S");
		//vcl
		//TDateTime FormatString?
		//strftime
//		stringstream ss;
//		ss.imbue(locale(locale::classic(), facet));
//		ss << test_time;
//		OutputDebugString(ss.str().c_str());
	//ss.imbue(locale(locale::classic(), new time_facet(comboDateFormat->Text.c_str())));
	//boost::posix_time::ptime the_time;
	//ss >> the_time;
//	date d(2010, 1, 30);
//	std::stringstream strstr;
	//time_input_facet tif;
//	strstr << d.year() << std::endl;
//	strstr << d.month() << std::endl;
//	strstr << d.day() << std::endl;
//	strstr << d.day_of_week() << std::endl;
//	strstr << d.end_of_month() << std::endl;

//	string escape(editEscape->Text.c_str());
//	string delim(editDelim->Text.c_str());
//	string subdelim(editSubDelim->Text.c_str());
//	string quote("\"");  // this is for the quote char - default?
	// boost::offset_separator // for fixed-width
//	tmp.sprintf("findMatch(): tag: '%s', tagCol %d, valCol %d, valSubCol %d", tag.c_str(), tagCol, valCol, valSubCol); OutputDebugString(tmp.c_str());
//	tmp.sprintf("e: '%s', d: '%s', q: '%s'", escape.c_str(), delim.c_str(), quote.c_str()); OutputDebugString(tmp.c_str());
//	tmp.sprintf("line: '%s'", line.c_str()); OutputDebugString(tmp.c_str());
	//escaped_list_separator<char> els(e[0], d[0], q[0]);
	//tokenizer< escaped_list_separator<char> > tok(line, els);

//	String tmp;
//	int selected = sgItems->Row;
//	Datum * field = (Datum *)(sgItems->Objects[0][selected]);
//	tmp.sprintf("Row: %d", selected);
//	memoDebug->Lines->Add(tmp.c_str());
//	OutputDebugString(tmp.c_str());
//    tmp.sprintf("Delete field?");
//	if (IDOK == Application->MessageBoxA(tmp.c_str(), "info", MB_OKCANCEL)) {
//		if (2 == sgItems->RowCount) {
//			for (int i=1; i<sgItems->ColCount; i++)
//                sgItems->Cells[i][1] = ""; // just clear
//			sgItems->Objects[0][1] = NULL;
//        } else if (selected == sgItems->RowCount-1) { // last row
//			sgItems->RowCount = sgItems->RowCount-1;
//        } else {
//			//sgItems->DeleteRow(sgItems->Row); // protected
//            assert(sgItems != NULL);
//			SNDMSG(sgItems->Handle, WM_SETREDRAW, false, 0);
//			try {
//				const int row_count = sgItems->RowCount;
//				for (int row = selected; row < row_count - 1; ++row) {
//					sgItems->Rows[row] = sgItems->Rows[row + 1]; // shift the contents of the trailing rows
//				}
//				sgItems->RowCount = row_count -1; // remove the last row
//			} __finally {
//				SNDMSG(sgItems->Handle, WM_SETREDRAW, true, 0);
//			}
//			RECT R = sgItems->CellRect(0, selected);
//			InflateRect(&R, sgItems->Width, sgItems->Height);
//			InvalidateRect(sgItems->Handle, &R, false); // repaint
//		}
//		delete field;
//	}         
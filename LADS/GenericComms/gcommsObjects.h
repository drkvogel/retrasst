
#ifndef gcommsObjectsH
#define gcommsObjectsH

#include <Classes.hpp>
#include <vector>
#include <boost/tokenizer.hpp>
#include "LCDbProject.h"
//---------------------------------------------------------------------------
static String tmp;

const static bool MYDEBUG =
#ifdef _DEBUG
	true;
#else
	false;
#endif

const static bool CONFIRMATIONS_OFF = true;

namespace genericComms {

template<typename Container>
void delete_referenced(Container& c) { while (!c.empty()) delete c.back(), c.pop_back(); }

enum FormatType { INPUT_FORMAT, OUTPUT_FORMAT };
enum FieldType  { FIELDTYPE_STRING, FIELDTYPE_INT, FIELDTYPE_FLOAT, FIELDTYPE_DATE };
enum PageType { PAGE_GENERAL, PAGE_MANAGE_FORMATS, PAGE_EDIT_FORMAT, PAGE_CREATE_WORKLIST, PAGE_ENTER_RESULTS };

typedef boost::tokenizer<boost::escaped_list_separator<char> > mytokenizer;

//static TColor formatColour[] = { 0x00FFC0E0, 0x00f4c27b };
// Indigo for input, Orange for output
// (TColor coding is back to front - in HTML/CSS Orange is #ffa500 not 0x00a5ff
// Indigo: 0x00FFC0E0
// Indigo2: 0x00ff34f2
// Orange: 0x0000a5ff
// Olive:  0x00008080
// Olive2: 0x00a7d5d6 (low saturation)
// Olive3: 0x00b6d6d6 (lower)
// Olive4: 0x00c8d6d6 (even lower)
static TColor formatColour[] = { (TColor)0x00ffe4f2, (TColor)0x00a7d5d6 };

// Date formats we have seen in the field:
// nephelometer: 140611093518
// menarini:     20120328095500
// DXC:          17062011
// DXH:          20120309112926
//
//	"DDMMYYhhmmss",
//  "YYYYMMDDhhmmss",
//  "DDMMYYYY"
//  DD / MM / YY   hh : mm : ss

//static const int numStrDateTimeFormats = 3;
//static char *strDateTimeFormats[numStrDateTimeFormats][128] = {
//	{ "DDMMYYhhmmss", "%02hu%02hu%2.2s%02hu%02hu%02hu" },
//	{ "YYYYMMDDhhmmss", "%04hu%02hu%02hu%02hu%02hu%02hu" },
//	{ "DDMMYYYY", "%02hu%02hu%04hu" }
//};
//const int numStrDateTimeFormats = sizeof(strDateTimeFormats)/sizeof(strDateTimeFormats[0]);

static char *strDelims[][2] = {
	{ "Comma", "," },
	{ "Pipe", "|" },
	{ "Tab", "\t" },
	{ "Semicolon", ";" },
	{ "Space", " " },
	{ "Fixed width", "?" },
	{ "Custom", "" }
};
static const int numStrDelims = sizeof(strDelims)/sizeof(strDelims[0]);

static char *strSubDelims[][2] = {
	{ "Circumflex", "^" },
	{ "Pipe", "|"},
	{ "Comma", "," },
	{ "Custom", "" } };
static const int numStrSubDelims = sizeof(strSubDelims)/sizeof(strSubDelims[0]);

static char *strEscapeChars[][2] = {
	{ "Backslash", "\\" },
	{ "Apostrophe", "'" },
	{ "Custom", "" }
};
static const int numStrEscapeChars = sizeof(strEscapeChars)/sizeof(strEscapeChars[0]);

static char *strQuoteChars[][2] = {
	{ "Double", "\"" },
	{ "Single", "\'" },
	{ "Custom", "" }
};
static const int numStrQuoteChars = sizeof(strQuoteChars)/sizeof(strQuoteChars[0]);

static char *strFieldTypes[] = { "String", "Int", "Float", "Date" };
static const int numStrFieldTypes = sizeof(strFieldTypes)/sizeof(strFieldTypes[0]);
	
class Datum : public TObject {
public:
	enum DatumType {
		DATUM_ROOT,
		DATUM_PID,
		DATUM_BARCODE,
		DATUM_STARTDATE,    // container
		DATUM_SAMPLETYPE,
		DATUM_RESULT,		// container
		DATUM_TID,
		DATUM_UNIT,
		DATUM_RESULTSTRING,
		DATUM_DATEANALYSED, // container
		DATUM_CUVETTE,
		DATUM_REAGENT,      // container
		DATUM_REAGENTID,
		DATUM_REAGENTLOT,
		DATUM_NOTES,
		DATUM_YEAR,
		DATUM_MONTH,
		DATUM_DAY,
		DATUM_HOUR,
		DATUM_MINUTE,
		DATUM_SECOND,
		DATUM_COUNT };
	DatumType 	datumType; // what this datum represents, e.g. Patient ID
	int         id;
	int         id_filetype;
	int			parent;
	String      tag;
	int	        tagCol;
	int         valCol;
	int         valSubCol;
	int			offset;
	int			length;
//	int			ival;
//	float		fval;
//	String		sval;
	int 		pos;
	String 		example; // but not stored in db - should be looked up from example file on load

	Datum();
	Datum(DatumType _datumType, String _tag, int _tagCol, int _valCol, int _valSubCol,
			int _length, int _offset, String _example = "") :
		datumType(_datumType), tag(_tag), tagCol(_tagCol), valCol(_valCol), valSubCol(_valSubCol),
		length(_length), offset(_offset), example(_example) {

	}
	void update(String _tag, int _tagCol, int _valCol, int _valSubCol,
			int _length, int _offset, String _example = "") {
		//datumType = _datumType; // shouldn't be able to change type
		tag = _tag;
		tagCol = _tagCol;
		valCol = _valCol;
		valSubCol = _valSubCol;
		length = _length;
		offset = _offset;
		example = _example;
	};

	bool isContainer() {
		switch (datumType) {
			case DATUM_ROOT:
			case DATUM_STARTDATE:
			case DATUM_RESULT:
			case DATUM_DATEANALYSED:
			case DATUM_REAGENT:
				return true;
			default:
				return false;
		}

	};

	String infoString() {
		String tmp;
		if (isContainer()) {
			return Datum::datumInfo[datumType].description;
		} else {
			tmp.sprintf("%s   [%d]:'%s' [col %d][sub %d][off %d][len %d]:'%s'",
				Datum::datumInfo[datumType].description,
				tagCol, tag.c_str(), valCol, valSubCol,
				offset, length,
				example.c_str());
			return tmp;
		}
    }

	struct DatumInfo {
		DatumType type;  //??? already got datumType in parent class
				// need for loading types into controls e.g. combo
				// statically from class
				// without any instances of Datum
				// DatumInfo should perhaps be a separate class
		char*     description; // for dropdown
		int		  imageIndex;  // for TImageList
		int 	  order;
	};

	static const DatumInfo datumInfo[];
	static const int numDatumIdentifiers;
	static const int datumRelation[][DATUM_COUNT];

//	Datum(int _id = 0, int _filetype = 0, String _name = "", String _tag = "", int _tagCol = 0, int _valCol, int _valSubCol,
//			int _ival, float _fval, String _sval, int _pos, String _example) :
//		id(_id), id_filetype(_filetype), name(_name), tag(_tag), tagCol(_tagCol), valCol(_valCol), valSubCol(_valSubCol),
//		ival(_ival), fval(_fval), sval(_sval), pos(_pos), example(_example)
//	{};

};
typedef Datum* pDatum;
typedef std::vector< pDatum > VecpDatum;

const Datum::DatumInfo Datum::datumInfo[] = {
	{ DATUM_ROOT, 			"[Analysis]", 		0,	0},     // container
	{ DATUM_PID,    		"Patient ID", 		1,	1},
	{ DATUM_BARCODE,    	"Barcode", 			2,	2},
	{ DATUM_STARTDATE,		"[Start date]", 	9,  3},		// container
	{ DATUM_SAMPLETYPE, 	"Sample type",		4,	5},
	{ DATUM_RESULT,			"[Result]",			5,	6},   	// container
	{ DATUM_TID,    		"Test ID",			6,	7},
	{ DATUM_UNIT,       	"Unit", 			7,	8},
	{ DATUM_RESULTSTRING, 	"Result string",	8,	9},
	{ DATUM_DATEANALYSED,   "[Date Analysed]",	9,	10}, 	// container
	{ DATUM_CUVETTE,    	"Cuvette Type",		10,	11},
	{ DATUM_REAGENT,    	"[Reagent]",		11,	12},  	// container
	{ DATUM_REAGENTID,  	"Reagant ID",		12,	13},
	{ DATUM_REAGENTLOT, 	"Reagant Lot",		13,	14},
	{ DATUM_NOTES,      	"Notes",			14,	15},
	{ DATUM_YEAR,       	"Year",				3,	4},
	{ DATUM_MONTH, 			"Month",			3,	4},
	{ DATUM_DAY, 			"Day",				3,	4},
	{ DATUM_HOUR,  			"Hour",				3,	4},
	{ DATUM_MINUTE,			"Minute",			3,	4},
	{ DATUM_SECOND,			"Second",			3,	4}
};

// static member variables must be initialized outside class declaration
// with type (e.g. const int) but without static keyword
const int Datum::numDatumIdentifiers = sizeof(Datum::datumInfo)/sizeof(Datum::datumInfo[0]); //???

// which data can be added to which other data, and as a sibling or a child?
// 0 - not allowed; 1 - sibling before; 2 - child; 3 - sibling after
const int Datum::datumRelation[][Datum::DATUM_COUNT] = {

// XXX remember to addSiblingBefore or addSiblingAfter depending on relation == 1 or relation ==3

// add ROOT PID BARCODE STARTDATE SAMPLETYPE RESULT TID UNIT RESULTSTRING DATEANAL CUVETTE REAGENT REAGENTID REAGENTLOT NOTES YEAR MONTH DAY HOUR MINUTE SECOND to \/
	 { 0,   2,  2, 		2, 		  2,         2,     0,  0,   0,           0,       0,      0,      0,        0,         2,    0,   0,    0,  0,   0,     0 }, //  ROOT
	 { 0,   0,  0,      0,        0,         0,     0,  0,   0,           0,       0,      0,      0,        0,         0,    0,   0,    0,  0,   0,     0 }, //  PID
	 { 0,   0,  0,      0,        0,         0,     0,  0,   0,           0,       0,      0,      0,        0,         0,    0,   0,    0,  0,   0,     0 }, //  BARCODE
	 { 0,   0,  0,      0,        0,         0,     0,  0,   0,           0,       0,      0,      0,        0,         0,    2,   2,    2,  2,   2,     2 }, //  STARTDATE
	 { 0,   0,  0,      0,        0,         0,     0,  0,   0,           0,       0,      0,      0,        0,         0,    0,   0,    0,  0,   0,     0 }, //  SAMPLETYPE
	 { 0,   0,  0,      0,        0,         1,     2,  2,   2,           2,       2,      2,      0,        0,         0,    0,   0,    0,  0,   0,     0 }, //  RESULT
	 { 0,   0,  0,      0,        0,         0,     0,  0,   0,           0,       0,      0,      0,        0,         0,    0,   0,    0,  0,   0,     0 }, //  TID
	 { 0,   0,  0,      0,        0,         0,     0,  0,   0,           0,       0,      0,      0,        0,         0,    0,   0,    0,  0,   0,     0 }, //  UNIT
	 { 0,   0,  0,      0,        0,         0,     0,  0,   0,           0,       0,      0,      0,        0,         0,    0,   0,    0,  0,   0,     0 }, //  RESULTSTRING
	 { 0,   0,  0,      0,        0,         0,     0,  0,   0,           0,       0,      0,      0,        0,         0,    2,   2,    2,  2,   2,     2 }, //  DATEANAL
	 { 0,   0,  0,      0,        0,         0,     0,  0,   0,           0,       0,      0,      0,        0,         0,    0,   0,    0,  0,   0,     0 }, //  CUVETTE
	 { 0,   0,  0,      0,        0,         0,     0,  0,   0,           0,       0,      1,      2,        2,         0,    0,   0,    0,  0,   0,     0 }, //  REAGENT
	 { 0,   0,  0,      0,        0,         0,     0,  0,   0,           0,       0,      0,      0,        0,         0,    0,   0,    0,  0,   0,     0 }, //  REAGENTID
	 { 0,   0,  0,      0,        0,         0,     0,  0,   0,           0,       0,      0,      0,        0,         0,    0,   0,    0,  0,   0,     0 }, //  REAGENTLOT
	 { 0,   0,  0,      0,        0,         0,     0,  0,   0,           0,       0,      0,      0,        0,         1,    0,   0,    0,  0,   0,     0 }, //  NOTES
	 { 0,   0,  0,      0,        0,         0,     0,  0,   0,           0,       0,      0,      0,        0,         0,    0,   3,    3,  3,   3,     3 }, //  YEAR
	 { 0,   0,  0,      0,        0,         0,     0,  0,   0,           0,       0,      0,      0,        0,         0,    1,   0,    3,  3,   3,     3 }, //  MONTH
	 { 0,   0,  0,      0,        0,         0,     0,  0,   0,           0,       0,      0,      0,        0,         0,    1,   1,    0,  3,   3,     3 }, //  DAY
	 { 0,   0,  0,      0,        0,         0,     0,  0,   0,           0,       0,      0,      0,        0,         0,    1,   1,    1,  0,   3,     3 }, //  HOUR
	 { 0,   0,  0,      0,        0,         0,     0,  0,   0,           0,       0,      0,      0,        0,         0,    1,   1,    1,  1,   0,     3 }, //  MINUTE
	 { 0,   0,  0,      0,        0,         0,     0,  0,   0,           0,       0,      0,      0,        0,         0,    1,   1,    1,  1,   1,     0 }  //  SECOND
};

class FileFormat {
public:
	int         id;
	String      name;
	String      description;
	FormatType  type;
	int         firstrow;
	char        delim;
	char        subdelim;

	FileFormat() : id(-1), name("undef"), description("undef"), type(OUTPUT_FORMAT),
		firstrow(-1), delim('\0'), subdelim('\0') {} //, status(-1) {};
	FileFormat(int _id, String _name, String _desc, FormatType _type, int _firstrow,
			String _delim, String _subdelim) :
			id(_id), name(_name), description(_desc), type(_type), firstrow(_firstrow)
	{
		if (_delim.IsEmpty()) {
			String tmp;
			tmp.sprintf("Null delimiter in format '%s'!", _delim.c_str());
			throw tmp;
		}
		delim       = _delim[1];
		subdelim    = _subdelim.IsEmpty() ? '\0' : _subdelim[1];
	}
};
typedef FileFormat* pFileFormat;
typedef std::vector< pFileFormat > VecpFileFormat;

class FormatMap {
    int         id_format;
	int         id_cluster;
    int         status;
};
typedef FormatMap* pFormatMap;
typedef std::vector< pFormatMap > VecpFormatMap;

// might need in order to flatten limsanalysis structure
class WorklistItem : public TObject {
public:
    int         machineID;
	String      barcode;
    String      testName;
    int         testID;
    int         projectID;
    String      projectName;
    String      profileName;
    TDateTime   timeStamp;

    WorklistItem(  int          _machineID,
                    String      _barcode,
                    String      _testName,
                    int         _testID,
                    int         _projectID,
                    String      _profileName,
                    TDateTime   _timeStamp) :
        machineID(_machineID),
        barcode(_barcode),
        testName(_testName),
        testID(_testID),
        projectID(_projectID),
        profileName(_profileName),
		timeStamp(_timeStamp),
		projectName( LCDbProjects::records().get(_projectID).getName() )
		{}

    operator< (const WorklistItem wi) {
        return (timeStamp < wi.timeStamp);
	}
};

class foWorklistItemLessTDT { // for sorting worklist by time? use STL sort
							  // fo = function object; TDT = ?
public:
	bool operator() (const WorklistItem* wi1, const WorklistItem* wi2) const {
		return (wi1->timeStamp < wi2->timeStamp);
	};
};
typedef WorklistItem* pWorklistItem;
typedef std::vector< pWorklistItem > tdvecpWorklistItem;

class WorklistSelection : public TObject {
	//tdvecpWorklistItem
	std::vector< WorklistItem * > items;
public:
	WorklistSelection() {};
	WorklistSelection(tdvecpWorklistItem & worklistItems) {
		items = worklistItems; // ???
    }                       	
	void clear() { items.clear(); }
};

// flattened result structure for output to screen/csv
class Result {
	int 		patientID;
	String		barcode;
	TDateTime	startDate;
	String		sampleType;
	int			testID;
	String		unit;
	String		result;
	TDateTime	dateAnalysed;
	String		cuvetteType;
	int			reagentID;
	String		reagentLot;
	String		notes;
};
typedef Result * pResult;
typedef std::vector< pResult > VecpResult;

} // namespace genericComms

//---------------------------------------------------------------------------
#endif

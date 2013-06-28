//---------------------------------------------------------------------------

#ifndef FormCommonH
#define FormCommonH
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
#include <Db.hpp>
#include <DBTables.hpp>
#include <DB.hpp>
#include <IdBaseComponent.hpp>
#include <IdComponent.hpp>
#include <IdMessageClient.hpp>
#include <IdSMTP.hpp>
#include <IdTCPClient.hpp>
#include <IdTCPConnection.hpp>

#include <vector>
#include <map>
#include <fstream>

#include "LCDbProject.h"
#include "LCDbAuditTrail.h"

#define QF(x)      frmCommon->x->Fields->Fields
#define QTEXT(x)   frmCommon->x->SQL->Text
#define QOPEN(x)   frmUtils->query_open(frmCommon->x)
#define QEXEC(x)   frmUtils->query_exec(frmCommon->x)
#define QNEXT(x)   frmCommon->x->Next()
#define QCLOSE(x)  frmCommon->x->Close()
#define QEOF(x)    frmCommon->x->Eof

// ???
#define QPARAM(x,y)  frmCommon->x->ParamByName(y)

const bool MYDEBUG =
#ifdef _DEBUG
    true;
#else
    false;
#endif

class WorklistItem {
public:
    WorklistItem(  int     _iMachineID,
                    String  _strBarcode,
                    String _strTestName,
                    int _iTestID,
                    int _iProjectID,
                    String _strProfileName,
                    TDateTime _tdtTimeStamp) :
        iMachineID(_iMachineID),
        strBarcode(_strBarcode),
        strTestName(_strTestName),
        iTestID(_iTestID),
        iProjectID(_iProjectID),
        strProfileName(_strProfileName),
		tdtTimeStamp(_tdtTimeStamp),
		strProjectName( LCDbProjects::records().get(_iProjectID).getName() )
		{}

    WorklistItem() {}

    operator< (const WorklistItem wi) {
        return (tdtTimeStamp < wi.tdtTimeStamp);
    }

    int     iMachineID;
    String  strBarcode;
    String  strTestName;
    int     iTestID;
    int     iProjectID;
    String  strProjectName;
    String  strProfileName;
    TDateTime tdtTimeStamp;
};

class foWorklistItemLessTDT {
public:
    bool operator() (const WorklistItem* wi1, const WorklistItem* wi2) const {
        return (wi1->tdtTimeStamp < wi2->tdtTimeStamp);
    };
};

typedef WorklistItem* pWorklistItem;
typedef std::vector< pWorklistItem > tdvecpWorklistItem;

class EmpowerOutputRow {
public:
    EmpowerOutputRow() :
        iRow(0), strBarcode(""), strTestID(""), flResultValue(0), flAreaHeight(0),
        tdtDateOfAnalysis(NULL), iVialNum(0) {}
    int         iRow;
    String      strBarcode;
    String      strTestID;
    float       flResultValue;
    float       flAreaHeight;
    TDateTime   tdtDateOfAnalysis;
    int         iVialNum;

    operator String() {
        String temp;
        temp.sprintf(
            "Vial: %5d Barc: %-10s Test: %-20s Result: %10f Area: %10f date: %-20s",
            iVialNum,
            strBarcode,
            strTestID,
            flResultValue,
            flAreaHeight,
            tdtDateOfAnalysis.DateTimeString());

        return temp;
    }
};

typedef EmpowerOutputRow* pEmpowerOutputRow;
typedef std::vector< pEmpowerOutputRow > tdvecpEmpowerOutputRow;

typedef std::vector< String > tdvecString;

class TokenString {
private:
    tdvecString vecStrings;
    tdvecString::const_iterator it;
    String  strText;
    String  strDelimiters;  // ??? must make sure there are no duplicates
    int     i;
    int     iStart;
    void Tokenize() {//ansistring
        vecStrings.clear();
        iStart = i = 1;
        int end = strText.Length();
        for (; i<=end; i++) {
            for (int j=1; j<=strDelimiters.Length(); j++) {
                if (i==end) {
                    vecStrings.push_back(strText.SubString(iStart, i+1 - iStart));
                } else if (strDelimiters[j] == strText[i]) {
                    vecStrings.push_back(strText.SubString(iStart, i - iStart));
                    iStart = i+1;
                }
            }
        }
    }
public:
    TokenString(String _strText, String _strDelimiters)
        : strText(_strText), strDelimiters(_strDelimiters) { Tokenize(); }
    ~TokenString() { vecStrings.clear(); } //???
    String First() {
        it = vecStrings.begin();
        if (vecStrings.size() == 0) return "";
        return *it;
    }
    String Next() {
        if (it == vecStrings.end() ) return "";
        it++;
        return *it;
    }
    operator String() {
        String temp;
        tdvecString::const_iterator it;
        int i;
        for (i=0, it = vecStrings.begin(); it != vecStrings.end(); it++, i++) {
            temp += String(i)+":"+*it + " ";
        }
        return temp;
    }
};

//---------------------------------------------------------------------------
class TfrmCommon : public TForm
{
__published:	// IDE-managed Components

	TQuery *qryBuddyDB;
	TDatabase *dbBuddy;
	TDatabase *dbCentral;
	TQuery *qryLabCentral;

private:	// User declarations

	LCDbAuditTrail auditTrail;

public:		// User declarations

	__fastcall TfrmCommon(TComponent* Owner);
};

//---------------------------------------------------------------------------
extern PACKAGE TfrmCommon *frmCommon;
//---------------------------------------------------------------------------
#endif

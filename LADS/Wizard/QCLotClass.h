//---------------------------------------------------------------------------
#ifndef QCLotClassH
#define QCLotClassH

#include "dbtables.hpp"

//---------------------------------------------------------------------------

int SortByTest( const void *a, const void *b );

class QCTestClass;

class QCLotClass
{
	void FormatSQL( TQuery* query );

	int numQCTests;
	QCTestClass** theQCTests;

	// persistent parameters
	int QCID;
	AnsiString Material;
	AnsiString Level;
	AnsiString Name;
	AnsiString Description;
	AnsiString ArrivalDate;

public:
	QCLotClass();
	QCLotClass(AnsiString aName);
	virtual __fastcall ~QCLotClass();

	//set get QCID
	int GetQCID(){ return QCID;}
	void SetQCID(int mid){ QCID = mid; }

	//set get Name
	AnsiString GetName(){ return Name;}
	void SetName(AnsiString name){ Name = name; }

	//set get Material
	AnsiString GetQCMaterial(){ return Material;}
	void SetQCMaterial(AnsiString mat){ Material = mat; }

	//set get Level
	AnsiString GetQCLevel(){ return Level;}
	void SetQCLevel(AnsiString lev){ Level = lev; }

	// set get Description
	AnsiString GetDescription(){ return Description; }
	void SetDescription(AnsiString desc){ Description = desc; }

	// set get ArrivalDate
	AnsiString GetArrivalDate() { return ArrivalDate; }
	void SetArrivalDate(AnsiString ad) { ArrivalDate = ad; }

	void LoadFromDatabase(TQuery* query, int recno);
	void InsertToDatabase(TQuery* query);
	void UpdateToDatabase(TQuery* query);

	//get numQCTests
	int GetNumQCTests(){ return numQCTests;}

	void LoadTestsFromDatabase(TQuery* query);
	void SortTestsAlphabetically();
	void DeleteQCTestClass();

	QCTestClass* GetQCTest( int index );
	QCTestClass* GetQCTest( AnsiString QCTestName );
	QCTestClass* GetQCTestByID( int id  );
};

#endif


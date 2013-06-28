//---------------------------------------------------------------------------

#ifndef testMainH
#define testMainH

#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
#include <Db.hpp>
#include <DBTables.hpp>
#include <ExtCtrls.hpp>
#include <DB.hpp>
#include <set>
#include <vector>

//---------------------------------------------------------------------------

class TtestForm : public TForm
{
__published:	// IDE-managed Components

    TMemo *display;
    TTimer *timer;
	TQuery *qCluster;
	TQuery *qCentral;
	TQuery *qProject;

    void __fastcall timerTimer(TObject *Sender);
    void __fastcall FormShow(TObject *Sender);
    void __fastcall FormHide(TObject *Sender);

private:	// User declarations

	std::vector< int > testIDs;
	std::vector< String > samples;
	String selected;

	bool magic() { return rand() % 8 == 0; }

	void readBarcodes();
    bool wipeSample();
	int createResults();

public:		// User declarations

    __fastcall TtestForm(TComponent* Owner);
};

//---------------------------------------------------------------------------

extern PACKAGE TtestForm *testForm;

//---------------------------------------------------------------------------
#endif

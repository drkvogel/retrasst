//---------------------------------------------------------------------------

#ifndef frmEnterResultsH
#define frmEnterResultsH
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
#include <Dialogs.hpp>
#include <ExtCtrls.hpp>
#include "gcommsObjects.h"
//---------------------------------------------------------------------------
using namespace genericComms;
class TformEnterResults : public TForm
{
__published:	// IDE-managed Components
	TOpenDialog *openDialogChooseFile;
	TGroupBox *GroupBox2;
	TMemo *memoPreview;
	TPanel *Panel1;
	TButton *btnOK;
	TButton *Button2;
	TPanel *Panel2;
	TPanel *Panel3;
	TEdit *editFilename;
	TButton *btnBrowse;
	TPanel *panelFormatChooser;
	TLabel *labelOutputFormat;
	TComboBox *comboFormat;
	TSplitter *Splitter1;
	TListBox *listboxInfo;
	TCheckBox *cbDummyRun;
	TCheckBox *cbGenerate;
	TButton *btnProcess;
	void __fastcall btnBrowseClick(TObject *Sender);
	void __fastcall btnOKClick(TObject *Sender);
	void __fastcall comboFormatChange(TObject *Sender);
	void __fastcall FormShow(TObject *Sender);
	void __fastcall btnProcessClick(TObject *Sender);
private:	// User declarations
	TStringList *lines;
	FileFormat * format;
	void parseFile();
public:		// User declarations
	__fastcall TformEnterResults(TComponent* Owner);
};
//---------------------------------------------------------------------------
extern PACKAGE TformEnterResults *formEnterResults;
//---------------------------------------------------------------------------
#endif

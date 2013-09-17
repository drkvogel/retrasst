//---------------------------------------------------------------------------

#ifndef mainDialogH
#define mainDialogH
//---------------------------------------------------------------------------
#include <System.Classes.hpp>
#include <Vcl.Controls.hpp>
#include <Vcl.StdCtrls.hpp>
#include <Vcl.Forms.hpp>
#include <Vcl.Dialogs.hpp>
#include <Vcl.ExtCtrls.hpp>
#include <Vcl.ExtDlgs.hpp>
//---------------------------------------------------------------------------
#include "RetrievalList.h"


#define DEF_BARCODE_MIN_LENGTH 8
#define DEF_BARCODE_MAX_LENGTH 16

class TfrmViewListMain : public TForm
{
__published:	// IDE-managed Components
	TComboBox *retrivalListComboBox;
	TButton *OpenListByNameButton;
	TPanel *Panel1;
	TPanel *Panel2;
	TOpenTextFileDialog *OpenTextFileDialog1;
	TButton *findBarCodesButton;
	TLabel *barCodeFileLabel;
	TComboBox *ProjectNamesComboBox;
	TButton *OpenList2Button;
	TLabel *Label3;
	TLabel *Label4;
	TComboBox *aliquitListComboBox;
	TLabel *Label5;
	TLabel *Label1;
	TLabel *Label2;
	TPanel *Panel3;
	TLabel *Label6;
	TButton *findBarCodesButton3;
	TLabel *barCode3FileLabel;
	TLabel *Label8;
	TButton *OpenList3Button;
	TComboBox *ProjectNamesComboBox3;
	TLabel *Label7;
	TComboBox *exerciseListComboBox;
	void __fastcall FormShow(TObject *Sender);
	void __fastcall OpenListByNameButtonClick(TObject *Sender);
	void __fastcall OpenList2ButtonClick(TObject *Sender);
	void __fastcall findBarCodesButton3Click(TObject *Sender);
	void __fastcall findBarCodesButtonClick(TObject *Sender);
	void __fastcall OpenList3ButtonClick(TObject *Sender);
	void __fastcall exerciseListComboBoxChange(TObject *Sender);
	void __fastcall FormDestroy(TObject *Sender);
private:	// User declarations
	void fillRetrivalListComboBox(int exercise_id);
	void fillRetrivalListComboBox();
	void fillProjectsListComboBox();
	void fillAliquotListComboBox();
	void fillExerciseListComboBox();
	void clearComboBox(TComboBox *cleanme);
	bool loadBarCodesFile(String &filename, std::vector<String> &barcodes);

	retrievalList m_retrievalList;
    String m_filename;
public:		// User declarations
	__fastcall TfrmViewListMain(TComponent* Owner);
};
//---------------------------------------------------------------------------
extern PACKAGE TfrmViewListMain *frmViewListMain;
//---------------------------------------------------------------------------
#endif

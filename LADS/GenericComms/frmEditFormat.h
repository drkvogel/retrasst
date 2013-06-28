//---------------------------------------------------------------------------

#ifndef frmEditFormatH
#define frmEditFormatH
#include <Classes.hpp>
#include <ComCtrls.hpp>
#include <Controls.hpp>
#include <Dialogs.hpp>
#include <ExtCtrls.hpp>
#include <Grids.hpp>
#include <StdCtrls.hpp>
#include <Menus.hpp>
#include <Forms.hpp>
#include <ImgList.hpp>
#include <ActnList.hpp>
//---------------------------------------------------------------------------
//#include <boost/date_time.hpp>
//#include <boost/date_time/gregorian/gregorian.hpp>
//#include <boost/date_time/posix_time/posix_time.hpp>
//#include <boost/date_time/posix_time/posix_time_io.hpp>
//#include <boost/date_time/time_facet.hpp>
//#include <boost/foreach.hpp>
//#include <boost/algorithm/string.hpp>
//#include <boost/algorithm/string/iter_find.hpp>
#include <boost/tokenizer.hpp>
#include <LQuery.h>
#include "frmManage.h"
#include "gcommsObjects.h"
//---------------------------------------------------------------------------
using namespace std;
using namespace boost;
using namespace genericComms;

class TformEditFormat : public TForm
{
__published:	// IDE-managed Components
    TOpenDialog *openDialogChooseFile;
	TGroupBox *groupboxResultSet;
	TGroupBox *groupboxGeneral;
	TPanel *panelSaveCancel;
	TButton *btnOK;
	TButton *btnCancel;
	TTimer *timerKey;
	TPanel *panelDelimiters;
	TGroupBox *GroupBox12;
	TEdit *editRowBegin;
	TUpDown *UpDown1;
	TGroupBox *GroupBox13;
	TComboBox *comboDelim;
	TEdit *editDelim;
	TGroupBox *GroupBox14;
	TComboBox *comboSubDelim;
	TEdit *editSubDelim;
	TGroupBox *GroupBox17;
	TComboBox *comboEscape;
	TEdit *editEscape;
	TPanel *panelTagVal;
	TGroupBox *GroupBox3;
	TComboBox *comboDataType;
	TGroupBox *GroupBox4;
	TEdit *editTag;
	TGroupBox *GroupBox5;
	TUpDown *upDownTagCol;
	TEdit *editTagCol;
	TGroupBox *GroupBox6;
	TUpDown *upDownValCol;
	TEdit *editValCol;
	TGroupBox *GroupBox7;
	TUpDown *upDownSubCol;
	TEdit *editValSubCol;
	TGroupBox *GroupBoxExample;
	TEdit *editExample;
	TGroupBox *GroupBoxButtons;
	TButton *btnAdd;
	TButton *btnSave;
	TButton *btnRemove;
	TPanel *Panel1;
	TTreeView *treeviewParse;
	TPanel *panelNameDesc;
	TGroupBox *GroupBox16;
	TMemo *memoDesc;
	TPanel *panelNameAndExample;
	TPanel *Panel2;
	TGroupBox *GroupBox8;
	TGroupBox *GroupBox10;
	TEdit *editName;
	TGroupBox *GroupBox1;
	TEdit *editFile;
	TButton *btnBrowse;
	TPopupMenu *popupMenuActions;
	TMenuItem *menuItemDelete;
	TGroupBox *GroupBox2;
	TEdit *editQuote;
	TComboBox *comboQuote;
	TCheckBox *checkBoxCheck;
	TImageList *imageListTreeView;
	TMenuItem *N1;
	TActionList *actionListActions;
	TGroupBox *GroupBox9;
	TEdit *editOffset;
	TUpDown *upDownOffset;
	TGroupBox *GroupBox11;
	TUpDown *upDownLength;
	TEdit *editValLength;
	void __fastcall FormShow(TObject *Sender);
	void __fastcall btnOKClick(TObject *Sender);
	void __fastcall FormCreate(TObject *Sender);
	void __fastcall FormClose(TObject *Sender, TCloseAction &Action);
	void __fastcall FormDestroy(TObject *Sender);
	void __fastcall btnAddClick(TObject *Sender);
	void __fastcall btnRemoveClick(TObject *Sender);
	void __fastcall comboSubDelimChange(TObject *Sender);
	void __fastcall btnBrowseClick(TObject *Sender);
	void __fastcall comboDelimChange(TObject *Sender);
	void __fastcall editValSubColChange(TObject *Sender);
	void __fastcall editTagChange(TObject *Sender);
	void __fastcall editTagColChange(TObject *Sender);
	void __fastcall editValColChange(TObject *Sender);
	void __fastcall comboEscapeChange(TObject *Sender);
	void __fastcall btnCancelClick(TObject *Sender);
	void __fastcall checkBoxCheckClick(TObject *Sender);
	void __fastcall comboDataTypeChange(TObject *Sender);
	void __fastcall timerKeyTimer(TObject *Sender);
	void __fastcall editValLengthChange(TObject *Sender);
	void __fastcall menuItemDeleteClick(TObject *Sender);
	void __fastcall comboQuoteChange(TObject *Sender);
	void __fastcall treeviewParseClick(TObject *Sender);
	void __fastcall treeviewParseDragDrop(TObject *Sender, TObject *Source, int X, int Y);
	void __fastcall treeviewParseGetSelectedIndex(TObject *Sender, TTreeNode *Node);
	void __fastcall menuItemClick(TObject *Sender);
	void __fastcall treeviewParseDragOver(TObject *Sender, TObject *Source, int X,
          int Y, TDragState State, bool &Accept);
	void __fastcall treeviewParseCompare(TObject *Sender, TTreeNode *Node1,
          TTreeNode *Node2, int Data, int &Compare);
	void __fastcall editOffsetChange(TObject *Sender);
	void __fastcall btnSaveClick(TObject *Sender);
private:	// User declarations
	void setDefaults();
	void errorAddDatum(Datum::DatumType addType, Datum::DatumType selectedType);

    // db interaction
    void loadItems();
    void saveFields(int filetype);
    void validateAndSave();

	// parsing file
	bool findMatch(
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
		const string quote);
	void findExample();
	void parseLine();
	void attributeChanged() ;
	TTreeNode * addDatum(Datum * add, TTreeNode * to);
	void itemAdd(TObject *Sender, Datum::DatumType addType);
	short lastCol;
	FileFormat *format;
	TStringList *lines;
	String tmp;
	FormatType inputOrOutput;
public:		// User declarations
    __fastcall TformEditFormat(TComponent* Owner);
    void clearFields();
	void setFormat(FileFormat *);
	void setFormatType(FormatType t);
};
//---------------------------------------------------------------------------
extern PACKAGE TformEditFormat *formEditFormat;
//---------------------------------------------------------------------------
#endif

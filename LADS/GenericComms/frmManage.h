//---------------------------------------------------------------------------

#ifndef frmManageH
#define frmManageH
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
#include <DB.hpp>
#include <DBTables.hpp>
#include <ComCtrls.hpp>
#include <ExtCtrls.hpp>
#include <vector>
#include "gcommsObjects.h"
//---------------------------------------------------------------------------
//class FileFormat {
//public:
//    int         id;
//    String      name;
//    String      description;
//    int         type;
//    int         firstrow;
//    char        delim;
//    char        subdelim;
//    String      dateformat;
//    int         status;
//    FileFormat() : id(-1), name("undef"), description("undef"), type(-1),
//        firstrow(-1), delim('\0'), subdelim('\0'), dateformat("undef"), status(-1) {};
//    FileFormat(int _id, String _name, String _desc, int _type, int _firstrow,
//            String _delim, String _subdelim, String _dateformat, int _status) :
//            id(_id), name(_name), description(_desc), type(_type), firstrow(_firstrow),
//            status(_status) {
//                if (_delim.IsEmpty()) {
//                    String tmp;
//                    tmp.sprintf("Null delimiter in format '%s'!", _delim);
//                    throw tmp;
//                }
//                delim       = _delim[1];
//                subdelim    = _subdelim.IsEmpty() ? '\0' : _subdelim[1];
//                dateformat  = _dateformat;
//    }
//};
//typedef FileFormat* pFileFormat;
//typedef std::vector< pFileFormat > VecpFileFormat;
//
//class FormatMap {
//    int         id_format;
//    int         id_cluster;
//    int         status;
//};
//typedef FormatMap* pFormatMap;
//typedef std::vector< pFormatMap > VecpFormatMap;

using namespace genericComms;

class TformManage : public TForm
{
__published:	// IDE-managed Components
    TStatusBar *StatusBar1;
	TQuery *Query1;
	TGroupBox *groupboxAnalysers;
	TListBox *listboxAnalysers;
	TPanel *panelFormats;
	TGroupBox *groupboxInputFormats;
	TGroupBox *GroupBox5;
	TListBox *listboxInputAvailable;
	TGroupBox *GroupBox4;
	TListBox *listboxInputAssigned;
	TGroupBox *groupboxOutputFormats;
	TGroupBox *GroupBox3;
	TListBox *listboxOutputAvailable;
	TGroupBox *groupBoxOutputAssigned;
	TListBox *listboxOutputAssigned;
	TPanel *panelOutputActions;
	TButton *btnOutputAdd;
	TButton *btnOutputRemove;
	TButton *btnOutputNew;
	TButton *btnOutputEdit;
	TButton *btnOutputDel;
	TPanel *panelInputActions;
	TButton *btnInputAdd;
	TButton *btnInputRemove;
	TButton *btnInputNew;
	TButton *btnInputEdit;
	TButton *btnInputDel;
    void __fastcall FormShow(TObject *Sender);
    void __fastcall listboxAnalysersClick(TObject *Sender);
    void __fastcall btnOutputNewClick(TObject *Sender);
    void __fastcall btnOutputEditClick(TObject *Sender);
    void __fastcall btnOutputDelClick(TObject *Sender);
    void __fastcall btnOutputAddClick(TObject *Sender);
    void __fastcall btnOutputRemoveClick(TObject *Sender);
    void __fastcall listboxOutputAvailableDblClick(TObject *Sender);
	void __fastcall btnInputAddClick(TObject *Sender);
	void __fastcall btnInputRemoveClick(TObject *Sender);
	void __fastcall btnInputNewClick(TObject *Sender);
	void __fastcall btnInputEditClick(TObject *Sender);
	void __fastcall btnInputDelClick(TObject *Sender);
	void __fastcall listboxInputAvailableDblClick(TObject *Sender);
	void __fastcall FormCreate(TObject *Sender);
private:	// User declarations
	void refreshInputAvailable();
	void refreshOutputAvailable();
	void refreshInputAssigned();
	void refreshOutputAssigned();
	void refreshAnalysers();
	void refreshAssigned(FormatType t);
	void refreshAvailable(FormatType t);
	void refreshAll();
	void fnEditFormat(FormatType t);
	void fnDelFormat(FormatType t);
	void fnAssignFormat(FormatType t);
	void fnDeassignFormat(FormatType t);
	void fnNewFormat(FormatType t);
public:		// User declarations
	__fastcall TformManage(TComponent* Owner);
};
//---------------------------------------------------------------------------
extern PACKAGE TformManage *formManage;
//---------------------------------------------------------------------------
#endif

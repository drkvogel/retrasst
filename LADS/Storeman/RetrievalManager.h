//---------------------------------------------------------------------------

#ifndef RetrievalManagerH
#define RetrievalManagerH
//---------------------------------------------------------------------------
#include <System.Classes.hpp>
#include <Vcl.Controls.hpp>
#include <Vcl.StdCtrls.hpp>
#include <Vcl.Forms.hpp>
#include <Vcl.ExtCtrls.hpp>
#include <Vcl.Grids.hpp>
//---------------------------------------------------------------------------
class TfrmRetrievalManager : public TForm
{
__published:	// IDE-managed Components
    TGroupBox *groupList;
    TPanel *Panel1;
    TButton *btnSave;
    TButton *btnCancel;
    TSplitter *Splitter1;
    TPanel *Panel2;
    TButton *btnAddChunk;
    TButton *btnDelChunk;
    TStringGrid *sgChunkList;
    TGroupBox *GroupBox1;
    TStringGrid *sgChunk;
    TButton *btnSaveChunk;
    void __fastcall FormShow(TObject *Sender);
private:	// User declarations
public:		// User declarations
    __fastcall TfrmRetrievalManager(TComponent* Owner);
};
//---------------------------------------------------------------------------
extern PACKAGE TfrmRetrievalManager *frmRetrievalManager;
//---------------------------------------------------------------------------
#endif

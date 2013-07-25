#ifndef RetrievalManagerH
#define RetrievalManagerH
#include <System.Classes.hpp>
#include <Vcl.Controls.hpp>
#include <Vcl.ExtCtrls.hpp>
#include <Vcl.Grids.hpp>
#include <Vcl.StdCtrls.hpp>
#include <sstream>
#include <System.Classes.hpp>
#include <Vcl.Controls.hpp>
#include <Vcl.StdCtrls.hpp>
#include <Vcl.Forms.hpp>
#include <Vcl.ExtCtrls.hpp>
#include <Vcl.Grids.hpp>
#include "LCDbJob.h"

class Chunk {

};


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
    TLabel *Label1;
    void __fastcall FormShow(TObject *Sender);
    void __fastcall FormCreate(TObject *Sender);
    void __fastcall btnCancelClick(TObject *Sender);
private:
    void autoChunk();
public:
    bool autochunk;
    LCDbCryoJob::JobKind jobType;
    __fastcall TfrmRetrievalManager(TComponent* Owner);
};

extern PACKAGE TfrmRetrievalManager *frmRetrievalManager;
#endif

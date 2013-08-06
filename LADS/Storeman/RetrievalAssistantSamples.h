#ifndef RetrievalAssistantSamplesH
#define RetrievalAssistantSamplesH
#include <System.Classes.hpp>
#include <Vcl.Controls.hpp>
#include <Vcl.StdCtrls.hpp>
#include <Vcl.Forms.hpp>
#include <Vcl.ExtCtrls.hpp>
#include <Vcl.Grids.hpp>
#include <sstream>
#include "RetrievalAssistant.h"
#include "LCDbJob.h"
#include "LPDbCryovialStore.h"
#include "LPDbCryovial.h"

//class Chunk { // not recorded in database
//public:
//    //Chunk() : section(0), retrieval_cid(0), name(""), start(0), end(0) { }
//    Chunk() : section(0), start(0), end(0) { }
//    int         section;
//    //int         retrieval_cid;
//    //int         exercise_cid;
//    //std::string name;
//    int         start;
//    int         end;
//    //string      descrip;
//    //int         job_type;
//    //int         project_cid;
//    //int         primary_aliquot;
//};
//
//typedef std::vector< Chunk * >  vecpChunk;

typedef std::vector< LPDbCryovialStore *> vecpVial;

class TfrmSamples : public TForm
{
__published:	// IDE-managed Components
    TSplitter *Splitter1;
    TGroupBox *groupList;
    TPanel *Panel2;
    TButton *btnAddChunk;
    TButton *btnDelChunk;
    TButton *btnSaveChunk;
    TButton *btnIncr;
    TButton *btnDecr;
    TStringGrid *sgChunks;
    TPanel *Panel1;
    TButton *btnSave;
    TButton *btnCancel;
    TCheckBox *cbLog;
    TRadioGroup *radgrpRows;
    TRadioButton *radbutAll;
    TRadioButton *radbutDefault;
    TRadioButton *radbutCustom;
    TEdit *editCustomRows;
    TGroupBox *GroupBox1;
    TStringGrid *sgChunk;
    TMemo *memoDebug;
    TTimer *timerCustomRows;
    void __fastcall FormCreate(TObject *Sender);
    void __fastcall FormShow(TObject *Sender);
    void __fastcall sgChunksDrawCell(TObject *Sender, int ACol, int ARow, TRect &Rect, TGridDrawState State);
    void __fastcall btnSaveClick(TObject *Sender);
    void __fastcall btnAddChunkClick(TObject *Sender);
    void __fastcall cbLogClick(TObject *Sender);
    void __fastcall btnDelChunkClick(TObject *Sender);
    void __fastcall radbutDefaultClick(TObject *Sender);
    void __fastcall radbutAllClick(TObject *Sender);
    void __fastcall radbutCustomClick(TObject *Sender);
    void __fastcall timerCustomRowsTimer(TObject *Sender);
    void __fastcall editCustomRowsChange(TObject *Sender);
    void __fastcall btnCancelClick(TObject *Sender);
private:	// User declarations
    LCDbCryoJob * job;
    int                 numrows; // rows to show at a time
    //vecpRetrievalPlan plans;
    vecpChunk           chunks;
    vecpVial            vials;
    void                autoChunk();
    void                showChunks();
    void                loadRows();
    void                showRows();
    void                radgrpRowsChange();
public:		// User declarations
    __fastcall          TfrmSamples(TComponent* Owner);
    void                setJob(LCDbCryoJob * ajob) { job = ajob; }
    bool                autochunk;
};

extern PACKAGE TfrmSamples *frmSamples;
#endif

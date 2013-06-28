//---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop

#include "frmCommon.h"
#include "fmMain.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"
TfmCommon *fmCommon;

__fastcall TfmCommon::TfmCommon(TComponent* Owner)
    : TForm(Owner)
{
}

void __fastcall TfmCommon::FormCreate(TObject *Sender)
{
//xxx    String TimeDateString;
//
//    String AppRoot = ExtractFilePath(ParamStr(0));
//    DateTimeToString(TimeDateString, "yyyy-mm-dd_hh-ss", Now());
//    String LogFileName = AppRoot+Application->Title+TimeDateString+".log";

//xxx    logfile.open(LogFileName.c_str());
//    if (!logfile.is_open()) {
//        APPMSG( "Error opening logfile "+LogFileName, "Error", MB_OK);
//    }

        //Visible = true;
}

void TfmCommon::ShowText(AnsiString text)
{
    MemoDebug->Lines->Add(text);
}

void TfmCommon::LogError(String text, String source)
{
    String strTimeDateString;

    DateTimeToString(strTimeDateString, "yyyy-mm-dd", Now());

    String ErrorText = strTimeDateString + ": " + text + " (" + source + ")";
    MemoErr->Lines->Add(ErrorText);

//xxx    if (logfile.is_open()) {
//        logfile << ErrorText.c_str() << std::endl;
//        logfile.flush();
//    }
}

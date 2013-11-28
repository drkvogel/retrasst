#include <vcl.h>
#pragma hdrstop

#include <string>
#include "DiscardSearch.h"

#pragma package(smart_init)
#pragma resource "*.dfm"

TfrmSearch *frmSearch;

void TfrmSearch::init( Discard::Context * context ) {
    if (context == 0) {
        std::string message = "";
        message += "null context";
        message += " at ";
        message += HERE;
        throw Exception(message.c_str());
    }
    m_context = context;

    m_stepno = -1;

    this->ediText->Text = "";

    //this->rgType->Controls[3]->Enabled = false; // crash!
    //this->rgType->Enabled = false; // maybe?

    this->cmbMenu->Items->Clear();

    if (m_context->isSelectJobStage()) {
        this->cmbMenu->Enabled = false;
        m_jobnos = m_context->getJobnos();
        for (IntVec::const_iterator it = m_jobnos.begin(); it != m_jobnos.end(); it++) {
            const int jobno = *it;
            const std::string jobPrompt = m_context->calcJobPrompt(jobno);
            this->cmbMenu->Items->Add(jobPrompt.c_str());
            this->cmbMenu->Enabled = true;
        }
    }

    if (Discard::Person::canSearch()) {
        this->rgType->Controls[3]->Enabled = true;
    }

    if (m_context->isCreateJobStage()) {
        gotoStep1();
    } else {
        gotoStep3();
    }

    return;
}

void TfrmSearch::gotoStep1() {
    m_stepno = 1;
    updateUI();
    return;
}

void
TfrmSearch::gotoStep2( ) {
    m_stepno = 2;
    updateUI();
    return;
}

void
TfrmSearch::gotoStep3( ) {
    m_stepno = 3;
    updateUI();
    return;
}

void
TfrmSearch::updateUI( ) {
    do {
        if (m_stepno == 1) {
            updateUIStep1();
            break;
        }
        if (m_stepno == 2) {
            updateUIStep2();
            break;
        }
        if (m_stepno == 3) {
            updateUIStep3();
            break;
        }

    } while (false);

    return;
}

void
TfrmSearch::updateUIStep1( ) {
    this->Caption = "Search Step 1 of 3";

    this->pnlStep1->Visible = false;
    this->pnlStep2->Visible = false;
    this->pnlStep3a->Visible = false;
    this->pnlStep3b->Visible = false;
    this->pnlStep3c->Visible = false;
    this->pnlStep1->Visible = true;

    this->btnNext->Caption = "Next";

    this->btnBack->Enabled = false;

    this->btnNext->Enabled = false;
    if (this->rgType->ItemIndex >= 0) {
        this->btnNext->Enabled = true;
    }
    return;
}

void
TfrmSearch::updateUIStep2( ) {
    this->Caption = "Search Step 2 of 3";

    this->pnlStep1->Visible = false;
    this->pnlStep2->Visible = false;
    this->pnlStep3a->Visible = false;
    this->pnlStep3b->Visible = false;
    this->pnlStep3c->Visible = false;
    this->pnlStep2->Visible = true;

    this->btnNext->Caption = "Next";

    this->btnBack->Enabled = true;

    this->btnNext->Enabled = false;
    if (this->rgSource->ItemIndex >= 0) {
        this->btnNext->Enabled = true;
    }

    return;
}

void
TfrmSearch::updateUIStep3( ) {
    this->pnlStep1->Visible = false;
    this->pnlStep2->Visible = false;

    if (m_context->isSelectJobStage()) {
        this->Caption = "Choose job";
        this->pnlStep3a->Visible = false;
        this->pnlStep3b->Visible = true;
        this->pnlStep3c->Visible = false;
        this->btnNext->Caption = "OK";
        this->btnBack->Enabled = false;
        this->btnBack->Visible = false;
    } else {
        this->Caption = "Search Step 3 of 3";
        this->pnlStep3a->Visible = (this->rgSource->ItemIndex == 0);
        this->pnlStep3b->Visible = false;
        this->pnlStep3c->Visible = (this->rgSource->ItemIndex == 1);
        this->btnNext->Caption = "Finish";
        this->btnBack->Enabled = true;
    }

    this->btnNext->Enabled = false;
    if (this->pnlStep3a->Visible) {
        if (this->ediText->Text != "") {
            this->btnNext->Enabled = true;
        }
    } else if (this->pnlStep3b->Visible) {
        if (this->cmbMenu->Text != "") {
            this->btnNext->Enabled = true;
        }
    } else if (this->pnlStep3c->Visible) {
        if (this->ediFile->Text != "") {
            this->btnNext->Enabled = true;
        }
    } else {
        std::string message = "";
        message += "internal error";
        message += " at ";
        message += HERE;
        throw Exception(message.c_str());
    }

    return;
}

// end of user code


//---------------------------------------------------------------------------
__fastcall TfrmSearch::TfrmSearch(TComponent* Owner)
    : TForm(Owner) {
}
//---------------------------------------------------------------------------

void __fastcall TfrmSearch::btnCancelClick(TObject *Sender) {
    int mr = mrNone;

    do {
        mr = mrCancel;

    } while (false);

    this->ModalResult = mr;
}
//---------------------------------------------------------------------------

void __fastcall TfrmSearch::btnBackClick(TObject *Sender) {
    do {
        if (m_stepno == 2) {
            gotoStep1();
            break;
        }
        if (m_stepno == 3) {
            gotoStep2();
            break;
        }

    } while (false);
}
//---------------------------------------------------------------------------

void __fastcall TfrmSearch::btnNextClick(TObject *Sender) {
    do {
        if (m_stepno == 1) {
            gotoStep2();
            break;
        }

        if (m_stepno == 2) {
            gotoStep3();
            break;
        }

        if (m_stepno == 3) {
            int mr = mrNone;

            do {
                AnsiString type;
                AnsiString source;
                AnsiString text;

                if (m_context->isSelectJobStage()) {
                    type = "Job";
                    source = "Text";
                    size_t index = this->cmbMenu->ItemIndex;
                    if (index < m_jobnos.size()) {
                        const int jobno = m_jobnos[index];
                        String title = "Job";
                        String message = m_context->calcJobDescription(jobno).c_str();
                        message += "\n";
                        message += m_context->calcJobStatus(jobno).c_str();
                        Application->MessageBox(message.c_str(),title.c_str(), MB_OK);
                        text = jobno;
                    }
                } else {
                    const int index = this->rgType->ItemIndex;
                    if (index < 0) {
                        break;
                    }


                    type =  this->rgType->Items->Strings[index] ;
                    if (this->rgSource->ItemIndex == 0) {
                        source = "Text";
                        text =  this->ediText->Text;
                    } else if (this->rgSource->ItemIndex == 1) {
                        source = "File";
                        text = this->ediFile->Text ;
                    }
                }

                if ((type == "") || (source == "") || (text == "")) {
                    break;
                }

                m_context->setSearchType(type.c_str());
                m_context->setSearchSource(source.c_str());
                m_context->configSearchTexts(text.c_str());

                mr = mrOk;
            } while (false);

            this->ModalResult = mr;
            break;
        }

    } while (false);
}
//---------------------------------------------------------------------------

void __fastcall TfrmSearch::rgTypeClick(TObject *Sender) {
    updateUI();
}
//---------------------------------------------------------------------------

void __fastcall TfrmSearch::btnBrowseClick(TObject *Sender) {
    frmSearch->SendToBack();
    if (this->openFile->Execute()) {
        AnsiString filename = this->openFile->FileName;
        this->ediFile->Text = filename.c_str();
        updateUI();
    }
    frmSearch->BringToFront();
}
//---------------------------------------------------------------------------

void __fastcall TfrmSearch::cmbMenuChange(TObject *Sender) {
    updateUI();
}
//---------------------------------------------------------------------------

void __fastcall TfrmSearch::ediTextKeyPress(TObject *Sender, char &Key) {
    const char c = Key;

    do {
        if (c == '\r') {
            btnNextClick(0);
        }

    } while (false);
}
//---------------------------------------------------------------------------


void __fastcall TfrmSearch::ediTextChange(TObject *Sender) {
    updateUI();
}
//---------------------------------------------------------------------------

void __fastcall TfrmSearch::rgSourceClick(TObject *Sender) {
    updateUI();
}
//---------------------------------------------------------------------------



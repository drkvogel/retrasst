//---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop

#include <algorithm>
#include "DebugCommsFrm.h"
#include "NephInputProcessor.h"
#include "SerialComms.h"
#include "isysMessage.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"
TdebugComms *debugComms;
//---------------------------------------------------------------------------
__fastcall TdebugComms::TdebugComms(TComponent* Owner)
    : TForm(Owner)
{
}
//---------------------------------------------------------------------------

void TdebugComms::sendMessage(String text)
{
    neph::ByteOutputBuffer buf;
    buf << SerialComms::STX
        << text.c_str()
        << SerialComms::CR
        << SerialComms::ETX;

    neph::ByteString bytes = buf.str();
    std::copy(bytes.begin(), bytes.end(), std::back_inserter(outBuffer));
}

void __fastcall TdebugComms::sendButtonClick(TObject *Sender)
{
    sendMessage(analyserOutput->Text);
    analyserOutput->Text = "";
    neph::ByteOutputBuffer buf;
    buf << NephInputProcessor::STX
        << analyserOutput->Text.c_str()
        << NephInputProcessor::CR
        << NephInputProcessor::ETX;

    neph::ByteString bytes = buf.str();
    std::copy( bytes.begin(), bytes.end(), std::back_inserter(outBuffer) );
    analyserOutput->Text = "";
}
//---------------------------------------------------------------------------
unsigned TdebugComms::available()
{
    return outBuffer.size();
}

unsigned TdebugComms::readBytes( unsigned char * buffer, unsigned count )
{
    int i = 0;
    int max = count > outBuffer.size() ? outBuffer.size() : count;
    neph::ByteString::iterator iter = outBuffer.begin();
    while ( i < max )
    {
        buffer[i] = *iter;
        ++i;
        ++iter;
    }

    std::stringstream sstr;
    std::string s;
    sstr << "-->" << neph::displayBytes((char*)outBuffer.c_str());
    receivedBytes->Lines->Add(sstr.str().c_str());
    neph::ByteString bytes;
    std::copy( iter, outBuffer.end(), std::back_inserter(bytes) );
    outBuffer = bytes;

    return i;
}

unsigned TdebugComms::sendBytes( const unsigned char * bytes, unsigned count )
{
    std::ostringstream out;
    NephBytes nephBytes;
    std::copy( bytes, bytes + count, std::back_inserter( nephBytes ) );
    out << nephBytes; // overloaded '<<' adds decorations for control chars
    String strTemp;  strTemp.sprintf("<--%s", out.str().c_str());
    receivedBytes->Lines->Add(strTemp);
    return count;
}

void __fastcall TdebugComms::btnSendACKClick(TObject *Sender)
{
    outBuffer.push_back(SerialComms::ACK);
}
//---------------------------------------------------------------------------

void __fastcall TdebugComms::btnSendNAKClick(TObject *Sender)
{
    outBuffer.push_back(SerialComms::NAK);
}
//---------------------------------------------------------------------------

void __fastcall TdebugComms::btnSendENQClick(TObject *Sender)
{
    outBuffer.push_back(SerialComms::ENQ);
}
//---------------------------------------------------------------------------

void TdebugComms::printMessage(String message) {
    //debugComms
    //StaticText1->Caption = message;
    //Memo1->Lines->Add(message.c_str());
    //Memo1->Lines->Add(message);

//     Memo1->Invalidate();
//    Memo1->Clear();
    Memo1->Text = message;
//    Application->ProcessMessages();
}

void TdebugComms::addMessage(String message) {
    //debugComms
    //StaticText1->Caption = message;
    //Memo1->Lines->Add(message.c_str());
    Memo1->Lines->Add(message);

//     Memo1->Invalidate();
//    Memo1->Clear();
    //Memo1->Text = message;
//    Application->ProcessMessages();
}

void __fastcall TdebugComms::btnSendHRecordClick(TObject *Sender)
{
    //outBuffer.push_back(std::string("1H|\^_|||||||||||2|20100826142841"));
    sendMessage("1H|\\^_||||||||||P|2|20100826142841");
}
//---------------------------------------------------------------------------

void __fastcall TdebugComms::btnSendQRecordClick(TObject *Sender)
{
	//sendMessage("2Q|1|11111^100^|^^|ALL|||||||F");
	String strTemp;
	strTemp.sprintf("2Q|1|^%s||ALL||||||||O", editBarcode->Text);
	sendMessage(strTemp);
}
//---------------------------------------------------------------------------

void __fastcall TdebugComms::btnSendLRecordClick(TObject *Sender)
{
    sendMessage("3L|1|N");
}
//---------------------------------------------------------------------------

void __fastcall TdebugComms::btnSendPRecordClick(TObject *Sender)
{
	String strTemp;
	//strTemp.sprintf("2P|1|^%s||ALL||||||||O", editBarcode->Text);
	sendMessage(strTemp);
}
//---------------------------------------------------------------------------

void __fastcall TdebugComms::btnSendORecordClick(TObject *Sender)
{
    String strTemp;
	//strTemp.sprintf("2Q|1|^%s||ALL||||||||O", editBarcode->Text);
    IsysMessage * msg;
    msg = IsysMessage::createORecord(
        editOContainer->Text,
        editOSID->Text,
        editOManu->Text,
        editOPriority->Text[1],
        editOAction->Text[1],
        editOSampleType->Text[1],
        editOReportType->Text[1]);
	sendMessage(msg->toString());
    delete msg;
}
//---------------------------------------------------------------------------

void __fastcall TdebugComms::btnSendRRecordClick(TObject *Sender)
{
	String strTemp;
	//strTemp.sprintf("2Q|1|^%s||ALL||||||||O", editBarcode->Text);
	sendMessage(strTemp);
}
//---------------------------------------------------------------------------


void __fastcall TdebugComms::btnSendEOTClick(TObject *Sender)
{
    outBuffer.push_back(SerialComms::EOT);
}
//---------------------------------------------------------------------------


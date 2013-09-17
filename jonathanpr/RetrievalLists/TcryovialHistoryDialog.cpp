//---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop

#include "TcryovialHistoryDialog.h"
#include "Utils.h"
#include <sstream>
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"
TcryovialHistoryDialog *cryovialHistoryDialog;
//---------------------------------------------------------------------------
__fastcall TcryovialHistoryDialog::TcryovialHistoryDialog(TComponent* Owner)
	: TForm(Owner)
{
}
//-------------------------------------------------------------------------
void TcryovialHistoryDialog::setup(retrievalList *pList,std::map<String,String> &cryovialInfo)
{
	m_pRetrievalList = pList;

	try
	{
		m_pRetrievalList->fillRichEditInfo(cryovialInfo,RichEdit);

		//update title.
		std::wstringstream sstitle;
		sstitle << L"Cryovial history ";// << results.size();
		this->SetTextBuf(String(sstitle.str().c_str()).c_str());
	}
	catch( const String& msg )
	{
		String displayString = String("Failed - \n\n Reason :\n") + msg;
		Application->MessageBox(UnicodeString(displayString.c_str()).c_str(), L"User Error", MB_OK);
	}
}



//---------------------------------------------------------------------------

#ifndef Unit1H
#define Unit1H
//---------------------------------------------------------------------------
#include <System.Classes.hpp>
#include <FMX.Controls.hpp>
#include <FMX.Forms.hpp>
#include <FMX.ListView.hpp>
#include <FMX.ListView.Types.hpp>
#include <FMX.Types.hpp>
//---------------------------------------------------------------------------

#include "AcquireCriticalSection.h"
#include "CritSec.h"
#include <deque>
#include <string>

namespace stef
{
	class ThreadPool;
};

class LogMsgQ
{
public:
	void add( const std::string& msg );
	std::string popMsg();
private:
	paulst::CritSec m_cs;
	std::deque< std::string > m_queue;
};

class TForm1 : public TForm
{
__published:	// IDE-managed Components
	TListView *ListView1;
	void __fastcall onCreate(TObject *Sender);
	void __fastcall onClose(TObject *Sender, TCloseAction &Action);
private:	// User declarations
	stef::ThreadPool* 	m_threadPool;
	LogMsgQ 			m_logMsgQ;
public:		// User declarations
	__fastcall TForm1(TComponent* Owner);
	__fastcall void appendLogMsg();
};
//---------------------------------------------------------------------------
extern PACKAGE TForm1 *Form1;
//---------------------------------------------------------------------------
#endif

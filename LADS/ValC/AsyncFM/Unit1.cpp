//---------------------------------------------------------------------------

#include <fmx.h>
#pragma hdrstop


#include "StrUtil.h"
#include "Task.h"
#include "ThreadPool.h"
#include "Unit1.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.fmx"
TForm1 *Form1;


void LogMsgQ::add( const std::string& msg )
{
	paulst::AcquireCriticalSection a(m_cs);
	m_queue.push_back( msg );
}

std::string LogMsgQ::popMsg()
{
	paulst::AcquireCriticalSection a(m_cs);
	std::string msg = m_queue.front();
	m_queue.pop_front();
	return msg;
}

const int THREADPOOL_SHUTDOWN_TIMEOUT_SECS = 10;

class LazyUpdate : public stef::Task
{
private:
	TThreadMethod 	m_appendToListView;
	std::string 	m_msg;
	LogMsgQ* 		m_logMsgQ;
	long			m_sleepTime;
public:
	LazyUpdate(
		const std::string& 	s,
		TThreadMethod 		appendToListView,
		LogMsgQ* 			logMsgQ,
		long				sleepTime )
		:
		m_appendToListView(appendToListView),
		m_msg(s),
		m_logMsgQ( logMsgQ ),
		m_sleepTime( sleepTime )
	{
	}
protected:
	void doStuff()
	{
		Sleep( m_sleepTime );
		m_logMsgQ->add( m_msg );
		TThread::Queue( NULL, m_appendToListView );
	}
};

//---------------------------------------------------------------------------
__fastcall TForm1::TForm1(TComponent* Owner)
	: TForm(Owner), m_threadPool( new stef::ThreadPool( 1, 1 ) )
{
}
//---------------------------------------------------------------------------
void __fastcall TForm1::onCreate(TObject *Sender)
{
	TThreadMethod m;


	for ( int i = 0; i < 10; ++i )
	{
		m_threadPool->addTask(
			new LazyUpdate(
				paulst::toString(i),
				appendLogMsg,
				&m_logMsgQ,
				1000
				)
			);
	}
}
//---------------------------------------------------------------------------
void __fastcall TForm1::onClose(TObject *Sender, TCloseAction &Action)
{
	m_threadPool->shutdown( THREADPOOL_SHUTDOWN_TIMEOUT_SECS * 1000, true );
}
//---------------------------------------------------------------------------
void __fastcall TForm1::appendLogMsg()
{
	std::string s = m_logMsgQ.popMsg();
    TListViewItem* newItem = ListView1->Items->Add();
	newItem->Text = UnicodeString( s.c_str() );
}

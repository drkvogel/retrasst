//---------------------------------------------------------------------------

#include <fmx.h>
#pragma hdrstop

#include "TMainForm.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.fmx"

#include <sstream>
#include "ThreadPool.h"
#include "UserAdvisorPanel.h"

TMainForm *MainForm;

class UserAdvisorTask : public stef::Task
{
public:
	UserAdvisorTask( const std::string& msg, valc::UserAdvisor* ua, int sleepMillis )
        :
        m_msg(msg),
        m_userAdvisor( ua ),
        m_sleepMillis( sleepMillis )
    {
    }

	void doStuff()
    {
        Sleep( m_sleepMillis );
        m_userAdvisor->advise( m_msg );
    }
private:
    const int m_sleepMillis;
    valc::UserAdvisor* const m_userAdvisor;
    const std::string m_msg;
};

//---------------------------------------------------------------------------
__fastcall TMainForm::TMainForm(TComponent* Owner)
	:
	TForm(Owner),
	userAdvisorPanel(userAdvisorContainer, alarmOn, alarmOff, "Warnings", &incomingMessages ),
    m_threadPool( new stef::ThreadPool( 0, 10 ) )
{
	alarmOff();

	userAdvisorContainer->AddObject(userAdvisorPanel.panel);

	for ( int i = 0; i < 20; ++i )
	{
		std::ostringstream msg;
		msg << i << ") " << "Your message here";
		m_threadPool->addTask(
			new UserAdvisorTask( msg.str(), &incomingMessages, i * 500 ) );
	}

	userAdvisorPanel.setFocus();
}
//---------------------------------------------------------------------------
__fastcall TMainForm::~TMainForm()
{
    m_threadPool->shutdown( 500, true );
}

//---------------------------------------------------------------------------
void __fastcall TMainForm::alarmOn()
{
	alarm->Opacity = 1;
}

void __fastcall TMainForm::alarmOff()
{
	alarm->Opacity = 0;
}



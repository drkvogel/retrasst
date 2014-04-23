#ifndef USERADVISORPANELH
#define USERADVISORPANELH

#include <FMX.Layouts.hpp>
#include <FMX.ListView.hpp>
#include <FMX.ListView.Types.hpp>
#include <FMX.Memo.hpp>
#include <FMX.StdCtrls.hpp>
#include <FMX.Types.hpp>
#include <string>
#include "UserAdvisorAdapter.h"

namespace valcui
{

/*
	With the exception of 'notifyNewMessage', the design assumption is
	that all the public methods are called on the UI thread.
*/
class UserAdvisorPanel
{
public:
    TPanel* panel;

    void setFocus();

    UserAdvisorPanel( 
        TControl* owner, 
        TThreadMethod soundAlarm, 
        TThreadMethod silenceAlarm, 
        const std::string& title,
        UserAdvisorAdapter<UserAdvisorPanel>* msgCache );

    __fastcall ~UserAdvisorPanel();
	void __fastcall acknowledgeSelected( TObject* sender );
    void __fastcall addPendingMessagesToListView();
    void            notifyNewMessage();
	void __fastcall onChange	( TObject *Sender );
	void __fastcall onKeyDown	( TObject *Sender, WORD &Key, System::WideChar &KeyChar, TShiftState Shift);
	void __fastcall selectAll	( TObject *Sender );
	void __fastcall selectNone	( TObject *Sender );
private:
    TListView* listView;
    TMemo*     messageViewer;
    TThreadMethod m_soundAlarm, m_silenceAlarm;
    bool m_alarmOn;
    UserAdvisorAdapter<UserAdvisorPanel>* m_msgCache;

    UserAdvisorPanel( const UserAdvisorPanel& );
    UserAdvisorPanel& operator=( const UserAdvisorPanel& );
	int deleteNextChecked( int startIndex );
    void updateMessageViewer();
};

}

#endif


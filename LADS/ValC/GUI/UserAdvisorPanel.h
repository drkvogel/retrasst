#ifndef USERADVISORPANELH
#define USERADVISORPANELH

#include <FMX.Layouts.hpp>
#include <FMX.ListView.hpp>
#include <FMX.ListView.Types.hpp>
#include <FMX.Memo.hpp>
#include <FMX.StdCtrls.hpp>
#include <FMX.Types.hpp>
#include "IdleServiceUserAdapter.h"
#include <string>
#include "UserAdvisorAdapter.h"

namespace valcui
{

class Model;

/*
	The design assumption is that all the public methods are called on the UI thread.
*/
class UserAdvisorPanel
{
public:
    TPanel* panel;

    void setFocus();

    UserAdvisorPanel( 
        TControl* owner, 
        Model* model,
        const std::string& title,
        UserAdvisorAdapter* msgCache,
        IdleService* idleService );

	void __fastcall acknowledgeSelected( TObject* sender );
    void            onIdle();
	void __fastcall onChange	( TObject *Sender );
	void __fastcall onKeyDown	( TObject *Sender, WORD &Key, System::WideChar &KeyChar, TShiftState Shift);
	void __fastcall selectAll	( TObject *Sender );
	void __fastcall selectNone	( TObject *Sender );
private:
    TListView* listView;
    TMemo*     messageViewer;
    Model*      m_model;
    bool m_alarmOn;
    UserAdvisorAdapter* m_msgCache;
    IdleServiceUserAdapter<UserAdvisorPanel> m_idleServiceUser;

    UserAdvisorPanel( const UserAdvisorPanel& );
    UserAdvisorPanel& operator=( const UserAdvisorPanel& );
    void addPendingMessagesToListView();
	int deleteNextChecked( int startIndex );
    void updateMessageViewer();
};

}

#endif


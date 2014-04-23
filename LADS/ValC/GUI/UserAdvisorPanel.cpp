#include <deque>
#include <iterator>
#include <memory>
#include "UserAdvisorPanel.h"

namespace valcui
{

void load( const UnicodeString& uContent, TMemo* dest )
{
    std::auto_ptr<TStringStream> stream( new TStringStream( uContent ) );
    dest->Lines->Clear();
	dest->Lines->LoadFromStream(stream.get());
}

UserAdvisorPanel::UserAdvisorPanel(
        TControl* owner, 
        TThreadMethod soundAlarm, 
        TThreadMethod silenceAlarm, 
        const std::string& title,
        UserAdvisorAdapter<UserAdvisorPanel>* msgCache )
    :
    listView(0),
    m_soundAlarm( soundAlarm ),
    m_silenceAlarm( silenceAlarm ),
    m_alarmOn( false ),
    m_msgCache( msgCache )
{
	panel = new TPanel( owner );
    owner->AddObject( panel );
	panel->Align = TAlignLayout::alClient;

    TFlowLayout* layout = new TFlowLayout( panel );
    panel->AddObject(layout);

    layout->Justify         = fjCenter;
    layout->JustifyLastLine = fjCenter;
    layout->VerticalGap     = 5;
    layout->HorizontalGap   = 5;
    layout->Align           = TAlignLayout::alClient;

    const int heightUnits = panel->Height / 10;
    // Add the components I'm trying to organise:
    TLabel* label = new TLabel   (layout);
    listView      = new TListView(layout);
    messageViewer = new TMemo    (layout);
	TButton* selectAllButton  = new TButton(layout);
	TButton* selectNoneButton = new TButton(layout);
	TButton* ackButton        = new TButton(layout);
	selectAllButton ->Text = L"Select All";
	selectNoneButton->Text = L"Select None";
	ackButton       ->Text = L"Acknowledge Selected";
	selectAllButton ->OnClick = selectAll;
	selectNoneButton->OnClick = selectNone;
	ackButton       ->OnClick = acknowledgeSelected;
	const int buttonWidth = ackButton->Width * 2;
	selectAllButton ->Width = buttonWidth;
	selectNoneButton->Width = buttonWidth;
	ackButton       ->Width = buttonWidth;
    label           ->Text = UnicodeString( title.c_str() );
    label           ->Width = panel->Width - 10;
    label           ->Height = 1 * heightUnits;
	listView->EditMode = false;
	listView->ShowSelection = true;
	listView->ItemAppearance->ItemEditAppearance = L"ListItemShowCheck";
	listView->ItemAppearance->ItemAppearance = L"ListItemShowCheck";
	listView->ShowSelection = true;
    listView->AllowSelection = true;
	listView->AlternatingColors = true;
	listView->OnChange = onChange;
    listView->OnKeyDown = onKeyDown;
    listView     ->Height = 7 * heightUnits;
	listView     ->Width  = 2 * ( panel->Width / 3 );
	messageViewer->Height = listView->Height;
	messageViewer->Width  = panel->Width - listView->Width - 10;
	messageViewer->ReadOnly = true;
    
    layout->AddObject(label);
	layout->AddObject( new TFlowLayoutBreak(layout) );
    layout->AddObject(listView);
    layout->AddObject(messageViewer);
    layout->AddObject( new TFlowLayoutBreak(layout) );
	layout->AddObject(selectAllButton);
	layout->AddObject(selectNoneButton);
	layout->AddObject(ackButton);

    m_msgCache->setObserver( this );
}

UserAdvisorPanel::~UserAdvisorPanel()
{
    m_msgCache->setObserver( NULL );
}

void __fastcall UserAdvisorPanel::acknowledgeSelected( TObject* sender )
{
	int i = 0;

	do
	{
		i = deleteNextChecked( i );
	}
	while ( i != -1 );

    updateMessageViewer();

    if ( 0 == listView->ItemCount )
    {
        m_alarmOn = false;
		TThread::Queue( NULL, m_silenceAlarm );
    }
}

void UserAdvisorPanel::notifyNewMessage()
{
    TThread::Queue( NULL, addPendingMessagesToListView );
}

void __fastcall UserAdvisorPanel::addPendingMessagesToListView()
{
    std::deque< std::string > pendingMessages;

    m_msgCache->drainCache( std::back_inserter( pendingMessages ) );

    while ( ! pendingMessages.empty() )
    {
        std::string msg = pendingMessages.front();

        pendingMessages.pop_front();

        TListViewItem* item = listView->Items->Add();

        item->Text = msg.c_str();
    }

    if ( ( ! listView->Selected ) && listView->ItemCount )
    {
        listView->Selected = listView->Items->Item[0];
    }

    if ( listView->ItemCount && ! m_alarmOn )
    {
        m_alarmOn = true;
		TThread::Queue( NULL, m_soundAlarm );
    }
}

int UserAdvisorPanel::deleteNextChecked( int startIndex )
{
	for ( int i = startIndex; i < listView->ItemCount; ++i )
	{
		TListViewItem* item = listView->Items->Item[i];
		if ( item->Checked )
		{
			listView->Items->Delete(i);
			return i;
		}
	}

	return -1;
}

void __fastcall UserAdvisorPanel::onChange(TObject *Sender)
{
	updateMessageViewer();
}
//---------------------------------------------------------------------------

void __fastcall UserAdvisorPanel::onKeyDown(TObject *Sender, WORD &Key, System::WideChar &KeyChar, TShiftState Shift)
{
	if( ( Key == vkReturn || KeyChar == L' ' ) && listView->Selected )
	{
		listView->Selected->Checked = ! listView->Selected->Checked;
	}
}

void __fastcall UserAdvisorPanel::selectAll( TObject *Sender )
{
	for ( int i = 0; i < listView->ItemCount; ++i )
	{
		TListViewItem* item = listView->Items->Item[i];
		item->Checked = true;
	}
}

void __fastcall UserAdvisorPanel::selectNone( TObject *Sender )
{
	for ( int i = 0; i < listView->ItemCount; ++i )
	{
		TListViewItem* item = listView->Items->Item[i];
		item->Checked = false;
	}
}

void UserAdvisorPanel::setFocus()
{
	listView->SetFocus();
}

void UserAdvisorPanel::updateMessageViewer()
{
	UnicodeString message;

	if ( listView->Selected )
	{
		message = listView->Selected->Text;
	}

	load( message, messageViewer );
}

}


//---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop

#include "TESTProject.h"
#include "passwordForm.h"
#include "NewMemberDialog.h"
#include "NewEventDialog.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"
TForm1 *Form1;
//---------------------------------------------------------------------------
__fastcall TForm1::TForm1(TComponent* Owner)
	: TForm(Owner)
{
}
//---------------------------------------------------------------------------

void __fastcall TForm1::FormShow(TObject *Sender)
{
	if (!m_events.hasPassword())
	{
		std::auto_ptr<TPassword> passwordForm = std::auto_ptr<TPassword>( new TPassword(this) );
		do
		{
			passwordForm->ShowModal();
			if (passwordForm->didUserQuit())
			{
				this->Close();
				return;
			}

			try
			{
				m_events.initalise(passwordForm->getPassword());
				//if we get to here, we have access to the database
				FillEventList();
				FillEventInfo();
				FillMemberList();
				return; // WE ARE SIGN IN
			}
			catch( const String& msg )
			{
				String displayString = String("Failed - \n\n Reason :\n") + msg;
				Application->MessageBox(UnicodeString(displayString.c_str()).c_str(), L"User Error", MB_OK);
			}
		}
		while(true);
	}
}
//---------------------------------------------------------------------------

void __fastcall TForm1::AddEventClick(TObject *Sender)
{
	m_events.test(Application);
}
//---------------------------------------------------------------------------


void TForm1::FillEventList()
{
	std::vector<String> eventslist;
	m_events.getEvents(eventslist);

	EventCombo->Clear();

	for (std::vector<String>::iterator it = eventslist.begin(); it != eventslist.end(); ++it)
	{
		String temp = *it;
		EventCombo->AddItem(temp,NULL);
	}
	if (eventslist.size() > 0)
	{
		EventCombo->ItemIndex = 0;
	}
}

void TForm1::FillMemberList()
{
	String currentEvent = EventCombo->Text;
	if (currentEvent.IsEmpty())
		return;

	std::vector<String> memberslist;
	m_events.getMembersNoInEvent(currentEvent,memberslist);

	MemberCombobox->Clear();

	for (std::vector<String>::iterator it = memberslist.begin(); it != memberslist.end(); ++it)
	{
		String temp = *it;
		MemberCombobox->AddItem(temp,NULL);
	}
	if (memberslist.size() > 0)
	{
		MemberCombobox->ItemIndex = 0;
	}
}

void TForm1::FillEventInfo()
{
//clear grid
	for (int I = 0; I < StringGrid->ColCount; I++)
		for (int J = 0; J < StringGrid->RowCount; J++)
			StringGrid->Cells[I][J] = "";

//fill grid
	String currentEvent = EventCombo->Text;

	std::vector<String> memberlist;
	m_events.getMembersForEvent(currentEvent,memberlist);

	StringGrid->RowCount = memberlist.size()+1;
	StringGrid->ColCount = 2;
	StringGrid->Cells[0][0] = "Name";
	StringGrid->Cells[1][0] = "Other Info";

	int counter = 1;
	for (std::vector<String>::iterator it = memberlist.begin(); it != memberlist.end(); ++it)
	{
	    String temp = *it;
		StringGrid->Cells[0][counter] = temp;
		counter++;
	}
}
void __fastcall TForm1::ComboChanged(TObject *Sender)
{
	FillEventInfo();
	FillMemberList();
}
//---------------------------------------------------------------------------

void __fastcall TForm1::Action1Execute(TObject *Sender)
{
	//new member diaglog box
	std::auto_ptr<TNewMemberDlg> memberForm = std::auto_ptr<TNewMemberDlg>( new TNewMemberDlg(this) );
	try
	{
		memberForm->ShowModal();
		if (memberForm->didUserQuit())
		{
			return;
		}
		String NewName = memberForm->getName();
		m_events.addMember(NewName);
		FillMemberList();
	}
	catch( const String& msg )
	{
		String displayString = String("Failed - \n\n Reason :\n") + msg;
		Application->MessageBox(UnicodeString(displayString.c_str()).c_str(), L"User Error", MB_OK);
	}

}
//---------------------------------------------------------------------------

void __fastcall TForm1::Action2Execute(TObject *Sender)
{
 //new event dialog box
	std::auto_ptr<TeventDialog> eventForm = std::auto_ptr<TeventDialog>( new TeventDialog(this) );
	try
	{
		eventForm->ShowModal();
		if (eventForm->didUserQuit())
		{
			return;
		}
		String NewName = eventForm->getEvenName();
		m_events.addEvent(NewName);
		FillEventList();
	}
	catch( const String& msg )
	{
		String displayString = String("Failed - \n\n Reason :\n") + msg;
		Application->MessageBox(UnicodeString(displayString.c_str()).c_str(), L"User Error", MB_OK);
	}
}
//---------------------------------------------------------------------------

void __fastcall TForm1::Button1Click(TObject *Sender)
{
//add member to event
	int selected = MemberCombobox->ItemIndex;
	if (selected == -1) {
		return;
	}
 //	String Name = MemberCombobox->Items[selected];

	String MemberName = MemberCombobox->Text;
	String currentEvent = EventCombo->Text;

	if (MemberName.IsEmpty())
		return;
	if (currentEvent.IsEmpty())
		return;
	try
	{
		m_events.addMemberToEvent(currentEvent,MemberName);
		FillEventInfo();
		FillMemberList();
	}
	catch( const String& msg )
	{
		String displayString = String("Failed - \n\n Reason :\n") + msg;
		Application->MessageBox(UnicodeString(displayString.c_str()).c_str(), L"User Error", MB_OK);
	}

}
//---------------------------------------------------------------------------



void __fastcall TForm1::StringGridSelectCell(TObject *Sender, int ACol, int ARow,
          bool &CanSelect)
{
 //clicked on grid.. if on a name, remove from event
	 if (ACol > 0) //if not on name
		return;

	String currentEvent = EventCombo->Text;
	if (currentEvent.IsEmpty())
		return;

	 String memberName = StringGrid->Cells[0][ARow];
	 if (memberName.IsEmpty())
		 return;
	try
	{
		m_events.removeMemberFromEvent(currentEvent,memberName);
		FillEventInfo();
		FillMemberList();
	}
	catch( const String& msg )
	{
		String displayString = String("Failed - \n\n Reason :\n") + msg;
		Application->MessageBox(UnicodeString(displayString.c_str()).c_str(), L"User Error", MB_OK);
	}
}
//---------------------------------------------------------------------------


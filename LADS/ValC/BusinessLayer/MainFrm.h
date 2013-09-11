//---------------------------------------------------------------------------

#ifndef MainFrmH
#define MainFrmH
//---------------------------------------------------------------------------
#include <System.Classes.hpp>
#include <FMX.Controls.hpp>
#include <FMX.Forms.hpp>
#include <FMX.Layouts.hpp>
#include <FMX.Memo.hpp>
#include <FMX.Types.hpp>
#include <FMX.StdCtrls.hpp>

#include "API.h"
//---------------------------------------------------------------------------
/*
	The main form, loaded at start-up.

	The 'onShow' method demonstrates using the API classes.
*/

template<class Client>
class UserAdvisorAdapter : public valc::UserAdvisor
{
public:
	UserAdvisorAdapter( Client * c ) : m_client(c) {}
	void advise( const std::string& msg ) { m_client->addWarning( msg ); }
private:
	Client* m_client;
};

class TMainForm : public TForm
{
__published:	// IDE-managed Components
	TMemo *output;
	TLabel *Label1;
	TMemo *warnings;
	void __fastcall onShow(TObject *Sender);
private:	// User declarations
	UserAdvisorAdapter<TMainForm> m_warningsListener;
public:		// User declarations
	__fastcall TMainForm(TComponent* Owner);

	void addWarning( const std::string& warning );

};
//---------------------------------------------------------------------------
extern PACKAGE TMainForm *MainForm;
//---------------------------------------------------------------------------
#endif

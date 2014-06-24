#include "MenuViewController.h"
#include "Model.h"
#include "ModelEventConstants.h"
#include "StrUtil.h"
#include "TMainForm.h"

namespace valcui
{

TMenuItem* find( TMenuItem* m, const UnicodeString& name )
{
	TMenuItem* found = NULL;

	if ( m->Text == name )
	{
		found = m;
	}
	else
	{
		for ( int i = 0; ( ! found ) && i < m->ItemsCount; ++i )
		{
			found = find( m->Items[i], name );
		}
	}

	return found;
}

TMenuItem* find( TMainMenu* m, const UnicodeString& name )
{
	TMenuItem* found = NULL;

	for ( int i = 0; ( ! found ) && i < m->ItemsCount; ++i )
	{
		TFmxObject* o = m->Items[i];
		TMenuItem* mi = dynamic_cast<TMenuItem*>(o);
		if ( mi )
		{
			found = find( mi, name );
		}
	}

	return found;
}



MenuViewController::MenuViewController(
    TMainMenu* menu,
    Model* model,
    TMainForm* mainForm )
    :
    m_mainForm( mainForm ),
    m_model( model ),
	m_menu( menu ),
	m_modelEventListenerAdapter(this)
{
	assignActionToMenuItem( actionClose, "Close" );
	assignActionToMenuItem( actionRunPendingUpdates, "Run pending updates..." );

	find( menu, "Run pending updates...")->Enabled = false;

	m_model->registerModelEventListener( &m_modelEventListenerAdapter );
}

void MenuViewController::assignActionToMenuItem( TNotifyEvent action, const char* menuText )
{
	TMenuItem* menuItem = find( m_menu, menuText );

	if ( menuItem )
	{
		menuItem->OnClick = action;
	}
	else
	{
		throw Exception(
			paulst::format(
				"Failed to find menu item called '%s'",
				menuText
				).c_str()
			);

	}
}

void __fastcall MenuViewController::actionClose( TObject* sender )
{
    m_mainForm->Close();
}

void __fastcall MenuViewController::actionRunPendingUpdates( TObject* sender )
{
	m_model->doRunPendingUpdates();
}

void MenuViewController::notify( int eventID, const EventData& eventData )
{
	if ( eventID == MODEL_EVENT::FORCE_RELOAD )
	{
		find( m_menu, "Run pending updates...")->Enabled = true;
	}

    if ( eventID == MODEL_EVENT::PROCESSED_PENDING_UPDATES )
    {
        ShowMessage( "Successfully processed pending updates." );
    }
}

}


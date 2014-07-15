#ifndef MENUVIEWCONTROLLERH
#define MENUVIEWCONTROLLERH

#include <FMX.Menus.hpp>
#include "ModelEventListenerAdapter.h"

class TMainForm;

namespace valcui
{

class Model;

class MenuViewController
{
public:
    MenuViewController(
        TMainMenu* menu,
        Model* model,
		TMainForm* mainForm );
	void __fastcall actionClose( TObject* sender );
	void __fastcall actionRunPendingUpdates( TObject* sender );
	void __fastcall actionView( TObject* sender );
	void notify( int eventID, const EventData& eventData );
private:
    TMainForm* m_mainForm;
    Model* m_model;
    TMainMenu* m_menu;
	ModelEventListenerAdapter<MenuViewController> m_modelEventListenerAdapter;

    MenuViewController( const MenuViewController& );
	MenuViewController& operator=( const MenuViewController& );

	void assignActionToMenuItem( TNotifyEvent action, const char* menuText );
};

}

#endif


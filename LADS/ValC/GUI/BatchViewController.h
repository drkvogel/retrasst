#ifndef BATCHVIEWCONTROLLERH
#define BATCHVIEWCONTROLLERH

#include <FMX.Layouts.hpp>
#include <FMX.StdCtrls.hpp>
#include "FMXTemplates.h"
#include "IdleServiceUserAdapter.h"
#include "ModelEventListenerAdapter.h"
#include "Properties.h"

class TBatchViewFrame;

namespace paulst
{
    class Config;
}

namespace valcui
{

class Model;

class BatchViewController
{
public:
    /*
    Refer to config-copies/config-gui.txt regarding the format of batchProfileStylesCSV.
    */
    BatchViewController( const paulst::Config* guiConfig );
    IdleServiceUser*    getIdleServiceUserInterface();
    ModelEventListener* getModelEventListenerInterface();
    void                init();
    void                notify( int eventID, const EventData& d );
    void                onIdle();
    void                onResize();
    void                setModel( Model* m );
    void                setView( TBatchViewFrame* view );
    void __fastcall     update();
    void __fastcall     updateWorklistEntrySelection( TObject* sender );

private:
    IdleServiceUserAdapter<BatchViewController>     m_idleServiceUser;
    ModelEventListenerAdapter<BatchViewController>  m_eventListener;
    TBatchViewFrame*                                m_view;
	Model*                                          m_model;
	FMXContainer<TScrollBox,TPanel>       			m_scrollBox;
    paulst::Properties                              m_profileStyles;

    static const int TPANEL_HEIGHT = 25;
    static const int TPANEL_WIDTH  = 75;
};

}

#endif


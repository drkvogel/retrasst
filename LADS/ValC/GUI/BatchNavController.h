#ifndef BATCHNAVCONTROLLERH
#define BATCHNAVCONTROLLERH

#include "IdleServiceUserAdapter.h"
#include <map>
#include "ModelEventListenerAdapter.h"

class TBatchNavFrame;

namespace valcui
{

class Model;


class BatchNavController
{
public:
    BatchNavController();
    IdleServiceUser*    getIdleServiceUserInterface();
    ModelEventListener* getModelEventListenerInterface();
	void                init();
	void __fastcall     onClick( TObject* sender );
    void                onIdle();
    void                onResize();
    void                notify( int modelEvent, const EventData& ed );
    void                setModel( Model* m );
    void                setView( TBatchNavFrame* view );
    void __fastcall     update();
private:
    IdleServiceUserAdapter<BatchNavController>      m_idleServiceUser;
    ModelEventListenerAdapter<BatchNavController>   m_eventListener;
    TBatchNavFrame*                                 m_view;
	Model*                                          m_model;
	int												m_selectedWorklistEntry;
	std::map<int,int>								m_worklistEntryOffsets;
};

}

#endif


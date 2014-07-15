#ifndef RULEVIEWCONTROLLERH
#define RULEVIEWCONTROLLERH

#include "IdleServiceUserAdapter.h"
#include "ModelEventListenerAdapter.h"

class TRuleFrame;

namespace valcui
{

class Model;

class RuleViewState;

class RuleViewController
{
public:
    RuleViewController();
    ~RuleViewController();
    IdleServiceUser*    getIdleServiceUserInterface();
    ModelEventListener* getModelEventListenerInterface();
    void                init();
    void                notify( int modelEvent, const EventData& data );
    void                onIdle();
    void                onResize();
    void                setModel( Model* m );
    void                setView( TRuleFrame* v );
private:
    IdleServiceUserAdapter<RuleViewController> m_idleServiceUser;
    ModelEventListenerAdapter<RuleViewController> m_eventListener;
    Model* m_model;
    TRuleFrame* m_view;
    RuleViewState* m_state;
};

}

#endif


#include "RuleViewController.h"
#include "RuleViewStateStart.h"

namespace valcui
{
RuleViewController::RuleViewController()
    :
    m_idleServiceUser(this),
    m_eventListener(this),
    m_model(0),
    m_view(0),
    m_state(0)
{
}

RuleViewController::~RuleViewController()
{
    delete m_state;
}

IdleServiceUser* RuleViewController::getIdleServiceUserInterface()
{
    return &m_idleServiceUser;
}

ModelEventListener* RuleViewController::getModelEventListenerInterface()
{
    return &m_eventListener;
}

void RuleViewController::init()
{
    m_state = new RuleViewStateStart();
    m_state->init( m_view, m_model, this  );
}

void RuleViewController::notify( int modelEvent, const EventData& data )
{
}

void RuleViewController::onIdle()
{
}

void RuleViewController::onResize()
{
}

void RuleViewController::setModel( Model* m )
{
    m_model = m;
}

void RuleViewController::setView( TRuleFrame* v )
{
    m_view = v;
}

}


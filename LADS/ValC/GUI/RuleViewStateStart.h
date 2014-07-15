#ifndef RULEVIEWSTATESTARTH
#define RULEVIEWSTATESTARTH

#include "ModelEventListenerAdapter.h"
#include "RuleViewState.h"

namespace valcui
{

class RuleViewStateStart : public RuleViewState
{
public:
	RuleViewStateStart();
	~RuleViewStateStart();
	void init( TRuleFrame* view, Model* model, RuleViewController* owner );
	void __fastcall loadRule( TObject* sender );
	void notify( int modelEvent, const EventData& data );
private:
	ModelEventListenerAdapter<RuleViewStateStart> m_eventListener;
    TRuleFrame* m_view;
    Model* m_model;
    RuleViewController* m_owner;
};

}

#endif



#ifndef RULEVIEWSTATEH
#define RULEVIEWSTATEH

class TRuleFrame;

namespace valcui
{

class Model;
class RuleViewController;

class RuleViewState
{
public:
    RuleViewState();
    virtual ~RuleViewState();
    virtual void init( TRuleFrame* view, Model* model, RuleViewController* owner ) = 0;
};

}

#endif


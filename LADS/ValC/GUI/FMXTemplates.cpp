#include "FMXTemplates.h"
#include "Require.h"


namespace valcui
{

void setText( TStyledControl* control, const char* textSubcomponentName, const std::string& text )
{
    TText* textSubcomponent = (TText*) control->FindStyleResource(textSubcomponentName);
    throwUnless(textSubcomponent);
    textSubcomponent->Text = text.c_str();
}

}


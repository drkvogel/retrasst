#include <boost/variant.hpp>
#include "ExceptionHandler.h"
#include <memory>
#include "Model.h"
#include "ModelEventConstants.h"
#include "RuleViewStateStart.h"
#include "StrUtil.h"
#include "TRuleFrame.h"

namespace valcui
{

std::string toString( TEdit* tt )
{
	return AnsiString( tt->Text.c_str() ).c_str();
}

RuleViewStateStart::RuleViewStateStart()
	:
	m_eventListener(this),
	m_view(0),
	m_model(0),
	m_owner(0)
{
}

RuleViewStateStart::~RuleViewStateStart()
{
	m_model->unregisterModelEventListener( &m_eventListener );
}

void RuleViewStateStart::init( TRuleFrame* view, Model* model, RuleViewController* owner )
{
    m_view = view;
    m_model = model;
	m_owner = owner;

	m_model->registerModelEventListener( &m_eventListener );

	m_view->controlPanelTabs->TabIndex = 0;
	m_view->tabStartTest     ->Text = L"";
	m_view->tabStartMachine  ->Text = L"";
	m_view->tabStartProject  ->Text = L"";
	m_view->editor->Text = L"";
	m_view->editor->Enabled = false;
	m_view->viewConfiguredRuleButton->OnClick = loadRule;
}

void __fastcall RuleViewStateStart::loadRule( TObject* sender )
{
	try
	{
		const int test 		= paulst::toInt( toString( m_view->tabStartTest    ) );
		const int machine 	= paulst::toInt( toString( m_view->tabStartMachine ) );
		const int project	= paulst::toInt( toString( m_view->tabStartProject ) );
		m_model->doLoadRule( test, machine, project );
	}
	catch( const Exception& e )
	{
		ExceptionHandler()(e);
	}
	catch( ... )
	{
		std::unique_ptr<Exception> e(
			new Exception( "Unspecified exception in RuleViewStateStart::loadRule" ) );

		ExceptionHandler()(*e);
	}
}

void RuleViewStateStart::notify( int eventID, const EventData& data )
{
    if ( eventID  == MODEL_EVENT::RULE_LOADED )
    {
        const std::string* rule = boost::get<std::string>(&data);
        
        require( rule );
        
        UnicodeString uContent( rule->c_str() );
        std::unique_ptr<TStringStream> stream( new TStringStream( uContent ) );
        m_view->editor->Lines->Clear();
        m_view->editor->Lines->LoadFromStream( stream.get() );
    }
}

}



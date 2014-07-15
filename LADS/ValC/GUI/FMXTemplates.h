#ifndef FMXTEMPLATESH
#define FMXTEMPLATESH

#include <System.Classes.hpp>
#include <FMX.Controls.hpp>
#include <FMX.Forms.hpp>
#include <FMX.Layouts.hpp>
#include <FMX.StdCtrls.hpp>
#include <FMX.Types.hpp>
#include <FMX.TabControl.hpp>
#include <FMX.TreeView.hpp>
#include "Require.h"
#include <string>
#include <vector>

/*
    Methods for working with FireMonkey components.
*/
namespace valcui
{

template<typename SubComponentClass>
SubComponentClass* addSubComponent( TFmxObject* container,
	TAlignLayout alignment = TAlignLayout::alClient )
{
	SubComponentClass* subComponent = new SubComponentClass( container );
	subComponent->Parent = container;
	if ( alignment != TAlignLayout::alNone )
	{
		subComponent->Align = alignment;
	}
	container->AddObject( subComponent );
	return subComponent;
}

template<typename Parent>
TTreeViewItem* addNodeUnder( Parent* p, const std::string& text = "" )
{
    TTreeViewItem* newNode = new TTreeViewItem( p );
    newNode->Parent = p;
    if ( ! text.empty() )
	newNode->Text = UnicodeString( text.c_str() );
    return newNode;
}

/*
    Looks for an instance of type T that is the named subcomponent of 'control'.

    Throws an Exception if such a subcomponent is not found.
*/
template<typename T>
T* findStyleResource( TStyledControl* control, const char* styleName )
{
    T* t = (T*) control->FindStyleResource( styleName );
    throwUnless( t );
    return t;
}

/*
Manages the addition and removal of subcomponents.
*/
template<typename Container, typename Component>
class FMXContainer
{
private:

	Container* 					m_container;
	std::vector< Component* > 	m_components;

	FMXContainer( const FMXContainer& );
	FMXContainer& operator=( const FMXContainer& );

public:
	FMXContainer()
		:
		m_container(NULL)
	{
	}

	Component* addComponent()
	{
		require( m_container );
		Component* c = new Component( m_container );
		m_container->AddObject( c );
		m_components.push_back( c );
		return c;
	}

	void clear()
	{
		while ( m_components.size() )
		{
			auto i = m_components.begin();
			Component* c = *i;
			m_components.erase(i);
			m_container->RemoveComponent( c );
			delete c;
		}
	}

	void setContainer( Container* c )
	{
		m_container = c;
	}
};

/*
    Looks for a TText subcomponent of control with the style name specified by 'textSubcomponentName' and
    assigns the value of text to the 'Text' property of this subcomponent.
*/
void setText( TStyledControl* control, const char* textSubcomponentName, const std::string& text );

}

#endif


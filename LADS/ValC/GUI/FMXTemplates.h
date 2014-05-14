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
#include <string>

namespace valcui
{

template<typename SubComponentClass>
SubComponentClass* addSubComponent( TFmxObject* container )
{
	SubComponentClass* subComponent = new SubComponentClass( container );
	subComponent->Parent = container;
	subComponent->Align = TAlignLayout::alClient;
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

}

#endif


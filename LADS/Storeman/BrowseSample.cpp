//---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop

#include "BrowseSample.h"
#include <memory>
#include "SampleMove.h"
#include "StoreUtil.h"
#include "FindSample.h"

//---------------------------------------------------------------------------

#pragma package(smart_init)
#pragma resource "*.dfm"
TfrmBrowse *frmBrowse;

//---------------------------------------------------------------------------

__fastcall TfrmBrowse::TfrmBrowse(TComponent* Owner)
 : TForm( Owner ), sites( NULL )
{}

//---------------------------------------------------------------------------

void TfrmBrowse::init( IPart * data, bool p_browse_only )
{
	browse_only = p_browse_only;
	Caption = browse_only ? "Browse Samples" : "Select Item to Move";
	BtnMove->Visible = false;
	SampleTree->Items->Clear();
	if( sites != NULL ) {
		delete sites;
	}
	SampleTree->Items->Add( NULL, "Site List" );
	TTreeNode* parent = SampleTree->Items->GetFirstNode();
	sites = data;
	parent->Data = sites;
	Util::InitPropertyGrid( grdProps );
	Util::ShowTreeNode( SampleTree, parent, true, true );
	ActiveControl = SampleTree;
}

//---------------------------------------------------------------------------

void __fastcall TfrmBrowse::FormDestroy(TObject *Sender)
{
	if( sites != NULL ) {
		delete sites;
		sites = NULL;
	}
}

//---------------------------------------------------------------------------
//	load part of the tree when user clicks on it
//---------------------------------------------------------------------------

void __fastcall TfrmBrowse::TreeClick(TObject *Sender)
{
	Util::ClearPropertyGrid(grdProps);
	TTreeNode* node = SampleTree->Selected;
	if( node != NULL ) {
		IPart* data = (IPart*)node->Data;
		if( data != NULL ) {
			Caption = data->getFullName().c_str();
			if( node->Count < 1 ) {
				Util::ShowTreeNode( SampleTree, node, true, true );
			}
//			Util::ShowPropertyGrid(grdProps, data);
			data->showProperties( grdProps );
		}
		ShowMoveButton( data );
	}
}

//---------------------------------------------------------------------------

void __fastcall TfrmBrowse::MoveClick(TObject *Sender)
{
	TTreeNode* p = SampleTree->Selected;
	if( p == NULL || p->Data == NULL ) {
		return;
	}

	if( p->IsFirstNode() || (p -> Parent)->IsFirstNode() ) {
		Application->MessageBox( L"Cannot move sites", L"Info", MB_OK );
		return;
	}

	IPart* p_part = (IPart*)p->Data;
/*	std::string name = bcsToStd( p -> Text );
	p = p -> Parent;
	while( !p -> IsFirstNode() ) {
		name = bcsToStd( p -> Text ) + " : " + name;
		p = p -> Parent;
	}
*/
	if( frmMove->init( p_part ) ) {
		ModalResult = mrOk;
    }
}

//---------------------------------------------------------------------------

void __fastcall TfrmBrowse::OnGetImage(TObject *Sender, TTreeNode *Node)
{
	Node->ImageIndex = Util::getImageIndex( Node );
}

//---------------------------------------------------------------------------

void __fastcall TfrmBrowse::OnSelectedImage(TObject *Sender, TTreeNode *Node)
{
	Node->SelectedIndex = Util::getImageIndex( Node );
}

//---------------------------------------------------------------------------

void TfrmBrowse::ShowMoveButton(IPart* data)
{
	BtnMove -> Visible = (data != NULL && !browse_only && data -> canMove());
}

//---------------------------------------------------------------------------

void __fastcall TfrmBrowse::BtnFindClick(TObject *Sender)
{
	Util::ClearPropertyGrid(grdProps);
	Box * box = NULL;
	if( frmFind -> ShowModal() == mrOk ) {
		box = frmFind -> getDetails();
	}
	if( box != NULL ) {
		PartFactory pf( true, true );
		pf.addBox( box );
		init( pf.createSiteList(), true );
		expandSubTree( SampleTree -> Items -> GetFirstNode(), box->getName().c_str() );
	}
}

//---------------------------------------------------------------------------

void TfrmBrowse::expandSubTree( TTreeNode* parent, AnsiString selection ) {
	IPart * data = (IPart *)(parent -> Data);
	if( data != NULL && selection.AnsiCompareIC( data->getName().c_str() ) == 0 ) {
		SampleTree -> Selected = parent;
		TreeClick( SampleTree );
	}
	TTreeNode * child = parent -> getFirstChild();
	while( child != NULL ) {
		if( child -> Data != NULL ) {
			Util::ShowTreeNode( SampleTree, child, false, false );
			expandSubTree( child, selection );
		}
		child = child -> getNextSibling();
	}
}

//---------------------------------------------------------------------------



//---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop

#include <memory>
#include "Configure.h"
#include "NewSite.h"
#include "NewTank.h"
#include "StoreUtil.h"
#include "TfrmConfirm.h"
#include "LCDbJob.h"
#include "SMLogin.h"

#pragma package(smart_init)
#pragma resource "*.dfm"

TfrmConfigure *frmConfigure;

//---------------------------------------------------------------------------

__fastcall TfrmConfigure::TfrmConfigure(TComponent* Owner)
	: TForm(Owner)
{}

//---------------------------------------------------------------------------

void TfrmConfigure::init()
{
	enableAdd( IPart::SitesType );
}

//---------------------------------------------------------------------------

void TfrmConfigure::enableAdd( IPart::ItemType selectedType )
{
	BtnAdd->Visible = false;
	BtnAddSib->Visible = false;
	BtnModify->Visible = false;
	BtnOnline->Visible = false;

	TTreeNode* node = SampleTree->Selected;
	switch( selectedType ) {
		case IPart::SitesType:
			BtnAdd->Caption = "Add New Site";
			BtnAdd->Visible = true;
			break;

		case IPart::SiteType:
			BtnAddSib->Caption = "Add New Site";
			BtnAddSib->Visible = true;
			BtnAdd->Caption = "Add Vessel";
			BtnAdd->Visible = true;
			break;

		case IPart::TankType:
			BtnAddSib->Caption = "Add Vessel";
			BtnAddSib->Visible = true;
			if( node != NULL && node -> Data != NULL ) {
				Tank *t = (Tank *)(node -> Data);
				if( t->getShelfNumber() > 0 ) {
					BtnAdd->Caption = "Add Population";
					BtnAdd->Visible = true;
				}
			}
			BtnModify->Visible = true;
			break;

		case IPart::LayoutType:
			if( node != NULL && node -> Data != NULL ) {
				Layout * lay = (Layout *)(node -> Data);
				if( lay->getPosition() > 0 ) {
					BtnAddSib->Caption = "Add Population";
					BtnAddSib->Visible = true;
				}
				if( lay->availability() == Layout::IS_AVAILABLE ) {
					BtnOnline->Caption = "Take off line";
				} else {
					BtnOnline->Caption = "Make Current";
				}
			}
			BtnModify->Visible = true;
			BtnOnline->Visible = true;
	}
}

//---------------------------------------------------------------------------

void __fastcall TfrmConfigure::FormClose(TObject *Sender, TCloseAction &Action)
{
	if( sites != NULL ) {
		delete sites;
		sites = NULL;
	}
}

//---------------------------------------------------------------------------

void __fastcall TfrmConfigure::FormShow(TObject *Sender)
{
	SampleTree->Items->Clear();
	SampleTree->Items->Add( NULL, "Site List" );
	this->ActiveControl = SampleTree;
	Util::InitPropertyGrid( grdProps );
	sites = PartFactory( true, true ).createSiteList();
	TTreeNode* parent = SampleTree->Items->GetFirstNode();
	parent->Data = sites;
	Util::ShowTreeNode( SampleTree, parent, true, true );
}

//---------------------------------------------------------------------------
//	Add a sibling site, tank or layout
//---------------------------------------------------------------------------

void __fastcall TfrmConfigure::AddSibClick(TObject *Sender)
{
	TTreeNode* node = SampleTree->Selected;
	if( node != NULL && !node -> IsFirstNode() && node -> Data != NULL
	 && node -> Parent != NULL && node -> Parent -> Data != NULL ) {
		IPart* data = (IPart*)(node -> Data);
		TTreeNode * parentNode = node -> Parent;
		IPart* parentPart = (IPart*)(parentNode->Data);
		IPart* sib = NULL;
		switch( data -> getType() ) {
			case IPart::SiteType:
				sib = createSite();
				break;
			case IPart::TankType:
				sib = createVessel( parentNode );
				break;
			case IPart::LayoutType:
				sib = addPopulation( (Tank*)parentPart, parentNode->Parent );
		}
		if( sib != NULL ) {
			parentPart -> clearList();
			parentNode -> DeleteChildren();
			delete sib;
		}
	}
	TreeClick( SampleTree );
}

//---------------------------------------------------------------------------

void __fastcall TfrmConfigure::ModifyClick(TObject *Sender)
{
	TTreeNode* node = SampleTree->Selected;
	if( node != NULL && !node -> IsFirstNode() && node -> Data != NULL
	 && node -> Parent != NULL && node -> Parent -> Data != NULL ) {
		IPart* data = (IPart*)(node -> Data);
		TTreeNode * parentNode = node -> Parent;
		IPart* parentPart = (IPart*)(parentNode->Data);
		IPart* modified = NULL;
		switch( data->getType() ) {
			case IPart::TankType:
				modified = changeVessel( (Tank*) data, parentNode );
				break;
			case IPart::LayoutType:
				modified = changePopulation( (Layout*) data, parentNode );
		}
		if( modified != NULL ) {
			parentPart -> clearList();
			parentNode -> DeleteChildren();
			delete modified;
		}
	}
	TreeClick( SampleTree );
}

//---------------------------------------------------------------------------
//	Add a child node: site, tank or layout
//---------------------------------------------------------------------------

void __fastcall TfrmConfigure::AddClick(TObject *Sender)
{
	TTreeNode * node = SampleTree -> Selected;
	IPart *parent, *child = NULL;
	if( node == NULL || node -> IsFirstNode() || node -> Parent == NULL ) {
		parent = sites;
		child = createSite();
	} else {
		parent = (IPart*)(node -> Data);
		switch( parent -> getType() ) {
			case IPart::SitesType:
				child = createSite();
				break;
			case IPart::SiteType:
				child = createVessel( node );
				break;
			case IPart::TankType:
				child = addPopulation( (Tank*)(node->Data), node->Parent );
		}
	}
	if( child != NULL ) {
		parent -> clearList();
		node -> DeleteChildren();
		delete child;
	}
	TreeClick( SampleTree );
}
//---------------------------------------------------------------------------

void __fastcall TfrmConfigure::TreeClick(TObject *Sender)
{
	Util::ClearPropertyGrid( grdProps );
	TTreeNode* node = SampleTree->Selected;
	if( node == NULL || node->Data == NULL ) {
		enableAdd( IPart::UnknownType );
	} else {
		IPart* data = (IPart*)node->Data;
		IPart::ItemType selectedType = data->getType();
		enableAdd( selectedType );

		//don't go below section if configuring items
		if( selectedType < IPart::SectionType && node -> Count < 1 ) {
			Util::ShowTreeNode( SampleTree, node, true, true );
		}
		Util::ShowPropertyGrid( grdProps, data );
	}
}

//---------------------------------------------------------------------------

Site * TfrmConfigure::createSite()
{
	if( frmNewSite -> ShowModal() == mrOk ) {
		Site & site = frmNewSite -> getDetails();
		std::string operation = "create new site " + site.getName();
		if( confirmSave( operation.c_str() ) && site.save() )
			return new Site( site );
	}
	return NULL;
}

//---------------------------------------------------------------------------

Tank * TfrmConfigure::createVessel( TTreeNode * site ) {
	frmNewTank->initNewTank( site, SampleTree );
	if( frmNewTank->ShowModal() == mrOk ) {
		Tank * tank = frmNewTank -> getPart();
		std::string operation = "create new vessel " + tank->getVessel();
		if( confirmSave( operation.c_str() ) )
			return frmNewTank -> createTank();
	}
	return NULL;
}

//---------------------------------------------------------------------------

Tank * TfrmConfigure::addPopulation( Tank * current, TTreeNode *parent )
{
	frmNewTank->initModLayout( current, parent, SampleTree, TfrmNewTank::ADD_LAYOUT );
	if( frmNewTank->ShowModal() == mrOk ) {
		Tank * tank = frmNewTank -> getPart();
		std::string operation = "add new population " + tank->getContent();
		if( confirmSave( operation.c_str() ) )
			return frmNewTank -> saveChanges();
	}
	return NULL;
}

//---------------------------------------------------------------------------

bool TfrmConfigure::confirmSave( const std::string & action )
{
	std::set< int > central;
	central.insert( 0 );
	frmConfirm -> initialise( TfrmSMLogin::CONFIGURE, "Ready to " + action, central );
	return frmConfirm->ShowModal() == mrOk;
}

//---------------------------------------------------------------------------

void __fastcall TfrmConfigure::OnGetImage(TObject *Sender, TTreeNode *Node)
{
	Node->ImageIndex = Util::getImageIndex( Node );
}

//---------------------------------------------------------------------------

void __fastcall TfrmConfigure::OnSelectedImage(TObject *Sender, TTreeNode *Node)
{
	Node->SelectedIndex = Util::getImageIndex( Node );
}

//---------------------------------------------------------------------------

Tank * TfrmConfigure::changeVessel( Tank * current, TTreeNode *parent )
{
	frmNewTank->initModLayout( current, parent, SampleTree, TfrmNewTank::EDIT_VESSEL );
	if( frmNewTank->ShowModal() == mrOk ) {
		Tank * tank = frmNewTank -> getPart();
		std::string operation = "restructure vessel " + tank->getName();
		if( confirmSave( operation.c_str() ) ) {
			current -> takeOffline();
			return frmNewTank -> saveChanges();
		}
	}
	return NULL;
}

//---------------------------------------------------------------------------

Tank * TfrmConfigure::changePopulation( Layout * current, TTreeNode *parent )
{
	Tank * tank = (Tank*)(current->getParent());
	tank->setFields( *current );
	frmNewTank->initModLayout( tank, parent->Parent, SampleTree, TfrmNewTank::MOD_LAYOUT );
	if( frmNewTank->ShowModal() == mrOk ) {
		tank = frmNewTank -> getPart();
		std::string operation = "modify population " + tank->getContent();
		if( confirmSave( operation.c_str() ) ) {
			current -> setAvailability( false );
			current -> saveAvailability();
			return frmNewTank -> saveChanges();
		}
	}
	return NULL;
}

//---------------------------------------------------------------------------

void __fastcall TfrmConfigure::OnlineClick(TObject *Sender)
{
	TTreeNode* node = SampleTree->Selected;
	if( node == NULL || node -> IsFirstNode() || node -> Data == NULL )
		return;

	Layout * lay = (Layout*) node->Data;
	TTreeNode* parent = node->Parent;
	if( parent->Data == NULL || lay->getType() != IPart::LayoutType )
		return;

	Tank * tank = (Tank *)(parent -> Data);
	if( Application->MessageBox( L"Are you sure? ", L"Change Status",
			MB_ICONWARNING|MB_YESNO|MB_DEFBUTTON2) == IDYES )
	{
		if( lay -> availability() == Layout::IS_AVAILABLE )
			lay -> setAvailability( false );
		else
		{	tank -> takeOffline();
			lay -> setAvailability( true );
		}
		lay -> saveAvailability();
		tank -> loadTankDetails();
	}

	parent -> DeleteChildren();
	tank -> clearList();
	SampleTree -> Selected = parent;
	TreeClick( SampleTree );
}

//---------------------------------------------------------------------------


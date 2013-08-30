//---------------------------------------------------------------------------

#include <vcl.h>
#include <sstream>
#pragma hdrstop

#include "SampleMove.h"
#include "NewSite.h"
#include "StoreUtil.h"
#include "LDbBoxStore.h"
#include "LCDbAuditTrail.h"
#include "TfrmConfirm.h"
#include "NewJob.h"
#include "LIMSDatabase.h"
#include "SMLogin.h"
#include "LCDbJob.h"
#include "LDbBoxType.h"

#pragma package(smart_init)
#pragma resource "*.dfm"

TfrmMove *frmMove;

//---------------------------------------------------------------------------

__fastcall TfrmMove::TfrmMove(TComponent* Owner)
 : TForm(Owner), jobQuery( LIMSDatabase::getCentralDb() )
{}

//---------------------------------------------------------------------------

bool TfrmMove::init( IPart* p_part )
{
	createNewJob = true;
	initDisplay( "Prepare to move from " + p_part -> getFullName() );

	TTreeNode* parent = AvlTree->Items->Add( NULL, "Available-Storage" );
	root = PartFactory( false, false ).createSiteList();
	parent -> Data = root;
	Util::ShowTreeNode(AvlTree, parent, false, true);

	part = p_part;
	parent = SampleTree->Items->Add( NULL, part->getName().c_str() );
	parent -> Data = part;
	Util::ShowTreeNode(SampleTree, parent, false, true);
	Util::ShowPropertyGrid( grdLeftProps, part );
	return true;
}

//---------------------------------------------------------------------------

void TfrmMove::initDisplay( const std::string & title )
{
	Caption = title.c_str();
	Util::InitPropertyGrid( grdLeftProps );
	Util::InitPropertyGrid( grdRightProps );
	AvlTree->Items->Clear();
	SampleTree->Items->Clear();
	ActiveControl = SampleTree;
	current = NULL;
}

//---------------------------------------------------------------------------

bool TfrmMove::init( const LCDbCryoJob *p_job )
{
	createNewJob = false;
	job = *p_job;
	initDisplay( "Move job " + job.getName() );

	PartFactory rhs( false, false );
	rhs.loadBoxes( job );
	root = rhs.createSiteList();
	if( root == NULL )
	{
		String error = "Cannot create RHS for ";
		error += job.getName().c_str();
		Application -> MessageBox( error.c_str(), NULL, MB_OK );
		return false;
	}

	PartFactory lhs( true, false );
	lhs.loadBoxes( job );
	part = lhs.createSiteList();
	if( part == NULL )
	{
		String error = "Cannot create LHS for ";
		error += job.getName().c_str();
		Application -> MessageBox( error.c_str(), NULL, MB_OK );
		return false;
	}

	int pid = LCDbAuditTrail::getCurrent().getProcessID();
	bool ownJob = (job.getProcessCID() == pid);
	if( !job.claim( jobQuery, ownJob ) )
	{
		String error = "Cannot claim job ";
		error += job.getName().c_str();
		Application -> MessageBox( error.c_str(), NULL, MB_OK );
		return false;
	}

	rightKids.clear();
	makeBoxList( root, rightKids, isUnmappedBox );
	leftKids.clear();
	makeBoxList( part, leftKids, isUnmappedBox );
	mapChildrenByJob();

	TTreeNode* parent = AvlTree->Items->Add( NULL, "Allocated-Storage" );
	parent->Data = (void*)root;
	LoadTree( AvlTree, parent, root );

	parent = SampleTree->Items->Add( NULL, "Items-to-Move" );
	parent->Data = (void*)part;
	LoadTree( SampleTree, parent, part );
	return true;
}

//---------------------------------------------------------------------------

void TfrmMove::LoadTree( TTreeView* tree, TTreeNode* parent, IPart* data )
{
	if( data != NULL && data->getType() < IPart::BoxType ) { //don't go below box
		Util::ShowTreeNode(tree, parent, false, false);
		TTreeNode* node = parent->getFirstChild();
		while( node != NULL ) {
			LoadTree( tree, node, (IPart*)node->Data );
			node = node->GetNextChild(node);
		}
	}
}

//---------------------------------------------------------------------------

void TfrmMove::deleteNodes( TTreeView* tree, TTreeNode* parent )
{
	TTreeNode* tmp = parent->getFirstChild();
	while( tmp != NULL ) {
		deleteNodes( tree, tmp );
		tmp = parent->GetNextChild( tmp );
	}
	if (!parent->IsFirstNode()) {
		tree->Items->Delete( parent );
	}
}

//---------------------------------------------------------------------------

bool TfrmMove::canAssign()
{
	if( SampleTree -> Selected == NULL || AvlTree -> Selected == NULL ) {
		return false;
	}
	IPart* leftData = (IPart *)(SampleTree -> Selected -> Data);
	IPart* rightData = (IPart *)(AvlTree -> Selected -> Data);
	return leftData != NULL && getImageIndex( leftData ) == Util::UNASSIGNED
		&& rightData != NULL && rightData->getType() >= IPart::LayoutType;
}

//---------------------------------------------------------------------------

void __fastcall TfrmMove::AssignClicked(TObject *Sender)
{
	BtnAssign -> Visible = false;
	if( canAssign() ) {
		IPart* leftData = (IPart *)(SampleTree -> Selected -> Data);
		Box* oldBox = dynamic_cast< Box* >( leftData );
		IPart* rightData = (IPart *)(AvlTree -> Selected -> Data);
		Box* newSlot = dynamic_cast< Box* >( rightData );
		if( oldBox && newSlot ) {
			mapBox( oldBox, newSlot );
		} else {
			leftKids.clear();
			Util::ShowSelectedSubTree( SampleTree, 0 );
			makeBoxList( leftData, leftKids, isUnmappedBox );
			rightKids.clear();
			Util::ShowSelectedSubTree( AvlTree, leftKids.size() );
			makeBoxList( rightData, rightKids, isUnmappedBox );
			mapChildren();
		}
	}
	updateDisplay();
}

//---------------------------------------------------------------------------

bool TfrmMove::mapBox( Box * oldBox, Box * newSlot ) {
	if( oldBox -> isLHSAssigned() || newSlot -> isRHSAssigned() ) {
		Application->MessageBox( L"Box is part of another job", NULL, MB_OK );
		return false;
	} else {
		oldBox -> setMapped( newSlot );
		oldBox -> setStatus( LCDbBoxStore::MOVE_EXPECTED );
		newSlot -> setMapped( oldBox );
		newSlot -> setStatus( LCDbBoxStore::EXPECTED );
		return true;
	}
}

//---------------------------------------------------------------------------

void TfrmMove::listCurrentBoxes( bool (*boxfn)(Box* b) )
{
	leftKids.clear();
	rightKids.clear();
	if( current -> TreeView == SampleTree ) {
		makeBoxList( (IPart *) current -> Data, leftKids, boxfn );
		for( std::vector<Box*>::const_iterator bi = leftKids.begin(); bi != leftKids.end(); ++ bi )
			if( boxfn( *bi ) )
				rightKids.push_back( (Box*)((*bi) -> getMapped()) );
	} else {
		makeBoxList( (IPart *) current -> Data, rightKids, boxfn );
		for( std::vector<Box*>::const_iterator bi = rightKids.begin(); bi != rightKids.end(); ++ bi )
			if( boxfn( *bi ) )
				leftKids.push_back( (Box*)((*bi) -> getMapped()) );
	}
}

//---------------------------------------------------------------------------

void TfrmMove::makeBoxList( IPart* item, std::vector<Box*>& boxes, bool (*boxfn)(Box* b) )
{
	Box * b = dynamic_cast<Box*>( item );
	if( b )	{
		if( boxfn( b ) ) {
			boxes.push_back( b );
		}
	} else if( item != NULL ) {
		const std::vector<IPart*> & list = item->getList();
		for( int i = 0; i < (int)list.size(); i++ ) {
			makeBoxList( list[i], boxes, boxfn );
		}
	}
}

//---------------------------------------------------------------------------

bool TfrmMove::isUnmappedBox( Box* b )
{
	return b->getMapped() == NULL;
}

bool TfrmMove::isAssignedBox( Box* b )
{
	return b->isLHSAssigned() || b->isRHSAssigned();
}

bool TfrmMove::isDoneBox( Box* b )
{
	return b->isLHSDone() || b->isRHSDone();
}

//---------------------------------------------------------------------------

void TfrmMove::mapChildrenByJob()
{
	int lsize = (int)leftKids.size();
	int rsize = (int)rightKids.size();
	for( int i = 0; i < lsize; i++ )
	{
		IPart *left = leftKids[i];
		for( int j = 0; j < rsize; j++ )
		{
			IPart *right = rightKids[j];
			if( left->getID() == right->getID() )
			{
				left -> setMapped( right );
				right -> setMapped( left );
				break;
			}
		}
	}
}

//---------------------------------------------------------------------------

void TfrmMove::mapChildren()
{
	unsigned lsize = leftKids.size();
	unsigned rsize = rightKids.size();
	int next = 0;
	for( unsigned i = 0; i < lsize; i++ ) {
		Box * left = (Box *)leftKids[i];
		if( !left -> isLHSAssigned() ) {
			for( unsigned j = next; j < rsize; j++ ) {
				Box * right = (Box *)rightKids[j];
				if( !right -> isRHSAssigned() )	{
					mapBox( left, right );
					next = j + 1;
					break;
				}
			}
		}
	}
}

//---------------------------------------------------------------------------

void TfrmMove::unmapChildren( IPart* left )
{
	if( left == NULL )
		return;

	if( left->getMapped() != NULL )
	{
		left->getMapped()->setMapped(NULL);
		left->setMapped(NULL);
	}

	const std::vector<IPart*> & list = left -> getList();
	for( std::vector<IPart*>::const_iterator li = list.begin(); li != list.end(); ++ li )
		unmapChildren( *li );
}

//---------------------------------------------------------------------------

void TfrmMove::showDetails( TTreeView* tree, TStringGrid* grid, bool expand )
{
	Util::ClearPropertyGrid(grid);
	TTreeNode* node = tree->Selected;
	if( node != NULL && node->Data != NULL )
	{
		IPart* data = (IPart*)node->Data;
		if( node->Count == 0 && data->getType() < IPart::BoxType ) {
			Util::ShowTreeNode( tree, node, false, expand );
		}
		Util::ShowPropertyGrid( grid, data );
		if( expand ) {
			node -> Expand( true );
			selectChildren( tree, node );
			tree -> Selected = node;
        }
	}
	tree -> Invalidate();
}

//---------------------------------------------------------------------------

void __fastcall TfrmMove::UnassignClicked(TObject *Sender)
{
	BtnUnassign -> Visible = false;
	if( current == NULL || current -> Data == NULL )
		return;

	IPart * data = (IPart *)(current -> Data);
	if( data -> getType() < IPart::BoxType )
	{
		String buf = "Remove assignments to <";
		buf += data->getName().c_str();
		buf += "> and all its children?";
		if( Application->MessageBox( buf.c_str(), L"Warning", MB_YESNO|MB_DEFBUTTON2 ) == IDNO )
			return;
	}
	
	unmapChildren( data );
	updateDisplay();
}

//---------------------------------------------------------------------------

void TfrmMove::selectChildren( TTreeView* tree, TTreeNode* parent )
{
	if( parent != NULL ) {
		tree->Selected = parent;
		TTreeNode* tmp = parent->getFirstChild();
		while( tmp != NULL ) {
			selectChildren( tree, tmp );
			tmp = parent->GetNextChild( tmp );
		}
	}
}

//---------------------------------------------------------------------------

int TfrmMove::getImageIndex( TTreeNode *node )
{
	if( node == NULL || node -> Data == NULL ) {
		return Util::UNASSIGNED;
	}
	IPart* data = (IPart*)(node -> Data);
	float fill = -1;
	if( createNewJob ) {
		switch( data->getType() ) {
			case IPart::TankType: {
				AvlTank * at = dynamic_cast< AvlTank *>( data );
				if( at ) {
					fill = at -> getFillFactor();
				}
				break;
			}
			case IPart::LayoutType: {
				if( ((Layout*)(data))->availability() == IPart::UNAVAILABLE ) {
					return Util::OFF_LINE;
				} else {
					return Util::UNASSIGNED;
				}
			}
			case IPart::SectionType: {
				AvlSection * at = dynamic_cast< AvlSection *>( data );
				if( at ) {
					fill = at -> getFillFactor();
				}
				break;
			}
			case IPart::RackType: {
				AvlRack * at = dynamic_cast< AvlRack *>( data );
				if( at ) {
					fill = at -> getFillFactor();
				}
			}
		}
	}

	if( fill < 0 ) {
		return getImageIndex( data );
	} else if( fill < 0.01 ) {
		return Util::NO_CHILDREN;
	} else if( fill > 0.98 ) {
		return Util::ALL_FILLED;
	} else {
		return Util::PART_FILLED;
	}
}

//---------------------------------------------------------------------------

int TfrmMove::getImageIndex( IPart *data )
{
	if( data->getType() == IPart::SitesType ) {
		return Util::SITE_LIST;
	} else if( data->getType() == IPart::SiteType ) {
		return Util::SITE;
	}

	Box * b = dynamic_cast<Box*>( data );
	if( b )	{
		if( b->isLHSAssigned() || b->isRHSAssigned() ) {
			return Util::ASSIGNED;
		}
		if( b->isLHSDone() || b->isRHSDone() ) {
			return Util::AVAILABLE;
		}
/*		if( createNewJob ) {
			if( b->availability() == Box::UNAVAILABLE ) {
				return I_UNAVAILABLE;
			}
		}
*/	}

	int children = data->getChildCount();
	if( children == 0 ) {
		return Util::NO_CHILDREN;
	}
	if( children < 0 ) {
		return Util::UNASSIGNED;
	}

	const std::vector<IPart*> & list = data->getList();
	int min = Util::AVAILABLE;
	for( std::vector<IPart*>::const_iterator li = list.begin(); li != list.end(); ++ li ) {
		int it = getImageIndex( *li );
		if( it < min ) {
			min = it;
		}
	}
	return min;
}

//---------------------------------------------------------------------------

bool TfrmMove::allBoxes( IPart* parent, Util::IconType type )
{
	if( parent -> getType() == IPart::BoxType ) {
		return getImageIndex( parent ) == type;  	// children don't matter
	}
	const std::vector<IPart*> & list = parent -> getList();
	for( std::vector<IPart*>::const_iterator li = list.begin(); li != list.end(); ++ li ) {
		if( !allBoxes( *li, type ) ) {
			return false;
		}
	}
	return true;
}

//---------------------------------------------------------------------------

bool TfrmMove::allBoxes( TTreeNode* parent, Util::IconType type )
{
	if( parent == NULL || parent -> Data == NULL ) {
		return false;
	} else {
		return allBoxes( (IPart*)(parent -> Data), type );
	}
}

//---------------------------------------------------------------------------

bool TfrmMove::anyBoxes( IPart * parent, Util::IconType type )
{
	if( parent -> getType() == IPart::BoxType ) {
		return getImageIndex( parent ) == type;
	}
	const std::vector<IPart*> & list = parent -> getList();
	for( std::vector<IPart*>::const_iterator li = list.begin(); li != list.end(); ++ li ) {
		if( anyBoxes( *li, type ) ) {
			return true;
		}
	}
	return false;
}

//---------------------------------------------------------------------------

bool TfrmMove::anyBoxes( TTreeNode* parent, Util::IconType type )
{
	if( parent == NULL || parent -> Data == NULL ) {
		return false;
	} else {
		return anyBoxes( (IPart*)(parent -> Data), type );
	}
}

//---------------------------------------------------------------------------

void TfrmMove::checkButtons()
{
	if( createNewJob ) {
		BtnAssign -> Visible = canAssign();
		if( anyBoxes( current, Util::ASSIGNED ) ) {
			BtnUnassign -> Visible = true;
			BtnCreate -> Visible = true;
		} else {
			BtnUnassign -> Visible = false;
			BtnCreate -> Visible = false;
		}
		BtnDone -> Visible = false;
		BtnRevert -> Visible = false;
		BtnSignOff -> Visible = false;
	} else {
		BtnAssign -> Visible = false;
		BtnUnassign -> Visible = false;
		BtnCreate -> Visible = false;
		if( allBoxes( part, Util::AVAILABLE ) ) {
			BtnDone -> Visible = false;
			BtnSignOff -> Visible = true;
			BtnRevert -> Visible = true;
		} else {
			BtnDone -> Visible = anyBoxes( current, Util::ASSIGNED );
			BtnSignOff -> Visible = false;
			BtnRevert -> Visible = anyBoxes( current, Util::AVAILABLE );
		}
	}
}

//---------------------------------------------------------------------------

void __fastcall TfrmMove::FormClose(TObject *Sender, TCloseAction &Action)
{
	if( createNewJob )
	{
		if( ModalResult != mrOk && anyBoxes( part, Util::ASSIGNED )
		 && Application->MessageBox( L"Discard current assignment?", L"Warning",
						MB_ICONWARNING|MB_YESNO|MB_DEFBUTTON2) != IDYES ) {
			Action = caNone;
			return;
		}
		unmapChildren( part );
	}
	else
	{	if( job.getStatus() == LCDbCryoJob::INPROGRESS )
			job.release( jobQuery, false );

		if( part != NULL )
		{
			delete part;
			part = NULL;
		}
	}

	if( root != NULL )
	{
		delete root;
		root = NULL;
	}
}

//---------------------------------------------------------------------------

void __fastcall TfrmMove::CreateClick(TObject *Sender)
{
	if( !allBoxes( part, Util::ASSIGNED )
	 && Application->MessageBox( L"Some boxes are not assigned yet; save anyway?",
							L"Warning", MB_ICONWARNING|MB_YESNO|MB_DEFBUTTON2) == IDNO ) {
		return;
	}

	if( frmNewJob -> ShowModal() != mrOk )
		return;

	String error;
	BtnCreate -> Visible = false;
	Screen->Cursor = crSQLWait;
	leftKids.clear();
	makeBoxList( part, leftKids, isAssignedBox );
	if( frmNewJob -> createJob( LCDbCryoJob::BOX_MOVE, leftKids ) ) {
		job = frmNewJob -> getDetails();
	} else {
		error = "Error creating job record";
	}
	progress -> Max = leftKids.size();
	for( std::vector<Box*>::iterator bi = leftKids.begin(); error.IsEmpty() && bi != leftKids.end(); ++ bi ) {
		Box *left = *bi, *right = dynamic_cast< Box* >( left->getMapped() );
		if( left->addToLHSJobList( job.getID() ) && right != NULL && right->addToRHSJobList( job.getID() ) ) {
			progress -> StepIt();
		} else {
			error = "Error creating movement job";
		}
	}
	if( error.IsEmpty() && !job.release( jobQuery, false ) ) {
		error = "Error releasing job record";
	}
	Screen->Cursor = crDefault;
	if( error.IsEmpty() ) {
		this -> ModalResult = mrOk;
	} else {
		Application->MessageBox( error.c_str(), NULL, MB_OK );
		updateDisplay();
	}
}

//---------------------------------------------------------------------------

/*
bool TfrmMove::markPart( IPart* left )
{
	if( left == NULL || !left -> addToLHSJobList( job.getID() ) )
		return false;

	IPart * right = left -> getMapped();
	if( right != NULL && !right -> addToRHSJobList( job.getID() ) )
		return false;

	const std::vector<IPart*> & list = left -> getList();
	for( std::vector<IPart*>::const_iterator li = list.begin(); li != list.end(); ++ li )
		if( !markPart( *li ) )
			return false;

	return true;
}
//---------------------------------------------------------------------------

std::string TfrmMove::getFullPartName( IPart* part )
{
	Box * box = dynamic_cast< Box * >( part );
	Rack * rack = dynamic_cast< Rack * >( box->getParent() );
	Section * section = dynamic_cast< Section * >( rack->getParent() );
	Layout * layout = dynamic_cast< Layout * >( section->getParent() );
	Tank * tank = dynamic_cast< Tank * >( layout->getParent() );

	std::string sep = "::";
	std::stringstream result;
	result << tank->getName() << sep << layout->getName() << sep << section->getName()
		   << sep << rack->getName() << sep << box->getName() << " (pos:" << box->getPosition() << ')';
	return result.str();
}
*/

//---------------------------------------------------------------------------

void __fastcall TfrmMove::DoneClick(TObject *Sender)
{
	BtnDone -> Visible = false;
	if( current == NULL || current -> Data == NULL )
		Application->MessageBox( L"No items selected to mark as done", NULL, MB_OK );
	else
	{ 	Screen->Cursor = crSQLWait;
		listCurrentBoxes( isAssignedBox );
		progress -> Max = rightKids.size() + leftKids.size();
		SetStatus( rightKids, LCDbBoxStore::SLOT_ALLOCATED );
		SetStatus( leftKids, LCDbBoxStore::REMOVED );
		Screen->Cursor = crDefault;
		updateDisplay();
	}
}

//---------------------------------------------------------------------------

void __fastcall TfrmMove::SignOffClick(TObject *Sender)
{
	if( !allBoxes( part, Util::AVAILABLE ) )
	{
		Application->MessageBox( L"Some boxes are not set to \'Done\'", NULL, MB_OK );
		return;
	}

	leftKids.clear();
	makeBoxList( part, leftKids, isDoneBox );
	std::set<int> projects;
	for( std::vector<Box*>::const_iterator bi = leftKids.begin(); bi != leftKids.end(); ++ bi ) {
		projects.insert( (*bi) -> getProjectCID() );
	}

	char summary[ 70 ];
	std::sprintf( summary, "%d boxes have been marked \'Done\'", (int) leftKids.size() );
	frmConfirm -> initialise( TfrmSMLogin::MOVE, summary, projects );
	if( frmConfirm->ShowModal() != mrOk )
		return;

	String error;
	if( !SignOff() ) {
		error = "Error updating box location";
	} else if( !job.release( jobQuery, true ) ) {
		error = "Error releasing job record";
	}
	if( error.IsEmpty() ) {
		ModalResult = mrOk;
	} else {
		Application->MessageBox( error.c_str(), NULL, MB_OK );
		updateDisplay();
    }
}

//---------------------------------------------------------------------------

bool TfrmMove::SignOff()
{
	rightKids.clear();
	makeBoxList( root, rightKids, isAnyBox );
	progress -> Max = rightKids.size();
	for( std::vector<Box*>::const_iterator bi = rightKids.begin(); bi != rightKids.end(); ++ bi ) {
		if( (**bi).getStatus() == LCDbBoxStore::SLOT_ALLOCATED && (**bi).signoff() ) {
			progress -> StepIt();
		} else {
			return false;
		}
	}
	return true;
}

//---------------------------------------------------------------------------

void TfrmMove::SetStatus( std::vector<Box*>& boxes, short status )
{
	for( std::vector<Box*>::iterator bi = boxes.begin(); bi != boxes.end(); ++ bi ) {
		(**bi).setStatus( status );
		if( (**bi).save() ) {
			progress -> StepIt();
        }
	}
}

//---------------------------------------------------------------------------

void __fastcall TfrmMove::RevertClick(TObject *Sender)
{
	BtnRevert -> Visible = false;
	if( current == NULL || current -> Data == NULL )
		Application->MessageBox( L"No items selected to revert", NULL, MB_OK );
	else
	{ 	Screen->Cursor = crSQLWait;
		listCurrentBoxes( isDoneBox );
		progress -> Max = rightKids.size() + leftKids.size();
		SetStatus( rightKids, LCDbBoxStore::EXPECTED );
		SetStatus( leftKids, LCDbBoxStore::MOVE_EXPECTED );
		Screen->Cursor = crDefault;
		updateDisplay();
	}
}

//---------------------------------------------------------------------------

void __fastcall TfrmMove::SampleTreeGetImageIndex(TObject *Sender, TTreeNode *Node)
{
	Node -> ImageIndex = getImageIndex( Node );
}

//---------------------------------------------------------------------------

void __fastcall TfrmMove::SampleTreeGetSelectedIndex(TObject *Sender, TTreeNode *Node)
{
	Node -> SelectedIndex = getImageIndex( Node );
}

//---------------------------------------------------------------------------

void __fastcall TfrmMove::AvlTreeGetSelectedIndex(TObject *Sender, TTreeNode *Node)
{
	Node -> SelectedIndex = getImageIndex( Node );
}

//---------------------------------------------------------------------------

void __fastcall TfrmMove::AvlTreeGetImageIndex(TObject *Sender, TTreeNode *Node)
{
	Node -> ImageIndex = getImageIndex( Node );
}

//---------------------------------------------------------------------------

void __fastcall TfrmMove::SampleTreeClick(TObject *Sender)
{
	current = SampleTree -> Selected;
	showDetails( SampleTree, grdLeftProps, true );
	checkButtons();
}

//---------------------------------------------------------------------------

void __fastcall TfrmMove::AvlTreeClick(TObject *Sender)
{
	current = AvlTree -> Selected;
	showDetails( AvlTree, grdRightProps, true );
	checkButtons();
}

//---------------------------------------------------------------------------

void TfrmMove::updateDisplay()
{
	progress -> Position = 0;
	showDetails( SampleTree, grdLeftProps, true );
	showDetails( AvlTree, grdRightProps, true );
	checkButtons();
}

//---------------------------------------------------------------------------

void __fastcall TfrmMove::FormShow(TObject *Sender)
{
	current = NULL;
//	SampleTree -> Selected = getFirstRack( SampleTree );
	AvlTree -> Selected = getFirstRack( AvlTree );
	checkButtons();
}

//---------------------------------------------------------------------------

TTreeNode * TfrmMove::getFirstRack( TTreeView * view ) {
	TTreeNode * node = view -> Items -> GetFirstNode();
	for( ;; ) {
		TTreeNode * next = node -> getFirstChild();
		if( next == NULL || next -> Data == NULL ) {
			next = node -> GetNext();
			if( next == NULL || next -> Data == NULL ) {
				return node;
			}
		}
		IPart * data = (IPart *)(next -> Data);
		if( data -> getType() >= IPart::RackType ) {
			return next;
		}
		node = next;
	}
}

//---------------------------------------------------------------------------

void __fastcall TfrmMove::FormResize(TObject *Sender)
{
	int width = this->Width / 2 - 70;
	int mid = this->Width / 2 + 50;
	SampleTree -> Width = width;
	grdLeftProps -> Width = width;
	AvlTree -> Left = mid;
	AvlTree -> Width = width;
	LblRightCap -> Left = mid;
	grdRightProps -> Left = mid;
	grdRightProps -> Width = width;
}

//---------------------------------------------------------------------------


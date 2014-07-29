//---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop

#include "NewTank.h"
#include "NewSection.h"
#include "StoreUtil.h"
#include "LCDbObject.h"
#include "NewType.h"
#include "LCDbTankMap.h"

#pragma package(smart_init)
#pragma resource "*.dfm"

TfrmNewTank *frmNewTank;

//---------------------------------------------------------------------------
__fastcall TfrmNewTank::TfrmNewTank(TComponent* Owner)
	: TForm(Owner)
{}

//---------------------------------------------------------------------------

void TfrmNewTank::initNewTank( TTreeNode* p_parent, TTreeView *p_tree )
{
	mode = NEW_VESSEL;
	tree = p_tree;
	SiteNode = p_parent;
	oldtank = NULL;
	tank = new Tank();
	Site* parent = (Site*)(SiteNode->Data);
	tank->setParent( parent );
	tank->setAvailability( true );
	TankInit( "Configure New Vessel" );

	// initialize tank attributes; all fields can be modified
	fillSiteList( parent->getID() );
	int pos = getDefaultPosition();
	TxtPos->Text = pos;
	UpDownPos->Position = pos;
	TxtSrl->Text = "";
	TxtSrl->ReadOnly = false;
	TxtPhysical->Text = "";
	TxtPhysical->ReadOnly = false;
	cbStoreType -> Enabled = true;
	fillStorageTypes( getDefaultStoreTypeID() );
	fillUpLayouts();
	cbLayout->ItemIndex = getDefaultLayoutIndex();
	chkPop->Enabled = true;
	setPopulation( 0 );
}

//---------------------------------------------------------------------------

void TfrmNewTank::initModLayout( Tank * p_tank, TTreeNode* p_parent, TTreeView *p_tree, Mode reason )
{
	mode = reason;
	SiteNode = p_parent;
	tree = p_tree;
	oldtank = p_tank;
	tank = new Tank( *oldtank );
	tank->discardList();
	IPart* parent = (IPart*)SiteNode->Data;
	tank->setParent(parent);
	TankInit( "Modify Tank Layout" );

	// use existing attributes; population and/or vessel can be modified
	fillSiteList( parent->getID() );
	UpDownPos->Position = oldtank->getPosition();
	TxtPhysical->Text = oldtank->getVessel().c_str();
	TxtSrl->Text = oldtank->getSrlno().c_str();
	fillStorageTypes(oldtank->getStoreType());
	bool ev = (reason == EDIT_VESSEL);
	TxtPhysical->ReadOnly = !ev;
	TxtSrl->ReadOnly = !ev;
	cbStoreType->Enabled = ev;

	// use existing layout if known, otherwise copy from previous vessel
	fillUpLayouts();
	int i = layman.find( oldtank->getLayoutID() );
	if( i < 0 ) {
		i = layman.find( layman.getDefaultLayoutId( oldtank->getID() ) );
	}
	cbLayout->ItemIndex = i;
	cbLayout->Enabled = (reason != EDIT_VESSEL);

	// frames have a population on each shelf, tanks have no shelves
	short shelf = oldtank->getShelfNumber();
	if( reason == ADD_LAYOUT ) {
		const std::vector<IPart*> & layout = oldtank->getList();
		for( unsigned i = 0; i < layout.size(); i++ ) {
			short pos = layout[ i ] -> getPosition();
			if( pos >= shelf ) {
				shelf = pos + 1;
			}
		}
	}
	chkPop->Enabled = (shelf == 0);
	setPopulation( shelf );
}

//---------------------------------------------------------------------------

void TfrmNewTank::setPopulation( short shelf ) {
	bool canAlter = (shelf > 0);
	lblPop->Visible = canAlter;
	TxtPop->Text = shelf;
	TxtPop->Visible = canAlter;
	UpDownPop->Position = shelf;
	UpDownPop->Visible = canAlter;
}

//---------------------------------------------------------------------------

void __fastcall TfrmNewTank::NextClick(TObject *Sender)
{
	if( pnlTank->Visible ) {
		TankValidate();
	} else if( pnlLayout->Visible ) {
		LayoutValidate();
	} else if( pnlConfirm->Visible ) {
		ModalResult = mrOk;
	}
}

//---------------------------------------------------------------------------

void TfrmNewTank::TankInit( const char* title )
{
	BtnBack->Visible = false;
	BtnNext->Caption = "Next >";
	pnlTank->Visible = true;
	pnlLayout->Visible = false;
	pnlConfirm->Visible = false;
	if( title != NULL ) {
		frmNewTank->Caption = title;
	}
	clearLayoutPanel();
}

//---------------------------------------------------------------------------

bool TfrmNewTank::TankValidate()
{
	if( !checkVessel( TxtPhysical, LblPhys, true ) ) {
		return false;
	}
	if( TxtSrl->Text.IsEmpty() ) {
		TxtSrl->Text = TxtPhysical->Text;
	}
	if( !checkVessel( TxtSrl, LblSrl, false ) ) {
		return false;
	}
	AnsiString place =  cbLocation->Text.Trim() ;
	const LCDbObject * loc = LCDbObjects::records().find( place.c_str(), LCDbObject::STORAGE_SITE );
	if( loc == NULL ) {
		Application->MessageBox( L"Please select site", L"Invalid site", MB_ICONWARNING );
		return false;
	}
	short pos = Util::validateInteger( TxtPos, 1, 100 );
	short shelf = Util::validateInteger( TxtPop, 0, 20 );
	if( pos < 0 || shelf < 0 || !checkPosition( loc->getID(), pos, shelf ) ) {
		return false;
	}
	const LCDbObject * st = findStorageType();
	if( st == NULL ) {
		return false;
	}
	Layout * l = layman.getLayout( cbLayout->ItemIndex );
	if( l == NULL || cbLayout->Text != l->getLayoutDescription().c_str() ) {
		if( Application->MessageBox( L"Create new layout?", L"Layout changed", MB_ICONQUESTION|MB_YESNO ) == IDYES ) {
			l = new Layout;
			layman.addToList( l );
		} else {
			return false;
		}
	}
	AnsiString serial = TxtSrl->Text.Trim();
	tank->setSrlno( serial.c_str() );
	AnsiString name = TxtPhysical->Text.Trim();
	tank->setName(  name.c_str() );
	tank->setPosition( pos );
	tank->setLocationID( loc->getID() );
	tank->setShelfNumber( shelf );
	tank->setStoreType( st->getID() );

	layout_cid = l->getLayout_cid();
	if( layout_cid == -1 ) {
		LayoutInit();	 		// new layout is chosen so take all details
	} else {
		TankConfirmInit();		// all details entered; ask user to confirm
	}
	return true;
}

//---------------------------------------------------------------------------

void TfrmNewTank::LayoutInit()
{
	frmNewTank->Caption = "Configure Storage Layout";
	BtnNext->Caption = "Next >";
	BtnBack->Visible = true;
	pnlConfirm->Visible = false;
	pnlTank->Visible = false;
	pnlLayout->Visible = true;
}

//---------------------------------------------------------------------------

bool TfrmNewTank::LayoutValidate()
{
	AnsiString layname, layfull;
	if( !Util::validateText( TxtLayName, LblLayName ) || !Util::validateText( TxtLayFull, LblLayFull ) ) {
		return false;
	}
	layname =  TxtLayName->Text.Trim();
	layfull =  TxtLayFull->Text.Trim();
	if( layman.isNameDuplicate( false, layname.c_str() ) || layman.isNameDuplicate( true, layfull.c_str() ) )
	{
		Application->MessageBox( L"Another layout has the same name", NULL, MB_ICONWARNING );
		return false;
	}
	nSections = Util::validateInteger( TxtCount, 1, 20 );
	if( nSections == 0 ) {
		return false;
	}
	Layout * newLayout = layman.getLayout( layman.find( layout_cid ) );
	if( nSections != newLayout->getChildCount() ) {
		Application->MessageBox( L"Number of sections doesn't match #Sections", NULL, MB_ICONWARNING );
		return false;
	}
	newLayout->setLayoutType( layname.c_str(), layfull.c_str() );
	TankConfirmInit();
	return true;
}

//---------------------------------------------------------------------------

void TfrmNewTank::TankConfirmInit()
{
	frmNewTank->Caption = "Confirm Storage Information";
	BtnNext->Caption = "OK";
	BtnBack->Visible = true;
	pnlConfirm->Visible = true;
	pnlTank->Visible = false;
	pnlLayout->Visible = false;

	TxtPhysical1->Text = tank->getVessel().c_str();
	TxtSrl1->Text = tank->getSrlno().c_str();
	const LCDbObjects & names = LCDbObjects::records();
	const LCDbObject * site = names.findByID( tank->getLocationID() );
	if( site == NULL ) {
		TxtLocation1->Text = tank->getStoreType();
	} else {
		TxtLocation1->Text = site->getDescription().c_str();
	}
	TxtPos1->Text = tank->getPosition();

	const LCDbObject * type = names.findByID( tank->getStoreType() );
	if( type == NULL ) {
		txtStoreType1->Text = tank->getStoreType();
	} else {
		txtStoreType1->Text = type->getDescription().c_str();
	}
	Layout * lay = layman.getLayout( layman.find( layout_cid ) );
	short shelf = tank->getShelfNumber();
	TxtLayName1->Text = lay->getLayoutDescription().c_str();
	TxtPop1->Text = shelf;
	ShowGrid( grdProps1, lay->getList() );
	if( mode == NEW_VESSEL || shelf != oldtank->getShelfNumber()
	 || mode == ADD_LAYOUT || layout_cid != oldtank->getLayoutID() ) {
		std::string population = getNextPopulation();
		std::string longName = "Population " + population + " (" + lay->getName() + ")";
		tank->setContent( population, longName );
		TxtFull1->ReadOnly = false;
		ActiveControl = TxtFull1;
	} else {
		TxtFull1->ReadOnly = true;
	}
	TxtName1->Text = tank->getContent().c_str();
	TxtFull1->Text = tank->getDescription().c_str();
}

//---------------------------------------------------------------------------

Tank * TfrmNewTank::saveDetails()
{
	Layout * lay = layman.getLayout( layman.find( layout_cid ) );
	lay->setAvailability( tank->availability() == Tank::IS_AVAILABLE );
	tank->addToList( lay );
	AnsiString pop = TxtName1->Text.Trim().UpperCase();
	AnsiString desc = TxtFull1->Text.Trim();
	tank->setContent( pop.c_str(), desc.c_str() );
	bool success = tank->save();
	const std::vector<IPart*> & list = lay->getList();
	for( int i = 0; i < (int)list.size() && success; i++)
	{
		IPart* childdata = list[i];
		if( childdata->isNew() )
		{
			childdata->setParent(lay);
			success = childdata->save();
		}
	}

	if( oldtank != NULL ) {
		oldtank->clearList();
	}
	tank->discardList();
	layman.clearList();

	if( success ) {
		return tank;
	} else {
		delete tank;
		return NULL;
	}
}

//---------------------------------------------------------------------------

void __fastcall TfrmNewTank::CancelClick(TObject *Sender)
{
	if (Application->MessageBox(L"Do you want to discard changes? ",
				L"Warning", MB_ICONWARNING|MB_YESNO|MB_DEFBUTTON2) == IDYES) {
		layman.clearList();
		delete tank;
		ModalResult = mrCancel;
	}
}

//---------------------------------------------------------------------------

const LCDbObject * TfrmNewTank::findStorageType() const {
	LCDbObjects & cache = LCDbObjects::records();
	AnsiString type =  cbStoreType->Text.Trim();
	const LCDbObject * st = cache.find( type.c_str(), LCDbObject::STORAGE_TYPE );
	if( st == NULL
	 && Application->MessageBox( L"Add new storage type?", L"No storage type", MB_ICONQUESTION|MB_YESNO ) == IDYES
	 && frmNewStoreType -> ShowModal() == mrOk ) {
		st = frmNewStoreType -> createRecord();
	}
	if( st != NULL ) {
		cbStoreType -> Text = st -> getDescription().c_str();
	}
	return st;
}

//---------------------------------------------------------------------------

int TfrmNewTank::getDefaultPosition() const
{
	TTreeNode* child = SiteNode->getFirstChild();
	int pos = 0;
	while( child != NULL ) {
		Tank* tank = (Tank*) child->Data;
		if( tank != NULL && tank->getPosition() > pos ) {
			pos = tank->getPosition();
		}
		child = SiteNode->GetNextChild( child );
	}
	return pos + 1;
}

//---------------------------------------------------------------------------

int TfrmNewTank::getDefaultLayoutIndex() const
{
	TTreeNode* child = SiteNode->getFirstChild();
	int ind = -1;
	if( child != NULL && child->Data != NULL ) {
		Tank* a_tank = (Tank*) child->Data;
		ind = layman.getDefaultLayoutId(a_tank->getID());
	}
	return layman.find( ind );
}

//---------------------------------------------------------------------------

int TfrmNewTank::getDefaultStoreTypeID() const
{
	TTreeNode* child = SiteNode->getFirstChild();
	if( child == NULL || child->Data == NULL ) {
		return 0;
	} else {
		Tank* a_tank = (Tank*)(child->Data);
		return a_tank->getStoreType();
	}
}

//---------------------------------------------------------------------------

void TfrmNewTank::ShowGrid(TStringGrid *grid, std::vector<IPart*> list)
{
	Util::ClearPropertyGrid(grid);
	int n = list.size();
	for( int i = 0; i < n; i++ ) {
		ShowGridRow( grid, i+1, (Section*)list[i] );
	}
	grid->RowCount = grid->FixedRows + n;
	BtnAdd->Visible = (n < nSections);
}

//---------------------------------------------------------------------------

void TfrmNewTank::ShowGridRow(TStringGrid *grid, int row, Section* data)
{
	grid->Cells[ 0 ][ row ] = data->getName().c_str();
	grid->Cells[ 1 ][ row ] = AnsiString( data->getPosition() );
	grid->Cells[ 2 ][ row ] = AnsiString( data->getFirstRack() );
	grid->Cells[ 3 ][ row ] = AnsiString( data->getLastRack() );
	grid->Cells[ 4 ][ row ] = AnsiString( data->getRackCapacity() );
}

//---------------------------------------------------------------------------

void TfrmNewTank::GridHeadings(TStringGrid *grid)
{
	grid->Cells[0][0] = "Prefix";
	grid->Cells[1][0] = "Order";
	grid->Cells[2][0] = "First";
	grid->Cells[3][0] = "Last";
	grid->Cells[4][0] = "# Slots";

	int colWidth = int((grid->ClientWidth - 20) * 0.2);
	for( int i = 0; i < 5; ++ i )
		grid->ColWidths[ i ] = colWidth;
}
//---------------------------------------------------------------------------

void __fastcall TfrmNewTank::BackClick(TObject *Sender)
{
	if( pnlConfirm->Visible && layout_cid == -1 ) {
		LayoutInit();
	} else {
		TankInit(NULL);
	}
}

//---------------------------------------------------------------------------

void __fastcall TfrmNewTank::OnShow(TObject *Sender)
{
	GridHeadings( grdProps );
	GridHeadings( grdProps1 );
}

//---------------------------------------------------------------------------

void __fastcall TfrmNewTank::AddClick(TObject *Sender)
{
	nSections = Util::validateInteger( TxtCount, 1, 20 );
	if( nSections == 0 )
		return;

	Layout * newLayout = layman.getLayout( layman.find(layout_cid) );
	if( newLayout->getChildCount() >= nSections )
	{
		String buf;
		buf.sprintf( L"Number of sections cannot exceed # Sections <%d>", nSections );
		Application->MessageBox( buf.c_str(), NULL, MB_ICONWARNING );
		TxtCount->SetFocus();
		return;
	}

	frmNewSection->init( newLayout, NULL );
	frmNewSection->ShowModal();
	newLayout->addToList( frmNewSection->getSection() );
	ShowGrid(grdProps, newLayout->getList());

	CountChange( Sender );
}
//---------------------------------------------------------------------------

void __fastcall TfrmNewTank::EditClick(TObject *Sender)
{
	int row = grdProps->Row;
	if( row > 0 ) {
		Layout * newLayout = layman.getLayout( layman.find(layout_cid) );
		IPart * selected = newLayout->getList()[row-1];
		frmNewSection->init( newLayout, (Section*) selected );
		frmNewSection->ShowModal();
		newLayout->sortChildren();
		ShowGrid(grdProps,newLayout->getList());
	}
}
//---------------------------------------------------------------------------

void __fastcall TfrmNewTank::CountChange(TObject *Sender)
{
	nSections = Util::validateInteger( TxtCount, 1, 20 );
	BtnAdd->Visible = (nSections >= grdProps->RowCount);
}

//---------------------------------------------------------------------------

void __fastcall TfrmNewTank::DeleteClick(TObject *Sender)
{
	int row = grdProps->Row;
	if( row != 0 ) {
		Layout * newLayout = layman.getLayout( layman.find(layout_cid) );
		grdProps->Rows[row]->Clear();
		newLayout -> remove(row-1);
		ShowGrid(grdProps,newLayout -> getList());
	}
	CountChange( Sender );
}

//---------------------------------------------------------------------------
//	suggest name for this population that has not been used before
//---------------------------------------------------------------------------

std::string TfrmNewTank::getNextPopulation() {
	char name[ 12 ];
	int pos = TxtPos->Text.ToIntDef( -1 );
	for( char prefix = 'D'; prefix <= 'Z'; prefix ++ ) {
		std::sprintf( name, "%c%d", prefix, pos );
		for( const LCDbObject & obj : LCDbObjects::records() ) {
			if( obj.getName() != name ) {
				return name;
			}
		}
	}
	Application->MessageBox( L"No suitable population name found", NULL, MB_ICONWARNING );
	return "???";
}

//---------------------------------------------------------------------------

void TfrmNewTank::fillUpLayouts()
{
	layman.loadAll();
	const std::vector<Layout*> & layouts = layman.getList();
	cbLayout->Clear();
	unsigned i = 0;
	while( i < layouts.size() ) {
		cbLayout->Items->Add( layouts[i]->getLayoutDescription().c_str() );
		i ++;
	}
	cbLayout->Items->Add( "(Add new tank layout)" );
	cbLayout->ItemIndex = i;
}

//---------------------------------------------------------------------------

void TfrmNewTank::fillSiteList( int selectedID ) {
	cbLocation->Clear();
	int index = -1;
	for( Range< LCDbObject > oi = LCDbObjects::records(); oi.isValid(); ++oi ) {
		if( oi->isActive() && oi->getObjectType() == LCDbObject::STORAGE_SITE ) {
			if( oi->getID() == selectedID ) {
				index = cbLocation->Items->Count;
			}
			cbLocation->Items->Add( oi->getDescription().c_str() );
		}
	}
	cbLocation->ItemIndex = index;
}

//---------------------------------------------------------------------------

void TfrmNewTank::fillStorageTypes( int selectedID ) {
	cbStoreType->Clear();
	int index = -1;
	for( Range< LCDbObject > oi = LCDbObjects::records(); oi.isValid(); ++oi ) {
		if( oi->isActive() && oi->getObjectType() == LCDbObject::STORAGE_TYPE ) {
			if( oi->getID() == selectedID ) {
				index = cbStoreType->Items->Count;
			}
			cbStoreType->Items->Add( oi->getDescription().c_str() );
		}
	}
	cbStoreType->Items->Add( "(Add new storage type)" );
	cbStoreType->ItemIndex = index;
}

//---------------------------------------------------------------------------

void TfrmNewTank::clearLayoutPanel()
{
	TxtLayName->Text = "";
	TxtLayFull->Text = "";
	UpDownCap->Position = 4;
	Util::ClearPropertyGrid(grdProps);
	CountChange( TxtCount );
}

//---------------------------------------------------------------------------

void __fastcall TfrmNewTank::chkPopClick(TObject *Sender)
{
	TxtPop->Text = chkPop->Checked ? 1 : 0;
}

//---------------------------------------------------------------------------

bool TfrmNewTank::checkVessel( TEdit * textBox, TLabel * label, bool isName ) const {
	AnsiString name =  textBox->Text.Trim();
	if( mode != NEW_VESSEL ) {
		std::string oldValue = isName ? oldtank->getVessel() : oldtank->getSrlno();
		if( oldValue == name.c_str() ) {
			return true;				// no change: no problem
		}
	}
	if( mode == EDIT_VESSEL || mode == NEW_VESSEL ) {
		return Util::validateText( textBox, label ) && !Util::isVesselInUse( name.c_str() );
	} else if( name.IsEmpty() ) {
		Application->MessageBox( L"No vessel at that location", NULL, MB_ICONWARNING );
		return false;
	} else {
		return true;		// adding population to an existing vessel
	}
}

//---------------------------------------------------------------------------

bool TfrmNewTank::checkPosition( int locationCID, short pos, short shelf ) const {
	if( mode != NEW_VESSEL && locationCID == oldtank->getLocationID()
	 && pos == oldtank->getPosition() && shelf == oldtank->getShelfNumber() ) {
		return true;
	}
	if( mode == NEW_VESSEL ) {
		return !Util::isPosDuplicate( locationCID, pos );
	} else {
		return !Util::isPosDuplicate( locationCID, pos, shelf );
	}
}

//---------------------------------------------------------------------------
//	change vessel name+serial number if user changes population's location
//---------------------------------------------------------------------------

void __fastcall TfrmNewTank::TxtPosChange(TObject *Sender)
{
	if( mode == ADD_LAYOUT || mode == MOD_LAYOUT ) {
		changeVessel( cbLocation->Text, TxtPos->Text );
	}
}

//---------------------------------------------------------------------------

void __fastcall TfrmNewTank::cbLocationChange(TObject *Sender)
{
	if( mode == ADD_LAYOUT || mode == MOD_LAYOUT ) {
		changeVessel( cbLocation->Text, TxtPos->Text );
	}
}

//---------------------------------------------------------------------------

void TfrmNewTank::changeVessel( AnsiString site, AnsiString position ) {
	const LCDbObjects & names = LCDbObjects::records();
	std::string serial, friendly;
	const LCDbObject * loc = names.find( site.c_str(), LCDbObject::STORAGE_SITE );
	short pos = position.ToIntDef( -1 );
	if( loc != NULL && pos > 0 ) {
		for( Range< LCDbTankMap >tmi = LCDbTankMaps::records(); tmi.isValid(); ++tmi ) {
			if( tmi->getStatus() < LCDbTankMap::OFFLINE
			 && tmi->getLocationCID() == loc->getID() && tmi->getPosition() == pos ) {
				const LCDbObject * vessel = names.findByID( tmi->getStorageCID() );
				if( vessel != NULL ) {
					serial = vessel->getName();
					friendly = vessel->getDescription();
				}
			}
		}
	}
	TxtSrl->Text = serial.c_str();
	TxtPhysical->Text = friendly.c_str();
}

//---------------------------------------------------------------------------


//---------------------------------------------------------------------------
#include "Utils.h"

#pragma hdrstop
//---------------------------------------------------------------------------


#include "InfoPanels.h"
#include "VisualComponents.h"
#include "WorklistEntriesView.h"
#include "WorklistEntriesPositioning.h"

#pragma package(smart_init)



/*---------------------- begin TInfoPanel functions -------------------*/

/** Destroys the close 'x' button and the area just inside the rounded rectangle.
  */
__fastcall TInfoPanel::~TInfoPanel()
{
	delete xButton;
	delete innerArea;
}

/** Sets up the close 'x' button for the top right of the panel,
  * and positions the inner area just inside the rounded rectangle
  * shape of this panel.  (This can't be done as part of the constructor
  * as the position of the close button depends on which way up the panel is.)
  *
  */
void TInfoPanel::setUpOuterArea()
{

    // area just inside the rounded rectangle
	innerArea = new TLayout(this);


	if (CalloutPosition==TCalloutPosition::cpLeft) {
		innerArea->Position->X = gui->param("edgeGap")
								 + gui->param("calloutTriangleHeight");
	}
	else {
		innerArea->Position->X = gui->param("edgeGap");
	}
	if (CalloutPosition==TCalloutPosition::cpTop) {
		// leave room for the triangle indicator at the top
		innerArea->Position->Y = gui->param("edgeGap")
									  + gui->param("calloutTriangleHeight");
	}
	else {
		innerArea->Position->Y = gui->param("edgeGap");
	}
	if (CalloutPosition==TCalloutPosition::cpLeft
		|| CalloutPosition==TCalloutPosition::cpRight) {
		innerArea->Width = Width - gui->param("calloutTriangleHeight")
						   - 2*gui->param("edgeGap");
		innerArea->Height = Height - 2*gui->param("edgeGap");
	}
	else { // floats up or down
		innerArea->Width = Width - 2*gui->param("edgeGap");
		innerArea->Height = Height - gui->param("calloutTriangleHeight")
							- 2*gui->param("edgeGap");
	}
	innerArea->Parent = this;


	// xButton is the 'x' close button in the top-right corner
	xButton = new TSpeedButton(innerArea);
	xButton->StyleLookup = "XButtonStyle";
	xButton->Text = "";
	xButton->Width = 17;
	xButton->Height = 17;

	// have to do it this way, as neither alRight and alTop alignment options
	// help (e.g. an alTop option will spread the component out widthways)
	xButton->Align = TAlignLayout::alNone;
	xButton->Position->X = innerArea->Width - xButton->Width;
	xButton->Position->Y = 0;

	xButton->Parent = innerArea;
}


/** Bare bones initialisation of this information panel.
  *
  * @param g       should be a link to the GUI manager object
  * @param owner   the component (if any, could be NULL) that will own this panel
  * @param t       the panel containing the worklist entry that this information
  *                will appear to point to
  * @param p       the type of pop-up panel (e.g. panel_NOTES)
  * @param f       the display preference of which area to put this panel in,
  *                relative to where it is pointing to (e.g. Positioning::TOP)
  * @param a       the display preference of the alignment of this panel,
  *                relative to the area it is positioned in (e.g. Position::CENTRE)
  */
__fastcall TInfoPanel::TInfoPanel(WorklistEntriesView *g, TComponent *owner, TTestPanel *t,
								  int p, int f, int a)
	: TCalloutPanel(Owner),
	  panel_type(p),
	  pref_float(f),
	  pref_align(a),
	  gui(g),
	  originator(t)   // t is the test panel from which the button was clicked,
					  // in order to bring up this panel in the first place
{
  // the rest of the setup will be initiated/done by the derived classes
}


/*---------------------- end TInfoPanel functions -------------------*/

/*---------------------- begin TBasicInfoPanel functions -------------------*/


__fastcall TBasicInfoPanel::~TBasicInfoPanel()
{
	delete basicInfoListBox;
}

/** Adds a given string to the list box on the information panel.
  *
  * @param s   the string to be added to the list box
  */
void TBasicInfoPanel::addListItem(const std::string &s) {
	TListBoxItem *item = new TListBoxItem(basicInfoListBox);
	item->StyleLookup = "AlertListBoxItemStyle";
	item->Height = gui->param("panelAlertItemHeight");
	item->Text = Utils::str2unicodestr(s);
	item->Parent = basicInfoListBox;
}

/** Makes a pop-up panel, to contain basic information about a worklist entry.
  *
  * @param g        the object that manages the GUI
  * @param owner    the component (if any, could be NULL) that will own the panel
  * @param t        the test panel from which the button was clicked, 
  *                 in order to bring up this panel in the first place
  */
__fastcall TBasicInfoPanel::TBasicInfoPanel(WorklistEntriesView *g, TComponent *owner, TTestPanel *t)
	: TInfoPanel(g,owner,t,panel_BASIC,
				 Positioning::TOP,Positioning::LEFT)
{
	StyleLookup = "InfoPanelStyle";
	innerSize.height = gui->param("popupPanelHeight") - gui->param("calloutTriangleHeight");
	innerSize.width = gui->param("popupPanelWidth");
	Visible = true;

	// contents get added later

}

/** Initialises the contents of this pop-up panel. Currently, just
  * positions the xButton.
  *
  * @see WorklistEntriesView::positionInfoPanel
  */
void TBasicInfoPanel::initialiseContents()
{
	xButton->OnClick = removeClick;

	// then we need a list box
	basicInfoListBox = new TListBox(innerArea);
	basicInfoListBox->StyleLookup = "InfoListStyle";
	basicInfoListBox->Width = innerArea->Width - xButton->Width - gui->param("edgeGap");
	basicInfoListBox->Height = innerArea->Height;
	basicInfoListBox->Parent = innerArea;

	// and some information items to put in it

	addListItem("Test: " + originator->testName);

	for (auto it = originator->ids->cbegin();
		 it != originator->ids->cend(); ++it) {
		std::string label = (*it).first + ": "
							 + Utils::int2str((*it).second);
		addListItem(label);
	}
	for (auto it = originator->attributes->cbegin();
		 it != originator->attributes->cend(); ++it) {
		std::string label = (*it).first + ": " + (*it).second;
		addListItem(label);
	}

}

void __fastcall TBasicInfoPanel::removeClick(TObject *Sender) {

	TSpeedButton *x = (TSpeedButton *)Sender; // x is the button clicked on
	TLayout *a = (TLayout *)(x->Parent);      // a is the innerArea it's in
	TBasicInfoPanel *p = (TBasicInfoPanel *)(a->Parent); // p is the info panel
														 // it belongs to
	if (p!=NULL) {
		gui->removeInfoPanel(p);
	}

}

/*---------------------- end TBasicInfoPanel functions -------------------*/


/*---------------------- begin TNotesPanel functions -------------------*/


__fastcall TNotesPanel::~TNotesPanel()
{
	delete notesListBox;
}


/** Makes a pop-up panel, to contain basic information about a worklist entry.
  *
  * @param g       the object that manages the GUI
  * @param owner   the component (if any, could be NULL) that will own this panel
  * @param t       the test panel from which the button was clicked, in order to bring up this panel in the first place
  */
__fastcall TNotesPanel::TNotesPanel(WorklistEntriesView *g, TComponent *owner, TTestPanel *t)
	: TInfoPanel(g,owner,t,panel_NOTES,
				 Positioning::BOTTOM,Positioning::CENTRE)
{
	StyleLookup = "InfoPanelStyle";
	innerSize.height = gui->param("popupPanelHeight") - gui->param("calloutTriangleHeight");
	innerSize.width = gui->param("popupPanelWidth");
	Visible = true;

	// contents get added later

}


/** Adds a given string to the list box on the information panel.
  *
  * @param s   the string to be added to the list box
  */
void TNotesPanel::addListItem(const std::string &n) {
	TListBoxItem *item = new TListBoxItem(notesListBox);
	item->StyleLookup = "AlertListBoxItemStyle";
	item->WordWrap = true;
	// item->Height = gui->param("panelAlertItemHeight");
	item->Text = Utils::str2unicodestr(n);
	item->Parent = notesListBox;
}

/** Initialises the contents of this pop-up panel.
  *
  * @see WorklistEntriesView::positionInfoPanel
  */
void TNotesPanel::initialiseContents()
{
	xButton->OnClick = removeClick;


	// then we need a list box
	notesListBox = new TListBox(innerArea);
	notesListBox->StyleLookup = "InfoListStyle";
	notesListBox->Width = innerArea->Width - xButton->Width - gui->param("edgeGap");
	notesListBox->Height = innerArea->Height;
	notesListBox->Parent = innerArea;
    addListItem("[dummy note]");
	/*
	// and some information items to put in it
	std::list<DNote>::const_iterator iter = originator->entry->notes.begin();
	while (iter!=originator->entry->notes.end()) {
		const DNote *n = &*iter;
		addListItem(*n);
		iter++;
	}
	*/

}

void __fastcall TNotesPanel::removeClick(TObject *Sender) {

	TSpeedButton *x = (TSpeedButton *)Sender; // x is the button clicked on
	TLayout *a = (TLayout *)(x->Parent);      // a is the innerArea it's in
	TNotesPanel *p = (TNotesPanel *)(a->Parent); // p is the info panel
														 // it belongs to
	if (p!=NULL) {
		gui->removeInfoPanel(p);
	}

}

/*---------------------- end TNotesPanel functions -------------------*/




/*---------------------- end TActionPanel functions -------------------*/


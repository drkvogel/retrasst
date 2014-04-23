//---------------------------------------------------------------------------
#include "Utils.h"

#pragma hdrstop
//---------------------------------------------------------------------------

#include "Utils.h"
#include "GUIpositioning.h"
#include "InfoPanels.h"
#include "VisualComponents.h"
#include "GUImanager.h"
#include "Main.h"

#pragma package(smart_init)

//---------------------------------------------------------------------------

/** Sets up this Positioning object with a link to the GUImanager.
  *
  * @param g       a pointer to the GUImanager object
  */
Positioning::Positioning(GUImanager *g)
	: gui(g)
{
	minTriOffset = gui->param("calloutTriangleOffset")
				   + gui->param("calloutTriangleWidth")/2;
}

int Positioning::decidePanelLocation(const Block & pointsAt,
									const Block & viewport,
									TInfoPanel *p)
{   // decides whether the panel goes up top, left, right or down
	// relative to where the triangle indicator points to
	// (this is equivalent to deciding which edge of the callout panel
	// the triangle indicator is going to be on)

	int panelLocation;  // to hold the decision about to be made

	// now to mark out possibilities for the pop-up panel location
	float heightIfTB = p->innerSize.height + gui->param("calloutTriangleHeight");
	float widthIfLR = p->innerSize.width + gui->param("calloutTriangleHeight");

	float targetVEdgeCentre = (pointsAt.topLeft.y + pointsAt.bottomRight.y)/2;
	float targetHEdgeCentre = (pointsAt.topLeft.x + pointsAt.bottomRight.x)/2;

	// too high: when putting the panel as low as possible with respect to
	// the triangle indicator would still mean the panel spills over the top edge
	bool targetTooHigh = (targetVEdgeCentre - minTriOffset) <= viewport.topLeft.y;

	// too low: when putting the panel as high as possible with respect to
	// the triangle indicator would still mean the panel spills over the bottom edge
	bool targetTooLow = (targetVEdgeCentre + minTriOffset) >= viewport.bottomRight.y;

	// too leftwards: when putting the panel as rightwards as possible wrt the
	// triangle indicator, the panel would still spill over the left hand side
	bool targetTooLeft = (targetHEdgeCentre - minTriOffset) <= viewport.topLeft.x;

	// too rightwards: when putting the panel as leftwards as possible wrt the
	// triangle indicator, the panel would still spill over the right hand side
	bool targetTooRight = (targetHEdgeCentre + minTriOffset) >= viewport.bottomRight.x;

	bool canFloatUpWrtInnerPanel = (pointsAt.topLeft.y - heightIfTB >= 0);  // without falling off the edge
	bool roomUpwardsWrtViewport = (pointsAt.topLeft.y - heightIfTB
								  >= viewport.topLeft.y);
	bool canFloatUpWrtViewport = roomUpwardsWrtViewport
								 && !targetTooLeft && !targetTooRight;

	bool roomBelowWrtViewport = (pointsAt.bottomRight.y + heightIfTB
									 < viewport.bottomRight.y);
	bool canFloatBelowWrtViewport = roomBelowWrtViewport
								 && !targetTooLeft && !targetTooRight;

	bool canFloatLeftWrtInnerPanel = (pointsAt.topLeft.x - widthIfLR >=0);

	bool roomOnLeftWrtViewport = (pointsAt.topLeft.x - widthIfLR
									>= viewport.topLeft.x);
	bool canFloatLeftWrtViewport = roomOnLeftWrtViewport
								   && !targetTooHigh && !targetTooLow;

	bool roomOnRightWrtViewport = (pointsAt.bottomRight.x + widthIfLR
									 < viewport.bottomRight.x);

	bool canFloatRightWrtViewport = roomOnRightWrtViewport
									&& !targetTooHigh && !targetTooLow;

	// Note: can always hang below wrt inner panel, or on the right wrt the
	// inner panel, because can always extend the size of the panel, if necessary


	if (p->pref_float==TOP) {
		if (canFloatUpWrtViewport) {
			panelLocation = TOP;
		}
		else if (canFloatBelowWrtViewport) {
			panelLocation = BOTTOM;
		}
		else if (canFloatRightWrtViewport) {
			panelLocation = RIGHT;
		}
		else if (canFloatLeftWrtViewport) {
			panelLocation = LEFT;
		}
		else { // very small height of viewport
			   // user will have to cope!
			if (canFloatUpWrtInnerPanel) {
				panelLocation = TOP;
			}
			else { // even if the panel is also small,
				   // if the user would kindly lower the splitter...
				panelLocation = BOTTOM;
			}
		}
	}
	else if (p->pref_float==BOTTOM) {
		if (canFloatBelowWrtViewport) {
			panelLocation = BOTTOM;
		}
		else if (canFloatUpWrtViewport) {
			panelLocation = TOP;
		}
		else if (canFloatRightWrtViewport) {
			panelLocation = RIGHT;
		}
		else if (canFloatLeftWrtViewport) {
			panelLocation = LEFT;
		}
		else { // very small height of viewport
			   // - user will have to lower the splitter or scroll down
			panelLocation = BOTTOM;
		}
	}
	else if (p->pref_float==RIGHT) {
		if (canFloatRightWrtViewport) {
			panelLocation = RIGHT;
		}
		else if (canFloatLeftWrtViewport) {
			panelLocation = LEFT;
		}
		else if (canFloatBelowWrtViewport) {
			panelLocation = BOTTOM;
		}
		else if (canFloatUpWrtViewport) {
			panelLocation = TOP;
		}
		else {  // small width of window
				// user will have to scroll over to the right
			panelLocation = RIGHT;
		}
	}
	else if (p->pref_float==LEFT) {
		if (canFloatLeftWrtViewport) {
			panelLocation = LEFT;
		}
		else if (canFloatRightWrtViewport) {
			panelLocation = RIGHT;
		}
		else if (canFloatBelowWrtViewport) {
			panelLocation = BOTTOM;
		}
		else if (canFloatUpWrtViewport) {
			panelLocation = TOP;
		}
		else {  // can only guarantee having enough space to the right
			panelLocation = RIGHT;
		}
	}

	return panelLocation;
}

float Positioning::getHorizontalPanelAlignment(const Block & pointsAt,
											  const Block & viewport,
											  TInfoPanel *p)
{   // assuming that the panel is being positioned above or below
	// the area which it points at, this method decides how far
	// left<->right the panel p is relative to where the triangle indicator
	// points to

	// it is x we have to decide (for the x-coordinate of the top-left corner
	// of the panel)
	float x;

	// this is the x-coordinate for where the triangle indicator goes
	float triX = (pointsAt.topLeft.x + pointsAt.bottomRight.x)/2;

	// find the x position of the panel if panel as right as possible
	float rightmostX = triX - minTriOffset;
	bool canGoRightmostWrtViewport = (rightmostX + p->innerSize.width
									  < viewport.bottomRight.x);

	// find the x position of the panel if panel as left as possible
	float leftmostX = triX - (p->innerSize.width - minTriOffset);
	bool canGoLeftmostWrtViewport = (leftmostX >= viewport.topLeft.x);

	// this is the x position of the panel when it's centred on posX
	float centreX = triX - p->innerSize.width/2;  //
	bool canCentreWrtViewport = (centreX >= viewport.topLeft.x)
		   && (centreX + p->innerSize.width <= viewport.bottomRight.x);

	// this is the x position of the panel when it's centred wrt the viewport
	float centreXwrtV = (viewport.topLeft.x + viewport.bottomRight.x
						 - p->innerSize.width)/2;

	float viewportWidth = viewport.bottomRight.x-viewport.topLeft.x;

	if (p->pref_align==RIGHT) {
		if (canGoRightmostWrtViewport) { // will fit on right, within viewport
			x = rightmostX;
		}
		else if (canCentreWrtViewport) {  // could centre , so do so
			x = centreX;
		}
		else if (canGoLeftmostWrtViewport) { // will fit on left, within viewport
			x = leftmostX;
		}
		else {
			// is the viewport wide enough, if it won't go on the right or left?
			if (viewportWidth >= p->innerSize.width) {  // it's wide enough
				x = centreXwrtV; // eh, plonk it in the middle
			}
			else { 	// oooo, what a narrow window!
				x = rightmostX;  // make it right-oriented
			}
		}
	}
	else if (p->pref_align==LEFT) {
		if (canGoLeftmostWrtViewport) { // will fit on left, within viewport
			x = leftmostX;
		}
		else if (canCentreWrtViewport) {  // could centre, so do so
			x = centreX;
		}
		else if (canGoRightmostWrtViewport) { // will fit on right, within viewport
			x = rightmostX;
		}
		else {
			// is the viewport wide enough, if it won't go on the right or left?
			if (viewportWidth >= p->innerSize.width) {  // it's wide enough
				x = centreXwrtV; // eh, plonk it in the middle
			}
			else { 	// oooo, what a narrow window!
				x = rightmostX;  // make it right-oriented
			}
		}
	}
	else { // maybe pref_align==CENTRE, or maybe TOP or BOTTOM,
	       // if the panel was hoping to be placed left or right
		if (canCentreWrtViewport) {  // will centre
			x = centreX;
		}
		else if (canGoRightmostWrtViewport) { // will fit on right, within viewport
			x = rightmostX;
		}
		else if (canGoLeftmostWrtViewport) { // will fit on left, within viewport
			x = leftmostX;
		}
		else {
			// is the viewport wide enough, if it won't go on the right or left?
			if (viewportWidth >= p->innerSize.width) {  // it's wide enough
				x = centreXwrtV; // eh, plonk it in the middle
			}
			else { 	// oooo, what a narrow window!
				x = rightmostX;  // make it right-oriented
			}
		}
	}
	return x;

}

/** This method decides the y coordinate of the pop-up panel.
  * This assumes that it has already been decided that the panel is
  * going to be above or below the place it points to.
  *
  * @param pointsAt   the rectangular target area to point at
  * @param viewport   the area of the viewport
  * @param p          the panel to be positioned
  * @return           the y coordinate for the panel
  */
float Positioning::getVerticalPanelAlignment(const Block & pointsAt,
											const Block & viewport,
											TInfoPanel *p)
{
	float y;  // it is y we have to decide

	// this is the y-coordinate for where the triangle indicator goes
	float posY = pointsAt.topLeft.y
				 + (pointsAt.bottomRight.y - pointsAt.topLeft.y)/2;

	// first figure out some positions and what's possible

	// this is the y position of the panel, if it's as high as poss
	float topMostY = posY - (p->innerSize.height - minTriOffset);
	bool canGoTopMostWrtViewport = (topMostY >= viewport.topLeft.y);

	// this is the y position of the panel, if it's as low as poss
	float bottomMostY = posY - minTriOffset;  // y pos if panel as low as poss
	bool canGoBottomMostWrtViewport = (bottomMostY + p->innerSize.height
									   < viewport.bottomRight.y);


	// this is the y pos of the panel when it's centred on posY
	float centreY = posY - p->innerSize.height/2;

	bool canCentreWrtViewport = (posY - p->innerSize.height/2
								 >= viewport.topLeft.y)
								 && (posY + p->innerSize.height/2
								 <= viewport.bottomRight.y);

	// this is the y pos of the panel, if centred wrt viewport
	float centreYwrtV = (viewport.bottomRight.y+viewport.topLeft.y
						 - p->innerSize.height)/2;


	float viewportHeight =  viewport.bottomRight.y-viewport.topLeft.y;

	if (p->pref_align==BOTTOM) {
		if (canGoBottomMostWrtViewport) { // will fit at bottom, within viewport
			y = bottomMostY;
		}
		else if (canCentreWrtViewport) {  // could centre , so do so
			y = centreY;
		}
		else if (canGoTopMostWrtViewport) { // will fit at top, within viewport
			y = topMostY;
		}
		else {
			// is the viewport tall enough, if it won't go on the top or bottom?
			if (viewportHeight >= p->innerSize.height) {  // it's tall enough
				y = centreYwrtV; // eh, plonk it in the middle
			}
			else { 	// oooo, what a short window!
				y = bottomMostY;  // plonk it at the bottom
			}
		}
	}
	else if (p->pref_align==TOP) {
		if (canGoTopMostWrtViewport) { // will fit on left, within viewport
			y = topMostY;
		}
		else if (canCentreWrtViewport) {  // could centre, so do so
			y = centreY;
		}
		else if (canGoBottomMostWrtViewport) { // will fit on right, within viewport
			y = bottomMostY;
		}
		else {
			// is the viewport tall enough, if it won't go on the top or bottom?
			if (viewportHeight >= p->innerSize.height) {  // it's tall enough
				y = centreYwrtV; // eh, plonk it in the middle
			}
			else { 	// oooo, what a short window!
				y = bottomMostY;  // plonk it at the bottom
			}
		}
	}
	else { // maybe pref_align==CENTRE, or maybe LEFT or RIGHT,
		   // in which case the panel was hoping to be placed TOP or BOTTOM

		if (canCentreWrtViewport) {  // will centre
			y = centreY;
		}
		else if (canGoBottomMostWrtViewport) { // will fit on right, within viewport
			y = bottomMostY;
		}
		else if (canGoTopMostWrtViewport) { // will fit on left, within viewport
			y = topMostY;
		}
		else {
			// is the viewport tall enough, if it won't go on the top or bottom?
			if (viewportHeight >= p->innerSize.height) {  // it's tall enough
				y = centreYwrtV; // eh, plonk it in the middle
			}
			else { 	// oooo, what a short window!
				y = bottomMostY;  // plonk it at the bottom
			}
		}
	}

	return y;
}


/** Sets the positioning details for the given panel, given the decisions,
  * which have already been made.
  *
  * @param p              the panel to be positioned
  * @param pos            the position that is being pointed to by the pop-up panel
  * @param panelLocation  whether the panel floats top/bottom/left/right
  *                       relative to where it points
  * @param edge           holds the location of the relevant edge. If the panel
  *                       is floating at top/bottom, then edge.x holds the location
  *                       of the left-hand edge. Otherwise, edge.y holds the y
  *                       coordinate of the top of the panel.
  *
  * @return     the full details for where the pop-up panel is to be positioned
  */
PopupPositioning Positioning::assemblePosition(const Block & pointsAt,
											  TInfoPanel *p,
											  const Coordinates edge,
											  int panelLocation)
{

	// thiese are the coordinates for where the triangle indicator goes



	PopupPositioning smartPos; // to hold the position chosen

	if (panelLocation==TOP || panelLocation==BOTTOM) {
		// x coordinate for triangle indicator
		float posX = (pointsAt.topLeft.x + pointsAt.bottomRight.x)/2;
		smartPos.size.width = p->innerSize.width;
		smartPos.size.height = p->innerSize.height
							   + gui->param("calloutTriangleHeight");
		smartPos.topLeft.x = edge.x;
		if (panelLocation==TOP) {
			smartPos.triLoc = TCalloutPosition::cpBottom; // triangle on bottom
			smartPos.topLeft.y = pointsAt.topLeft.y - smartPos.size.height;
		}
		else { // panelLocation==position_BOTTOM
			smartPos.triLoc = TCalloutPosition::cpTop; // triangle on top
			smartPos.topLeft.y = pointsAt.bottomRight.y;
		}
		// to position triangle, need to think whether right- or left- oriented
		float leftDist = posX - smartPos.topLeft.x;
		float rightDist = smartPos.topLeft.x + smartPos.size.width - posX;
		bool rightOriented = (rightDist > leftDist);
		if (rightOriented) {   // triangle on the left
			smartPos.triOffset = leftDist;
		}
		else { // triangle over to the right
			smartPos.triOffset = -rightDist;
		}
	}
	else { // panel is on the LEFT or RIGHT
		// y coordinate for triangle indicator
		float posY = pointsAt.topLeft.y
				 + (pointsAt.bottomRight.y - pointsAt.topLeft.y)/2;

		smartPos.size.width = p->innerSize.width
							  + gui->param("calloutTriangleHeight");
		smartPos.size.height = p->innerSize.height;

		smartPos.topLeft.y = edge.y;
		if (panelLocation==LEFT) {
			smartPos.triLoc = TCalloutPosition::cpRight; // triangle on the right
			smartPos.topLeft.x = pointsAt.topLeft.x - smartPos.size.width;
		}
		else { // panelLocation==RIGHT
			smartPos.triLoc = TCalloutPosition::cpLeft; // triangle
			smartPos.topLeft.x = pointsAt.bottomRight.x;
		}
		// to position triangle, need to think whether up- or down- oriented
		float topDist = posY - smartPos.topLeft.y;
		float bottomDist = smartPos.topLeft.y + smartPos.size.height - posY;
		bool downOriented = (bottomDist > topDist);
		if (downOriented) { // triangle towards the top
			smartPos.triOffset = topDist;
		}
		else { // triangle towards the bottom
            smartPos.triOffset = bottomDist;
		}
	}

	return smartPos;
}



/** Calculates where would be the best place to put a pop-up information panel.
  * It choose a position that avoids falling off the edge of the panel,
  * and where possible, chooses a positions for which the panel
  * would appear within the scroll view.
  *
  * @param pos  the position the triangle indicator will point to
  * @param fh   the height of the text label
  * @param p    the popup panel to be positioned
  * @param ip   the panel inner to the scrollpane, where the popup will live
  *
  * @todo  Taking into account other pop-up panels hanging about the place.
  * @todo  Add a heuristic to generally try to avoid covering up the barcode.
  */
PopupPositioning Positioning::smartPanelPosition(const Block & pointsAt,
												TInfoPanel *p, TPanel *ip)
{	// will use a slightly more advanced positioning, depending on the
	// scrollport but not any other panels

	// ip is unused at the moment but will get used once we try and get
	// panels to dodge other panels

	/*  For debugging purposes:

	TPointF vpos = mainForm->ResultsScrollBox->ViewportPosition;
	TRectF r = mainForm->ResultsScrollBox->ClipRect;
	UnicodeString s = "ip is " + Utils::int2unicodestr(ip->Width)
					  + "x" + Utils::int2unicodestr(ip->Height)
					  + ", p is " + Utils::int2unicodestr(p->Width)
					  + "x" + Utils::int2unicodestr(p->Height)
					  + ", pos is (" + Utils::int2unicodestr(pos.x)
					  + "," + Utils::int2unicodestr(pos.y) + ")"
					  + ", and scrollport position is ("
					  +  Utils::int2unicodestr(vpos.x)
					  + "," + Utils::int2unicodestr(vpos.y) + ")"
					  + ", clipRect is " + Utils::float2unicodestr(r.Width())
					  + "x" + Utils::int2unicodestr(r.Height());
	mainForm->StatusLabel->Text = s;
			*/


	// inner panel size is ip->Width by ip->Height
	// popup panel size is p->Width by p->Height
	// lhs of viewport is mainForm->ResultsScrollBox->ViewPortPosition.x from ip edge
	// top of viewport is mainForm->ResultsScrollBox->ViewPortPosition.y from ip top
	// width of viewport is mainForm->ResultsScrollBox->ClipRect.Width() ?
	// height of viewport is mainForm->ResultsScrollBox->ClipRect.Height() ?
	// position that triangle points to is (pos.x,pos.y)

	// see where the viewport is
	Block viewport;
	viewport.topLeft.x = gui->mainForm->ResultsScrollBox->ViewportPosition.x;
	viewport.topLeft.y = gui->mainForm->ResultsScrollBox->ViewportPosition.y;
	viewport.bottomRight.x = viewport.topLeft.x + gui->mainForm->ResultsScrollBox->ClipRect.Width();
	viewport.bottomRight.y = viewport.topLeft.y + gui->mainForm->ResultsScrollBox->ClipRect.Height();

	// Two decisions to make:
	// (1) where does it float?
	int panelLocation = decidePanelLocation(pointsAt,viewport,p);

	// (2) alignment wrt anchored triangle (i.e. left<->right or up<->down)
	Coordinates edge;

	if (panelLocation==TOP || panelLocation==BOTTOM) {
		edge.x = getHorizontalPanelAlignment(pointsAt,viewport,p);
	}
	else {  // must be LEFT or RIGHT
		edge.y = getVerticalPanelAlignment(pointsAt,viewport,p);
	}
	// decisions all made, just need to assemble the numbers required
	return assemblePosition(pointsAt,p,edge,panelLocation);
}

/** Specifies the rectangular area where a pop-up panel should point to.
  * For example, if the panel is to point at a number, then the
  * rectangle would be the bounding rectangle of the text displaying the number.
  *
  * @param t       the test result panel with which the pop-up panel is to be positioned
  * @param panel   the pop-up panel to be positioned
  * @return        the rectangle bounding the area that the panel should point to
  */
Block Positioning::areaToPointAt(TTestPanel *t, TInfoPanel *panel)
{
	Coordinates tPos = gui->findTestPanelPosition(t); // position of top-left corner
												 // relative to its inner panel

	Block pointsAt;

	int overlap = 2;
	if (panel->panel_type==TInfoPanel::panel_BASIC) {
		pointsAt.topLeft.x = tPos.x + t->testNameButton->Position->X + overlap;
		pointsAt.topLeft.y = tPos.y + t->testNameButton->Position->Y + overlap;
		pointsAt.bottomRight.x = tPos.x + t->testNameButton->Position->X
								 + t->testNameButton->Width - overlap;
		pointsAt.bottomRight.y = tPos.y + t->testNameButton->Position->Y
								 + t->testNameButton->Height - overlap;
	}
	else if (panel->panel_type==TInfoPanel::panel_NOTES) {
		pointsAt.topLeft.x = tPos.x + t->notesButton->Position->X + overlap;
		pointsAt.topLeft.y = tPos.y + t->notesButton->Position->Y + overlap;
		pointsAt.bottomRight.x = tPos.x + t->notesButton->Position->X
								 + t->notesButton->Width - overlap;
		pointsAt.bottomRight.y = tPos.y + t->notesButton->Position->Y
								 + t->notesButton->Height - overlap;
	}
	else { // (panel->panel_type==TInfoPanel::panel_ACTION)
		pointsAt.topLeft.x = tPos.x + t->resultButton->Position->X + overlap;
		pointsAt.topLeft.y = tPos.y + t->resultButton->Position->Y + overlap;
		pointsAt.bottomRight.x = tPos.x + t->resultButton->Position->X
								 + t->resultButton->Width - overlap;
		pointsAt.bottomRight.y = tPos.y + t->resultButton->Position->Y
								 + t->resultButton->Height - overlap;
	}
	return pointsAt;
}

/** Positions the given pop-up panel at an appropriate place.
  * If the pop-up panel would otherwise hang off the end of the panel
  * that is its parent (the one just inside the scroll view) then its
  * parent is extended to fit.
  *
  * @param panel      the information panel to be positioned
  * @todo             position the notes panel correctly wrt speedbutton
  */
void Positioning::positionInfoPanel(TInfoPanel *panel)
{

	// first, find where this calloutpanel is going to need to point to

	TTestPanel *t = panel->originator;   // the test result panel this info panel
	                                     // visually points to
	TPanel *ip = (TPanel *)(gui->findInnerPanelAncestor(t)); // e.g. ResultsInnerPanel

	// now to find the coordinates of where the triangle indicator will point to
	Block pointsAt = areaToPointAt(t,panel);

	// now do the smart positioning with respect to those parameters
	PopupPositioning smartPos = smartPanelPosition(pointsAt,panel,ip);

	panel->BeginUpdate();
	// now to set the position values for the callout panel and its triangle
	panel->Width = smartPos.size.width;
	panel->Height = smartPos.size.height;

	panel->Position->X = smartPos.topLeft.x;
	panel->Position->Y = smartPos.topLeft.y;
	panel->CalloutPosition = smartPos.triLoc;

	if (smartPos.triOffset>0) {  // triangle offset from lhs
		panel->CalloutOffset = smartPos.triOffset - gui->param("calloutTriangleWidth")/2;
	}
	else {
		panel->CalloutOffset = smartPos.triOffset + gui->param("calloutTriangleWidth")/2;
    }
	panel->CalloutWidth = gui->param("calloutTriangleWidth");
	panel->CalloutLength = gui->param("calloutTriangleHeight");

	panel->setUpOuterArea();
	panel->initialiseContents();


	panel->EndUpdate();

	// it is now possible that, depending on where the scrollview is,
	// the panel might now be hanging off the end of the inner panel,
	// so we fix...
	if (panel->Position->X + panel->Width >= ip->Width) {
		ip->Width = panel->Position->X + panel->Width + 2;
	}
	if (panel->Position->Y + panel->Height >= ip->Height) {
		ip->Height = panel->Position->Y + panel->Height + 2;
	}
}

/** Sets the text of a TLabel component, and also sets the width of
  * the component to fit.
  *
  * @param component   the label component to have its text & width set
  * @param label       the text for the label
  */
void Positioning::fitText(TLabel *component, const std::string &label)
{
	component->Text = Utils::str2unicodestr(label);
	component->Width = gui->findTextWidth(label) + 1;
}

/** Changes the x and y coordinates of a label to position it to
  * the right of a given label.
  *
  * @param placed    the component with respect to which the other will be positioned
  * @param placing   the component to position
  */
void Positioning::putRightOf(TLabel *placed, TLabel *placing)
{
	placing->Align = TAlignLayout::alNone;
	placing->Position->X = placed->Position->X + placed->Width;
	placing->Position->Y = placed->Position->Y;
}

/** Changes the x and y coordinates of a label to position it below
  * another given label.
  *
  * @param placed    the component with respect to which the other will be positioned
  * @param placing   the component to position
  */
void Positioning::putLowerThan(TLabel *placed, TLabel *placing)
{
	placing->Align = TAlignLayout::alNone;
	placing->Position->X = placed->Position->X;
	placing->Position->Y = placed->Position->Y
						   + gui->param("labelTextHeight");
}

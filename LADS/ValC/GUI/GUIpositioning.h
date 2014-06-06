//---------------------------------------------------------------------------

#ifndef GUIpositioningH
#define GUIpositioningH
//---------------------------------------------------------------------------

#include <fmx.h>
                            // some forward declarations
							class TInfoPanel;
							class TTestPanel;
							class WorklistEntriesView;

//---------------------------------------------------------------------------

/** Holds a pair of (x,y) coordinates. */
struct Coordinates {
	/** the x value of the coordinate */
	float x;

	/** the y value of the coordinate. 0 is at the top and the y axis goes down */
	float y;
};

/** Holds a pair of width and height, representing dimensions. */
struct Dimensions {
	/** the width */
	float width;

	/** the height */
	float height;
};

/** Holds the specification of a rectangular area. */
struct Block {
	/** the position of the top-left corner of this rectangle */
	Coordinates topLeft;

	/** the position of the bottom-right corner of this rectangle */
	Coordinates bottomRight;

};

/** Expresses a preference for a particular position.
  * For example, suppose a pop-up panel prefers being displayed to the left,
  * centred on what it's pointing to. Then this could be expressed by
  * area=Positioning::LEFT and spot=Positioning::CENTRE
  *
  * More complex preferences can be expressed by a list of PositionPreference.
  */
struct PositionPreference {
	/** the preferred area, e.g. Positioning::TOP */
	int area;
	/** the preferred spot within the preferred area */
	int spot;
};

/** Holds precisely the information need to position a pop-up information panel.
  * This supplies sufficient information to set the parameters of a TCalloutPanel,
  * as the length and width of the triangle indicator are already set in the
  * GUI configuration values.
  */
struct PopupPositioning {

	/** The location of the triangle indicator: on the top (pointing up)
	  * or on the bottom (pointing down). Will either take the value
	  * TCalloutPosition::cpBottom or TCalloutPosition::cpTop
	  */
	TCalloutPosition triLoc;

	/** This is the offset to the centre of the triangle indicator.
	  * Note that this differs from the TCalloutPanel offset, which gives the
	  * distance to the base, not centre, of the triangle.
	  * If triOffset is positive, this will be the offset from the left-hand
	  * side of the panel if the triangle is cpTop or cpBottom, and from the
	  * top of the panel if the triangle is cpLeft or cpRight.
	  * If triOffset is negative, this will be the offset from the
	  * right-hand side of the panel if the triangle is cpTop or cpBottom,
	  * and from the bottom of the panel if the triangle is cpLeft or cpRight.
	  */
	float triOffset;

	/** The coordinates of the top-left corner of the panel.
	  * Note that the dimensions of the panel include the triangle indicator,
	  * thus for a panel with triangle indicator pointing up, at the top,
	  * this coordinate position may well indicate a point in "mid-air" that
	  * isn't very near the apparent rounded corner of the panel.
	  */
	Coordinates topLeft;

	/** The size depends on the position, as it includes the triangle indicator.
	  */
	Dimensions size;
};

/** Provides constants for positioning and alignment, and can also position
  * pop-up panels intelligently with respect to where they have to point to.
  * There is a singleton Positioning object, which is a sidekick for the
  * WorklistEntriesView object.
  */
class Positioning {

private:

	WorklistEntriesView *gui;   // link back to the WorklistEntriesView object, used for parameters

	float minTriOffset; // minimum distance between triangle centre-line and edge
	                    // of panel, useful for seeing if panels will fit

	int decidePanelLocation(const Block & pointsAt,
								   const Block & viewport,
								   TInfoPanel *p);
	float getHorizontalPanelAlignment(const Block & pointsAt,
											 const Block & viewport,
											 TInfoPanel *p);
	float getVerticalPanelAlignment(const Block & pointsAt,
									const Block & viewport,
									TInfoPanel *p);
	PopupPositioning assemblePosition(const Block & pointsAt,
									  TInfoPanel *p,
									  const Coordinates w,
									  int panelLocation);
	PopupPositioning smartPanelPosition(const Block & pointsAt,
										TInfoPanel *p,
										TPanel *pg);
	Block areaToPointAt(TTestPanel *t,TInfoPanel *panel);

public:


	/** used to express a preference for centre positioning or alignment */
	const static int CENTRE = 0;

	/** used to express a preference for left positioning or alignment */
	const static int LEFT = -1;

	/** used to express a preference for right positioning or alignment */
	const static int RIGHT = 1;

	/** used to express a preference for upper positioning or alignment */
	const static int TOP = -2;

	/** used to express a preference for lower positioning or alignment */
	const static int BOTTOM = 2;

	Positioning(WorklistEntriesView *g);

	void positionInfoPanel(TInfoPanel *panel);

	void fitText(TLabel *component, const std::string &label);
	void putRightOf(TLabel *placed, TLabel *placing);
	void putLowerThan(TLabel *placed, TLabel *placing);
};


























#endif

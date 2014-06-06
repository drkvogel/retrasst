//---------------------------------------------------------------------------

#ifndef InfoPanelsH
#define InfoPanelsH
//---------------------------------------------------------------------------

#include <SysUtils.hpp>
#include <Classes.hpp>
#include <FMX.Controls.hpp>
#include <FMX.StdCtrls.hpp>
#include <FMX.Types.hpp>
#include <string>
#include <list>

#include "GUIpositioning.h"

							// some forward declarations
							class TTestPanel;
							class WorklistEntriesView;
							class DNote;




//---------------------------------------------------------------------------

/** A pop-up panel for displaying assorted kinds of information.
  * This class positions and handles a little 'x' button for closing
  * the window, and positions an inner panel. Subclasses handle the contents
  * of the inner panel.
  */
class TInfoPanel : public TCalloutPanel
{
private:
protected:

	WorklistEntriesView *gui;    /// reference to the object that manages the GUI

	__fastcall void setUpOuterArea(TTestPanel *t);

public:
	const static int panel_BASIC = 0; /// indicates a panel with basic information
	const static int panel_NOTES = 1; /// indicates a panel containing a sequence of notes
	const static int panel_ALERT = 2; /// indicates a panel containing alert information
	const static int panel_ACTION = 3; /// indicates a panel containing action opportunities (amongst other things)

	/** Indicates the position where this panel prefers to hang out, relative
	  * to where it is pointing. Thus it indicates the direction of its triangle
	  * indicator. For example, does the panel prefer to float up, with its triangle
	  * pointer indicating down?  In that case, it would take the value
	  * Positioning::TOP.  Soon to be obsolete.
	  */
	const int pref_float;

	/** Indicates where the bulk of the panel prefers to align with, relative to
	  * the direction (given by pref_float). For example, if the panel floats up,
	  * then it might prefer to align as leftwards as it can, in which case it
	  * would take the value Positioning::LEFT. Soon to be obsolete.
	  */
	const int pref_align;

	/** Expresses the list of preferences for where this panel would like to
	  * hang out.  The first of the list will be the most preferred place.
	  */
	std::list<PositionPreference>  positionPreferences;

	/** The type of panel this is. This information gets used for positioning.
	  * At the moment, the type of panel can be deduced from its class, but
	  * this may change in the future, hence the member value.
	  * Possible values are panel_NOTES, etc. (see the constants for this class).
	  */
	const int panel_type;

	/** the component which the triangle indicator appears to point to  */
	TTestPanel *originator;

	/** the close button (with an 'x' on it) in the top-right hand corner,
	  * that makes the panel disappear when clicked on */
	TSpeedButton *xButton;

	/** area just inside the perimeter of the main rounded rectangle (not including
	  * the triangle indicator shape, The whole callout panel includes the
	  * triangle indicator too, but we don't want to put anything there */
	TLayout *innerArea;

	/** Stores the dimensions of the rounded rectangle of the panel.
	  * That is, just the rectangle bit, not including the triangle indicator.
	  * (FireMonkey measures height and width by the smallest rectangle needed
	  * to enclose a callout panel, thus including the triangle indicator.)
	  * This is useful for calculating where there is room to pop-up a panel.
	  */
	Dimensions innerSize;

	virtual void initialiseContents() = 0;
	void setUpOuterArea();
	__fastcall ~TInfoPanel();
	__fastcall TInfoPanel(WorklistEntriesView *g, TComponent *owner, TTestPanel *t,
	                      int p, int f, int a);

__published:
};


/** A pop-up panel for displaying basic information about a worklist entry.
  * For example, name of the test, any result it has, status, machine, project etc.
  *
  * @image html basic-info.png "A recent example of a basic information panel."
  */
class TBasicInfoPanel : public TInfoPanel
{
private:
	void addListItem(const std::string &s);
protected:
public:

	/** the listbox holding the pieces of basic information */
	TListBox *basicInfoListBox;


	__fastcall ~TBasicInfoPanel();
	__fastcall TBasicInfoPanel(WorklistEntriesView *g, TComponent *owner, TTestPanel *t);
	void initialiseContents();
	void __fastcall removeClick(TObject *Sender);

};



/** A pop-up panel for displaying any notes concerning a worklist entry.
  *
  * @image html notes.png "A recent example of a (placeholder) notes panel."
  */
class TNotesPanel : public TInfoPanel
{
private:
	void addListItem(const std::string &n);
protected:
public:

	/** the listbox holding the notes */
	TListBox *notesListBox;



	__fastcall ~TNotesPanel();
	__fastcall TNotesPanel(WorklistEntriesView *g, TComponent *owner, TTestPanel *t);
	void initialiseContents();
	void __fastcall removeClick(TObject *Sender);
};






#endif

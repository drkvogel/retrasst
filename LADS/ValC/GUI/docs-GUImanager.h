// This file contains doxygen comments giving an overview of the data management 
// in the ValC GUI. There is no code in this file, just documentation. 
// (This comment itself is not a doxygen comment.)


/** @class GUImanager
@brief The main program has a singleton GUImanager object, whose job it is
       to manage all the GUI components, everything except the logging window.
 
The subsidiary classes that the GUI manager has responsibility for are as follows:

 + GUImanager (in GUImanager.cpp) - sets up the required components and manages pop-up panels
 + Positioning (in Positioning.cpp) - positions pop-up panels intelligently and also supplies some alignment/positioning constats
 + TBarcodePanel (in VisualComponents.cpp) - displays the barcode of a sample run
 + TTestPanel (in VisualComponents.cpp) - displays an instance of a test carried out on a sample
   (may also show a result for that test, if completed)
 + TBasicInfoPanel (in InfoPanels.cpp) - pop-up panel displaying basic information about a worklist entry
 + TNotesPanel (in InfoPanels.cpp) - pop-up panel displaying any notes for a test result
 + TActionPanel (in InfoPanels.cpp) -  pop-up panel with information (including alerts)
   and action options for test results
 + TInfoPanel (in InfoPanels.cpp) - the superclass of TActionPanel, TNotesPanel and TBasicInfoPanel

Visual overview:

The main form contains a split pane, separating the ResultsPanel (at the top)
from the WorkPanel (at the bottom). Both of these panels display a list of
sample runs, running top<->bottom. Each sample run is displayed horizontally,
represented by a barcode (a TBarcodePanel) at the left, and a list of
tests carried out for that sample run (each a TTestPanel).

Clicking on various parts of a TTestPanel can bring up some context
information on a test and its result, appearing in a pop-up panel.
The possible pop-up panels are TBasicInfoPanel (for basic information about
a worklist entry), TNotesPanel (for viewing and editing notes) and
a TActionPanel (for taking action on test results). These are positioned
in an intelligent way with respect to the test instance they refer to;
the Positioning class handles the positioning.

 */
  
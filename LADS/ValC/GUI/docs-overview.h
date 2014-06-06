// This file contains doxygen comments giving an overview of the ValC GUI.
// There is no code in this file, just documentation. It is here in a separate 
// file so that it doesn't take up major quantities of space in the Main.cpp file.  
// It is linked to from the comments for the declaration of the main form.
// (This comment itself is not a doxygen comment, the / then ** stuff is doxygen comments.)


/** @mainpage The GUI section of the ValC project

Here's a recent screenshot, showing worklist entries once a Force Reload
has been carried out:

@image html valc-gui-screenshot-latest.png


The main form for the project is contained in Main.cpp.
Very little code is in there; the rest is delegated to assorted managers:

+ a logging manager (an instance of the LogManager class)
+ a data manager (an instance of the DataManager class)
+ a manager for all the GUI components (an instance of the WorklistEntriesView class), 
  except the logging window 

There are also utilities such as number/string conversion in Utils.cpp.

@section config_sec Configuration

There are several configuration files:

+ the main configuration file, <b>config-top.txt</b>  - this contains
  machine/username information, and paths for the other two configuration files
+ a business layer configuration file, <b>config.txt</b>
+ a GUI configuration file, <b>config-gui.txt</b> - contains numerical parameters for size and positioning of visual components  
  
@section log_sec Logging Window
 
There is a singleton log manager object, who responsibility it is to initiate
the logging service. Messages are logged in a text file, and also in a (optional) 
logging window.

For more information about the logging and how the communication with the logging window works, 
please see the documentation for the LogManager and TloggingForm classes. 
 
@section data_sec Data Management

There is a singleton data manager object, whose responsibility it is to load the
data from the snapshot provided by the business layer on a ForceReload.
The data management involves keeping track of sample runs and tests on 
samples, as well as alerts, notes and other information.

For more information about the data management, please see the documentation
for the DataManager class.
  
  
@section visual_sec Visual Components

There is a singleton GUI manager, whose responsibility it is to manage all the visual components
for ValC, except for the logging window.

For more information about the GUI management, please see the documentation
for the WorklistEntriesView class.
  
 

*/
  
  
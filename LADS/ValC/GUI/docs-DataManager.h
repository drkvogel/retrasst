// This file contains doxygen comments giving an overview of the data management 
// in the ValC GUI. There is no code in this file, just documentation. 
// (This comment itself is not a doxygen comment.)


/** @class DataManager
@brief The singleton DataManager object has the job of managing the data
extracted from the database, including loading from the snapshot
provided by the business layer, and clearing data afterwards.

The subsidiary classes used by the DataManager are various 
containers for data, all living in DataContainers.* :

+ DSample - represents a sample run, including information
	  like barcode, and a list of worklist entries/test results for that sample run
+ DSampleTest - an instance of a test being run on a sample, could be a queued
	  worklist entry, or a test put on the analyser, or test with a completed result
+ DNote - represents a note, which will be attached to a test result
+ DAlert - represents QC alert information

The data is managed as follows:

The DataManager maintains two lists of DSample objects: one for queued sample runs,
and one for sample runs with currently-active test results. 
The test results, which can be pending or completed and get represented on the upper half
of the screen, and the queued sample runs get displayed lower down.  
  
Each of the DSample objects represents a sample run (corresponding visually to a
TBarcodePanel object), and contains a list of DSampleTest objects.
Each DSampleTest object represents an instance of a test being run on a sample,
and will usually correspond to a single worklist entry, as most worklist
worklist entries refer to just one test being carried out.
A DSampleTest object will also refer to other information, including alert information
(alert messages being contained in DAlert objects), any notes
that are attached to a test result (a DNote object), and a link
to the visual component that represents it (see TTestPanel, below).

*/  
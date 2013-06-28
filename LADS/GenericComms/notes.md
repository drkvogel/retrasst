#### Todo

add/save/delete enable/disabling
when editing a format, the details are not loaded
order by swipe time - nick's code?    
select format on enter and create  
create sample selection - format for current analyser?  
enter results - format for current analyser?  
copy results to clipboard?  

#### Meeting 2012-09-12 CB, MR, NG

*(note to self: [gitk](http://gitk.sourceforge.net/) does the job for me in  visualizing git commit history)*

---

### Meeting 2012-09-12 - CB, MR, NG ###

#### Worklist categories ####

Discussed briefly whether it would be useful to filter the worklist by category as well as by project. Need more discussion with MR on this.

#### Colour Scheme ####

Agreed colour scheme of Indigo for input formats (ie. data **to** analyser, and Olive for output formats (ie. data **from** analyser)

#### Delimiters ####

We agreed to use the ASTM terminology for special characters:

* 'Field' (e.g `|`)
* 'Repeat' (e.g. `\`, used like a comma in a list of values - e.g. val1\val2\val3\val4)
* 'Component' (i.e. sub-delimiter, e.g. `^`)
* 'Escape' (e.g. `&` - NOT `\` as we're used to in C++ etc)

*(examples from the MEQNET ASTM reference)*

and that these should be configured by the user, rather than picked up from the data itself - the order of these characters in the header of a Menarini CSV file seemed to be different to the ASTM spec, so perhaps we cannot be sure that headers will be well-formed in this way.

#### Value examples ####

#### Further thoughts

We agreed that having the program find an example of the field we are currently configuring from the currently loaded example file was useful.

#### Index from 1, not 0 ####

We agreed that it would be more intuitive to specify  columns, rows etc. as being indexed from 1, rather than 0 - particularly as the ASTM refers to columns in this way.

#### Fixed width vs Delimited explanation ####

We agreed that some kind of explanatory text is needed to explain the difference between setting up a field in a delimited format (e.g. 'the 5th field') versus a fixed-width format (e.g. 'from the 5th column').

#### Tree structure ####

We agreed that the data structures we will be defining may be tree-like, i.e. contain nested structures, and we need a method of describing this in the user interface as well as being able to parse it. The Patient ID should occur only once per saved LIMSresult and can be thought of as the 'master' field, other attributes may repeat for the same Patient ID, ie. there may be several results for the same patient, and of those results, there may be several reagents and/or lots per result. So we need a way of describing this in the interface, perhaps a TreeView, and obviously of parsing this structure when the program is run on real data.

#### Spelling of reagent ####

We agreed that reagent is spelt R-E-A-G-E-N-T, like re-agent, and not reageant or something that looks like sergeant or pageant or something.

#### Further thoughts (CB)

* Each field that is set up to be parsed from the file called be referred to as an 'Extraction'
* Look at ISYS code
* look at doxygen for LIMS classes

#### Meeting 2012-10-03 CB, NG

Reminded that the 32-bit ODBC driver is in `C:\Windows\SysWOW64\odbcad32.exe`. 

`C:\Windows\System32\ocdbad32.exe` gives 'setup routines mismatch' etc.

`"row count: -1"` error is thrown by LQuery when it encounters long varchars.

After login, the LIMS classes change the database to `t_ldbc`, `test_ldbc` or `ldbc`, depending on live/test/mirror. Which explains why my changes to `t_ldbc_min` weren't having any effect.

`centralDb`:`dsn_lab_ingres_min` actually points to `t_ldbc`, not `t_ldbc_min`

Examples of saving LIMSResults etc are in AAAComms HA8180 code:

    AAACResults::saveRecord
    AAACResults::checkAnalysis (saves Buddy id)
    AAACResults::saveTestResult
	
Nick and I will go through this together when the time comes.

To accumulate data row by row, and then save it when all the information is complete, may need to push back results, and possibly reagents within results, as we get them, then pop them off and save before the next patient is encountered.

It might be useful at this stage, for debugging, for visual feedback, doing dummy runs, and for Martin to be able to print out, to print the collected LIMSResults to a CSV/stringgrid. Should be trivial to implement.

We agreed that the structure of the data/datums I had envisaged was pretty much right:

	//class patient { // A container class to cope with multiple analyses,
	//  ResultSet[]   // that may not be needed - leave for now
	//  notes;
	//}

	class resultSet { // ~ LIMSAnalysis
		patientID;
		Result result[];
		barcode
		startDate;
		sampleType;
		String notes; // could record all notes/errors for the analysis
	}

	class Result { // LIMSResult
		Reagent   reagents[];
		ErrorFlag errors[]; // can be put into notes for time being
		int       testID;
		String    resultString;
		String    unit;
		TDateTime dateAnalysed;
		CuvetteType cuvetteType;
		String notes;  // perhaps not necessary
	}

	class Reagent { // LDbResAttribute
		buddyResAttribute/String     reagentID;
		buddyResAttribute/String    lot;
	}

LIMSResult is a pretty flat structure, repeated multiple times for multiple results (and reagents?).	
	

---

#### Pseudo-classes (and actual LIMS classes in comments)

*(this is silly - just list the real classes)*

	class LIMSResult : public LBDbResult, public LBDbSource
	    vector< LDbResAttribute > attributes;
	    addAttribute( const LDbResAttribute)

	class resultSet { // ~ LIMSAnalysis : public LBDbSource
	    Result result[];
			class Result { // LIMSResult? LDbResult
				Reagent reagents[];
					class Reagent { // LDbResAttribute
						buddyResAttribute/String 	reagentID; // struct ReagentID : public LDbResAttribute
						buddyResAttribute/String	lot;       // struct ReagentLot : public LDbResAttribute
					}
				ErrorFlag errors[];
				int 	testID;
				String	resultString;
				String	unit;
				TDateTime dateAnalysed;
				CuvetteType cuvetteType;  // struct Cuvette : public LDbResAttribute
				String notes;
			}
	    String notes;
	}

### Fields in example data

*e.g. Menarini*:

	PID patient
	ORC ?
	OBR - test
	OBX - result

	LIMSResult
	  PID
	  test id
	  result
	  date analysed
	  reagentID
	  reagentLot
	  Cuvette
	  buddyResAttributes
	  notes
	LDbResAttribute

db design
=========

New tables
----------

	c_xfer_filetype
	  name
	  type: input/output
	  dates
	  status
	  delimiter
	  quotes etc
	c_xfer_config
	   filetype
	   machine/cluster
	   status
	
	c_xfer_field
	  file_cid
	  name
	  type: int/string/test result/test code(c_test_machine_cid)/date(range)
	  (date format string?ddmmyy)

### Existing tables

	c_test--assay
	c_machine--analyser
	c_object_name-- [object-type=5] -- cluster
	c_test_machine
	  test_cid = assay
	  machine_cid == analyser OR cluster (group of analysers)
	  machine_test_name == assay code
	  protocol == config option
	  test_order == position in result stream
	  sample_type 

---

### Done

make controls for columns 1-indexed not 0-indexed
date format change crashes  
restrict manage button to privileged users (c_permissions?)  
(page_number is the screen/page that you want to restrict access to - developer-defined)  
LCDbOperator::canRun( int project, int buddy, short page )  
filter worklist by project  

---

### useful snippets

	select * from c_test_machine where machine_cid = -9001
	select * from c_buddy_worklist where machine_cid = -9001
	pw: hunter
	insert into c_machine (machine_cid, external_name, external_full, valid_from, valid_to, status, location) values (-2000000, 'KVOGEL-PROBOOK', 'Chris'' 6550b', date('now'), date('now') + date('1 year'), 0, 'core')
	L151:
	   c_operator
	     chrisb -31438
	   t_ldb15: -82897
	LabsLeasing/leaseIDs.h: `#define   LEASEE_GENCOMMS    110`
	c_permission_cid -1074961
	lims: bl0gger, hunter
	
	insert into c_test_machine (test_cid, machine_cid, machine_test_name) values (-852161, -1108607, 'any name')
	select * from c_test_machine where machine_cid = -1108607


---

### Archive

"row count: -1" error on select from c_gcomms_filetype
   use sprintf rather than param? no - but it worked before!!
    it's the involvement of LQuery that seems to uncover it
   same sql string that throws error in program (ie through BDE) works fine in VDBA
   set varchar(256) -> varchar(250)
    actually was using t_ldbc, not t_ldbc_min, of which c_gcomms_filetype contained a long varchar
   don't have nullable columns

centralDb:dsn_lab_ingres_min actually points to t_ldbc, not t_ldbc_min

start with login form

mapping database

login/audit

rearrange controls on editformat
compare notes.txt vs notes2.txt vs notes3.txt
git push, fields being loaded for all formats instead of selected
parsing

copydb of current schema  
add escapechar  
db tables could be made emptied (eg `archive_c_buddy_worklist`)  
but they're in binary - dump as text? 

    copydb -c
    createdb t_ldbc_min
    /cygdrive/c/Program\ Files\ \(x86\)/Ingres/IngresII/ingres/bin/
added escape_char field
remove "lust". -

    s/group wizard_group/public/
    s/\/tmp\/t_ldbc\/copydb\/// -

	terminal in kate ($SHELL=/bin/tcsh): which createdb: /dbsystem/II/ingres/bin/createdb
	mintty ($SHELL=/bin/tcsh): which createdb: /usr/bin/createdb [postgresql]
	
	lust@brat:TwoBaby!
	t_ldbc.tgz
	nrat:cp/lims-gcomms/create*
	-> /user/lust/proj/lims-gcomms
	-> git
	Patient ID
	Test ID
	Date
	Reagent
	Lot
	
	hplc
	 barcode
	 test id
	 date analysed
	 result

	c_cgcomms_filetype
	 add escapechar
	copydb
	name
	descrip
	(type)
	delimiter
	subdelim
	escapechar-
	dateformat-
	examplefile
	resultset 
	  ((are you sure?) if empty)
	
	OutputDebugString
	c_gcomms_field
	 id
	 id_filetype
	 name
	 tag
	 tagcol
	 valcol
	 subcol
	 type
	 // pos field for ordering?
	 status...
	what happens when you delete a format that has been assigned?
	search for XXX and TODO, FIXME

# StoreMan Retrieval Assistant

###  8. Sample retrieval screen

#### 8.1	Previous lists

Allow previous retrieval lists to be viewed, first list names (REQ 8.1.1) then list of boxes samples retrieved into with current storage location (REQ 8.1.2).

#### 8.2	Create list

Allow import of list of sample ID’s, cryovial ID’s or box names from either a text file (REQ 8.2.1) or from specimen table (REQ 8.2.2) and allow selection of primary aliquot choice and secondary aliquot choice (REQ 8.2.3). Generate list of sample ID’s with location of primary aliquot choice and secondary aliquot choice in separate columns (REQ 8.2.4), or where appropriate a list of box numbers (REQ 8.2.5) and allow list to be ordered alphanumerically by any column (REQ 8.2.6). Ask user if samples are to be retrieved to new boxes (REQ 8.2.7), if user specifies yes then allow a destination location to be mapped to each sample barcode (REQ 8.2.8), this would be a new box for the sample to go into once retrieved, if user responds no then use existing box numbers and locations and assume the samples will be kept in the original location (REQ 8.2.9). Where no destination is specified new boxes should be generated unless whole boxes are being retrieved (REQ 8.2.10) and they should be filled sequentially (REQ 8.2.11). After creating a list the following options should be given, Save or Discard (REQ 8.2.12). If Save is selected the user should be asked to name the list prior to it being saved (REQ 8.2.13), if Discard is selected an “Are you sure?” message should be displayed with the “Yes” response returning to the sample retrieval screen (REQ 8.2.14) and the “No” response returning to Save/Discard option (REQ 8.2.15). Created lists should be displayed on the status indicator on the main screen (REQ 8.2.16).

#### 8.3	Retrieval assistant

Allow selection of retrieval list that are either in progress or new (REQ 8.3.1). For new lists give the option to divide the list (REQ 8.3.2),
if “Yes” is selected allow user to define division points in list (REQ 8.3.3) and present each section for retrieval in turn with the option to re-order each sub-section by sample ID, primary aliquot location, secondary location or destination (REQ 8.3.4). If no is selected allow whole list to be ordered by sample ID, primary aliquot location, secondary location or destination (REQ 8.3.5). Following this division and re-ordering the user should be asked to save the changes prior to commencing retrieval with the option going back to re-order if necessary (REQ 8.3.6). For in progress lists these steps should be skipped (REQ 8.3.7). Work through list or sub-section by giving the storage location and sample ID of each sample on the list in the order saved above (REQ 8.3.8)
as each sample is retrieved it’s barcode should be scanned, if the scanned barcode matches that on the list the destination location should be displayed and the next ID/location should be displayed (REQ 8.3.9) if the ID’s do not match a warning should be displayed and re-entry of the barcode required (REQ 8.3.10). 
When working through the list the previous five successfully entered ID’s should always be visible (REQ 8.3.11). The option to exit the process saving progress should be offered, with an “are you sure?” message in case of accidental selection (REQ 8.3.12).

## More detail

### Create retrieval work list, requirements 8.3.1 – 8.3.7

When the retrieval assistant is given a new job, it allows the user to divide the work into sections:

#### Box retrieval

    c_retrieval_job.status = new job (0); job type = box retrieval (2) or disposal (3)

Find where the boxes are supposed to be:

    Select … from box_name n, box_store bs, c_rack_number r, c_tank_map m
    where n.box_cid=bs.box_cid and bs.rack_cid=r.rack_cid and r.tank_cid=m.tank_cid
    and bs.retrieval_cid = jobID;

List the name, current structure and expected location of each box.  The location should include site+location+name+layout, as it does in StoreMan’s storage browser

Allow the user to divide up the list (this may not be necessary; see below for more details).  After division, show each section in turn and allow user to sort by location, otherwise show the whole list and allow them to sort that.

Ask user to save changes with the option of going back to re-order if necessary.

Insert an entry into `c_box_retrieval` for each box, recording the section it is in and its position within that section.  

	update c_retrieval_job set status=in progress (1)

#### Sample retrieval

    c_retrieval_job.status = new job (0); job type = sample retrieval (4) or disposal (5)
    
Find the cryovials to be retrieved:

    Select … from cryovial c, cryovial_store cs, box_name n, box_store bs, c_rack_number r, c_tank_map m
    where c.cryovial_id=cs.cryovial_id and n.box_cid=cs.box_cid and n.box_cid=bs.box_cid and bs.status=6 and bs.rack_cid=r.rack_cid and r.tank_cid=m.tank_cid and cs.retrieval_cid=jobID;

Roughly half of these will be for the primary aliquot (i.e. `cryovial.aliquot_type_cid = c_retrieval_job.primary_aliquot`).  The other half will be for the secondary aliquot.  Primary and secondary should have the same barcode.

Entries for the primary aliquot could have a destination defined.  You could find these using a left join (but not on a distributed database, hence no location):

    select … from cryovial_store s1 left join cryovial c on c.cryovial_id=s1.cryovial_id
    left join  box_name n1 on  n1.box_cid=s1.box_cid
    left join cryovial_store s2 on s1.cryovial_id=s2.cryovial_id and s2.status=0
    left join box_name n2 on n2.box_cid = s2.box_cid
    where s1.retrieval_cid = jobID

Create suitable `box_name` and `cryovial_store` entries if no destination boxes have been defined (but only for one cryovial in each pair)

Piece this information together to create a list giving the cryovial barcode, destination box, position, current box, position, structure and location of the primary and secondary

Display the size of the job and ask user if they want to divide up the list.  If they do:

1.	Ask them the maximum section size (default = 500 cryovials)
2.	Calculate slot/box (where c_box_size.box_size_cid = box_content.box_size_cid)
3.	Ask them to select the size of first section from a list – it must be a multiple of the box size (from 2) and no more than the maximum (from 1)
4.	Allocate the appropriate number of destination boxes to the first section
5.	Repeat steps (2) and (3) until every entry has been allocated to a section

After division, show each section in turn and allow the user to sort by location, otherwise show the whole list and allow them to sort that.

Ask user to save changes with the option of going back to re-order if necessary.

Insert an entry into `c_box_retrieval` for each destination box, recording the section it is in and its position within that section (you might use position = 0 to indicate the section should be sorted by current sample location).

    update c_retrieval_job set status=in progress (1)

## Other jobs

    C_retrieval_job.status = in progress (1) and job.type in (2,3,4,5): act on list [tbd]
    C_retrieval_job.status not in (0,1) or job.type not in (2,3,4,5): complain

 * save progress
 * signoff
 * allow going back over skipped
 * load in collected/not found/ignored samples as well to return to in progress chunk
 * Insert a record into c_box_retrieval for each box in turn and update c_retrieval_job: set status=in progress (1)
 * Note: a sample retrieval can include boxes that do not have their current locations recorded in the database.
 * Tick each cryovial off in the retrieval plan as you go along.
 * Ask the relevant question(s) from the URS when they’re ready to finish and 
 * update cryovial_store (old and new, primary and secondary) when they enter their password to confirm

## misc

 * accept(): if primary aliquot !collected   # expected, ignored (previously skipped), not found (now found?)
 * http://www.thefreedictionary.com/aliquot: a sample can be divided into aliquot parts

## 8.3 Retrieval assistant

The option to exit the process saving progress should be offered, 
    with an “are you sure?” message in case of accidental selection (REQ 8.3.12).

## done

 * Allow selection of retrieval list that are either in progress or new (REQ 8.3.1). 
 * For new lists give the option to divide the list (REQ 8.3.2), 
 * if “Yes” is selected allow user to define division points in list (REQ 8.3.3) and present each section for retrieval in turn with the option to re-order each sub-section by sample ID, primary aliquot location, secondary location or destination (REQ 8.3.4). 
 * If no is selected allow whole list to be ordered by sample ID, primary aliquot location, secondary location or destination (REQ 8.3.5). 
 * Following this division and re-ordering the user should be asked to save the changes prior to commencing retrieval with the option going back to re-order if necessary (REQ 8.3.6). 
 * For in progress lists these steps should be skipped (REQ 8.3.7).
 * Work through list or sub-section by giving the storage location and sample ID of each sample on the list in the order saved above (REQ 8.3.8) 
 * as each sample is retrieved it’s barcode should be scanned, if the scanned barcode matches that on the list the destination location should be displayed and the next ID/location should be displayed (REQ 8.3.9) 
 * if the IDs do not match a warning should be displayed and re-entry of the barcode required (REQ 8.3.10). 
 * When working through the list the previous five successfully entered ID’s should always be visible (REQ 8.3.11). 

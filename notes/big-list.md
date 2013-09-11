


find destination boxes 1st

stuff in updated spec (2013-09-11) about finding destination boxes?
  diff btwn specs in notes/spec-comparison

find number of boxes
  current box
  destination box?
what do we mean by 'size' of a chunk - number of boxes

Nick's sorter in SampleEntry.cpp/h

chunks
    section - Cannot Be Changed 
    start
        1st chunk: CBC
        Others:
    end:
        1st chunk: 
    size:
        calculate end (max if not valid)

    specify in samples or boxes?
        
highlight current sample/box
double-click sample/box
    current chunks upper boundary is here (check valid)
    next chunk (if present) lower boundary is here
  
## Retrieval Assistant

### Background

#### Create List

 * Any aliquot type used by a project can be selected as the primary; the secondary cannot match the primary
 * Creating a new sample retrieval list will always create new box name records and (provisionally) allocate cryovial positions
 * A source file may be pre-sorted and may include box numbers and positions as well as the barcode
 * The retrieval list can be re-sorted on screen but this will only affect the destination positions if the source was not pre-sorted
 * Columns should be displayed in Russian doll order, left to right: site, location, vessel, shelf, structure, slot, box, position
 * Rejected tasks can be modified (re-using existing box name records) or deleted
 * Finding where all the cryovials are can take a long time; users must wait for the information to be available 

### Retrieval Assistant

 * As retrieval lists will always specify destination boxes, chunk size can be based on the number of cryovials allocated to each box
 * Columns should be displayed in Russian doll order, left to right: site, location, vessel, shelf, structure, slot, box, position
 * If required, the secondary aliquots appear at the end of the retrieval plan for each chunk – they may never be needed
 * a panel displaying sort order for both Create List and Retrieval Assistant
 * sort by location,
 * location should include site+position+name+layout, as it does in StoreMan’s storage browser.-
 * chunking -  Allow the user to divide up the list
 * save changes with the option of going back to re-order if necessary.
 * Cryogenics staff can reject a retrieval list if it does not have a retrieval plan
    * Box Retrieval
        * Select * from c_box_retrieval b order by b.section, b.rj_box_cid
        
    * Sample (cryovial) Retrieval
        * Select * from c_box_retrieval b, l_cryovial_retrieval c where b.rj_box_cid = c.rj_box_cid order by b.section, c.position  


---to sort---
ukes reminder
xe2
sits:vision-
---

retrasst objectives for next meeting >> workblog >> stf eml
get rid of maxrows maximum-
retrasst emails -> folder
# Referred Boxes testing #

xrat ingres installation pw: garafia - must be supplied if vnode login name is '*' (current user)

### Test queries ##

#### storeTRS ###

    select * from box_store where box_cid = x order by time_stamp desc
    { EXPECTED = 0, MOVE_EXPECTED = 1, REMOVED = 3, SLOT_ALLOCATED = 5, SLOT_CONFIRMED = 6, REFERRED = 7, DELETED = 99 };
    
    select box_cid, box_capacity, external_name, status, slot_cid, time_stamp from box_name where box_cid = x
    { EMPTY = 0, IN_USE = 1, CONFIRMED = 2, ANALYSED = 3, IN_TANK = 4, DESTROYED = 8, DELETED = 99 };

#### addEvent ###

    select * from box_event_history where box_id = x order by event_date desc

#### updateLBA ###

    select box_arrival_id, time_stamp, box_name, status, first_position, first_barcode, last_position, last_barcode, tank_cid, rack_number, slot_position from l_box_arrival where box_arrival_id = x

### Test results ##

OK  2205148	HPS2-THRIVE DNA(UK) 2205148 	
        Already stored		
        No change

    before
    box_store: record_id: 1144, rack_cid: -971096, slot_position: 12, status: 6
    box_name: box_capacity: -1, status: 4, slot_cid: -1053904
    l_b_a: status: 7, 1stpos: 1, lstpos: 14, tank_cid: -943331, rack_number: C1, slot_position: 12

    After
    box_store: record_id: , rack_cid: , slot_position: , status: 
    box_name: box_capacity: , status: , slot_cid: 
    l_b_a: status: 4, 1stpos: , lstpos: , tank_cid: , rack_number: , slot_position:     
---
OK  2721846	HPS2-THRIVE DNA_LT 2721846 	
        Wrong last position		
        Create box_store record for 2721846; 
        update box_name.status and l_box_arrival_status
        [fixed box store save]

    Before
    box_store: (na) record_id: , rack_cid: , slot_position: , status: 
    box_name: box_capacity: -1, status: 2, slot_cid: -1038852
    l_b_a: status: 7, 1stpos: 1, lstpos: 72, tank_cid: -943330, rack_number: C1, slot_position: 3

    After
    box_store: record_id: 4109969, rack_cid: -967742, slot_position: 3, status: 6
    box_name: box_capacity: -1, status: 4, slot_cid: -1038852
    l_b_a: status: 4, 1stpos: 1, lstpos: 24, tank_cid: -943330, rack_number: C1, slot_position: 3    
---
OK  2706572	HPS2-THRIVE DNA_NLT 2706572 	
        Wrong first position	
        [Position is in use?]
        Create box_store record for 2706572; 
        update box_name.status and l_box_arrival_status	create		update		update				
    
    Before
    box_store: (na) record_id: , rack_cid: , slot_position: , status: 
    box_name: box_capacity: 0, status: 2, slot_cid: -1025455
    l_b_a: status: 7, 1stpos: 0, lstpos: 72, tank_cid: -943330, rack_number: C1, slot_position: 2

    After
    box_store: record_id: 4109981, rack_cid: -967742, slot_position: 2, status: 6
    box_name: box_capacity: 0, status: 4, slot_cid: -1025455
    l_b_a: status: 4, 1stpos: 1, lstpos: 72, tank_cid: -943330, rack_number: C1, slot_position: 2    
---
OK  2750504	HPS2-THRIVE DNA_NLT 2750504 	
        No tank_cid; should be F38		
        box storage history - "no results" - see note 3	
        Create corrected box_store record for 2750504; 
        update box_name.status and l_box_arrival_status	
    
    Before
    box_store: (na) record_id: , rack_cid: , slot_position: , status: 
    box_name: box_capacity: 0, status: 2, slot_cid: -1038854
    l_b_a: status: 7, 1stpos: 1, lstpos: 59, tank_cid: 0, rack_number: C1, slot_position: 4

    After
    box_store: record_id: , rack_cid: , slot_position: , status: 
    box_name: box_capacity: , status: , slot_cid: 
    l_b_a: status: , 1stpos: 1, lstpos: 59, tank_cid: -943330, rack_number: C1, slot_position: 4    

tank_cid not changed in l_b_a: fixed
---
OK  2793174	HPS2-THRIVE EDTA_1(UK) 2793174 	
        Wrong position; should be 9		see note 1		
        Create corrected box_store record for 2793174; 
        update box_name.status and l_box_arrival_status	
        note 1: 
            no initial indication that pos might be wrong - no way round that - relies on lab to have made a note that it needs changing.
            cheap check: check for c_rack_number.capacity = 0 (no space - must be wrong).     
            CheckTRSWorkerThread checks if (1 <= box.slot_position <= layout->getRackCapacity()) - OK
            if cheap check OK, searches projects and gives 'position in use' if in use and prevents OK - correct.
    
    Before
    box_store: (na) record_id: , rack_cid: , slot_position: , status: 
    box_name: box_capacity: 0, status: 2, slot_cid: -1035726
    l_b_a: status: 7, 1stpos: 1, lstpos: 65, tank_cid: -319623, rack_number: B7, slot_position: 8 

    After
    box_store: record_id: 4109993, rack_cid: -932114, slot_position: 9, status: 6
    box_name: box_capacity: 0, status: 4, slot_cid: -1035726 
    l_b_a: status: 4, 1stpos: 1, lstpos: 65, tank_cid: -319623, rack_number: B7, slot_position: 9    
---
OK  2725250	HPS2-THRIVE EDTA_1_LT 2725250 	
        Wrong box_name		
        in list but not highlighted
        selecting changes box name (missing 'n')
        Create box_store record for 2725250; 
        update box_name.status and l_box_arrival_status

    Before
    box_store: (na) record_id: , rack_cid: , slot_position: , status: 
    box_name: HPS2-THRIVE EDTA_1_nLT 2725250, box_capacity: 0, status: 2, slot_cid: -1035721
    l_b_a: box_name: HPS2-THRIVE EDTA_1_LT 2725250, status: 7, 1stpos: 1, lstpos: 9, tank_cid: -319623, rack_number: B7, slot_position: 11

    After
    box_store: record_id: 4109997, rack_cid: -932114, slot_position: 11, status: 6
    box_name: HPS2-THRIVE EDTA_1_nLT 2725250, box_capacity: 0, status: 4, slot_cid: -1035721
    l_b_a: box_name: HPS2-THRIVE EDTA_1_nLT 2725250, status: 4, 1stpos: 1, lstpos: 90, tank_cid: -319623, rack_number: B7, slot_position: 11    
---    
?   2758457	HPS2-THRIVE EDTA_1_LT 2758457 	
        Wrong tank_cid; should be W35		see note 1	
        Create corrected box_store record for 2758457; 
        update box_name.status and l_box_arrival_status	correct?
    
    Before
    box_store: (na) record_id: , rack_cid: , slot_position: , status: 
    box_name: HPS2-THRIVE EDTA_1_LT 2758457, box_capacity: 0, status: 2, slot_cid: -1035723
    l_b_a: status: 7, 1stpos: 1, lstpos: 80, tank_cid: -943330, rack_number: B7, slot_position: 10

    After
    box_store: record_id: 4110001, rack_cid: -932114, slot_position: 10, status: 6
    box_name: HPS2-THRIVE EDTA_1_LT 2758457, box_capacity: 0, status: 4, slot_cid: -1035723
    l_b_a: status: 4, 1stpos: 1, lstpos: 80, tank_cid: -319623, rack_number: B7, slot_position: 10
box_name.slot_cid has not changed - is this correct? obsolete - use box_store
---        
?   1507	HPS2-THRIVE EDTA_2(UK) 1507	
        New location for existing box		
        t/r/s all red	new position is in use - is this the intended test?
        Update box_store.status; 
        create new box_store record for 1507; 
        update l_box_arrival_status	update, 
            gives 'position in use' - for new location? if select old location, ok, and warns about changes from those in lba
            
    Before
    box_store: record_id: , rack_cid: , slot_position: , status: 
    box_name: box_capacity: , status: , slot_cid: 
    l_b_a: status: , 1stpos: , lstpos: , tank_cid: , rack_number: , slot_position: 

    After
    box_store: record_id: , rack_cid: , slot_position: , status: 
    box_name: box_capacity: , status: , slot_cid: 
    l_b_a: status: , 1stpos: , lstpos: , tank_cid: , rack_number: , slot_position:     
program does what I think it should but test does not match description
---
OK  2758458	HPS2-THRIVE EDTA_2_LT 2758458 	
        Slot already occupied		
        storage hist shows 'no results'	
        pos in use, discard
        Error message	

    Before
    box_store: record_id: , rack_cid: , slot_position: , status: 
    box_name: box_capacity: , status: 2, slot_cid: 
    l_b_a: status: 7, 1stpos: , lstpos: , tank_cid: , rack_number: , slot_position: 

    After
    box_store: record_id: , rack_cid: , slot_position: , status: 
    box_name: box_capacity: , status: , slot_cid: 
    l_b_a: status: 99, 1stpos: , lstpos: , tank_cid: , rack_number: , slot_position:     
bad sql string for update l_b_a DELETED: fixed
---
OK  2725251	HPS2-THRIVE EDTA_2_NLT 2725251 	
        Wrong first barcode		
        Create box_store record for 2725251; 
        update box_name.status and l_box_arrival_status	
    
    Before
    box_store: (na) record_id: , rack_cid: , slot_position: , status: 
    box_name: HPS2-THRIVE EDTA_2_nLT 2725251, box_capacity: 0, status: 2, slot_cid: -1038853
    l_b_a: HPS2-THRIVE EDTA_2_NLT 2725251, 
        status: 7, 1stpos: 1 [112143308], lstpos: 90 [112054031], tank_cid: -319653, rack_number: Top36, slot_position: 4
    (box names different (lowercase n) from box_name to l_b_a)

    After
    box_store: record_id: 4110009, rack_cid: -999219, slot_position: 4, status: 6
    box_name: box_capacity: 0, status: 4, slot_cid: -1038853
    l_b_a: HPS2-THRIVE EDTA_2_nLT 2725251,
        status: 4, 1stpos: 1 [112119287], lstpos: 90 [112054031], tank_cid: -319653, rack_number: Top36, slot_position: 4   
    (changed 1st barcode, changed name as well)
---    
OK  2193361	HPS2-THRIVE FL/OX(UK) 2193361 	
        None		
        Create box_store record for 2193361; 
        update box_name.status and l_box_arrival_status	
    
    Before
    box_store: (na)record_id: , rack_cid: , slot_position: , status: 
    box_name: box_capacity: -1, status: 2, slot_cid: -1035717
    l_b_a: status: 7, 1stpos: 1, lstpos: 19, tank_cid: -171055, rack_number: I4, slot_position: 7

    After
    box_store: record_id: 4110013, rack_cid: -739462, slot_position: 7, status: 6
    box_name: box_capacity: -1, status: 4, slot_cid: -1035717
    l_b_a: status: 4, 1stpos: 1, lstpos: 19, tank_cid: -171055, rack_number: I4, slot_position: 7   
do box_name.slot_cid and box_store.rack_cid/slot_postion match up to l_b_a tank_cid/rack number?
---    
OK/ 7464829	HPS2-THRIVE FL/OX_LT 2464829 	
        Wrong box_arrival_id		
        Create box_store record for 2464829; 
        update box_name.status and l_box_arrival_status
            does it change data for 2464829 instead of 7464829? should it?
        after improvement to delete wrong id (when selectedMatch->box_arrival_id != referredBox->box_arrival_id) and insert correct one:
            l_b_a box 7464829 is 99'd, box 2464829 looks like a copy of the right things
            warning that 'tank has changed from NULL, rack name from '' and slot from 0?
            then got 'duplicate key on insert' when I ran it again (not having signed off)

#### reset test data
update l_box_arrival set status = 7 where box_arrival_id = 7464829;
delete from l_box_arrival where box_arrival_id = 2464829
            
    Before - no project data for 7464829 (should be 2464829)
    box_store: (na)record_id: , rack_cid: , slot_position: , status: 
    box_name: (na) box_capacity: , status: , slot_cid: 
    Project data for 2464829:
    box_store: (na)record_id: , rack_cid: , slot_position: , status: 
    box_name: HPS2-THRIVE fl/ox_LT 2464829, box_capacity: 0, status: 2, slot_cid: -1035720

    l_b_a: status: 7, 1stpos: 1, lstpos: , tank_cid: , rack_number: , slot_position: 
doesn't copy correct box id [2464829] from canonical box_name into l_box_arrival/referred boxes - hence 'box_name record not found'-now does
    After
    box_store: record_id: , rack_cid: , slot_position: , status: 
    box_name: box_capacity: , status: , slot_cid: 
    l_b_a: status: , 1stpos: , lstpos: , tank_cid: , rack_number: , slot_position:     
---    
OK  2571741	HPS2-THRIVE FL/OX_NLT 2577741 	
        Wrong box_name		
        in list but not highlighted - selecting changes  box name (missing 'n')	
        Create box_store record for 2571741; 
        update box_name.status and l_box_arrival_status	

    Before
    box_store: (na) record_id: , rack_cid: , slot_position: , status: 
    box_name: HPS2-THRIVE fl/ox_nLT 2571741, box_capacity: 0, status: 2, slot_cid: -1035718
    l_b_a: name: HPS2-THRIVE FL/OX_NLT 2577741, status: 7, 1stpos: 1, lstpos: 38, tank_cid: -319625, rack_number: C2, slot_position: 4

    After
    box_store: record_id: 4110021, rack_cid: -732863, slot_position: 4, status: 6
    box_name: HPS2-THRIVE fl/ox_nLT 2571741, box_capacity: 0, status: 4, slot_cid: -1035718
    l_b_a: name: HPS2-THRIVE fl/ox_nLT 2571741, status: 4, 1stpos: 1, lstpos: 38, tank_cid: -319625, rack_number: C2, slot_position: 4    
---    
OK  2758459	HPS2-THRIVE LIHEP_LT 2758459 	
        Wrong rack number; should be B6		see note 1	
        pos in use.- 
        Change B5 to B6 - 'are you sure?'. If yes, ok.-
        Create corrected box_store record for 2758459; 
        update box_name.status and l_box_arrival_status	

    Before
    box_store: (na) record_id: , rack_cid: , slot_position: , status: 
    box_name: HPS2-THRIVE LiHep_LT 2758459, box_capacity: 0, status: 2, slot_cid: -1035724
    l_b_a: name: HPS2-THRIVE LIHEP_LT 2758459, status: 7, 1stpos: 1, lstpos: 80, tank_cid: -319625, rack_number: B5, slot_position: 4

    After
    box_store: record_id: 4110025, rack_cid: -942884, slot_position: 4, status: 6
    box_name: HPS2-THRIVE LiHep_LT 2758459, box_capacity: 0, status: 4, slot_cid: -1035724
    l_b_a: name: HPS2-THRIVE LiHep_LT 2758459 (corrected), 
        status: 4, 1stpos: 1, lstpos: 80, tank_cid: -319625, rack_number: B6 (corrected), slot_position: 4
---    
OK/ 2758460	HPS2-THRIVE URINE_LT 2758460 	
        No position; should be 7		see note 1	
        ie. No slot (slot = 0). Should check slot > 1?	
        Create corrected box_store record for 2758460; 
        update box_name.status and l_box_arrival_status	
            doesn't check slot position! is slot == 0 valid?
            (find empty racks, slots?)
    
    Before
    box_store: record_id: , rack_cid: , slot_position: , status: 
    box_name: box_capacity: , status: 2, slot_cid: -1035725
    l_b_a: status: 7, 1stpos: , lstpos: , tank_cid: -184833, rack_number: I7, slot_position: 0 

    After
    box_store: record_id: , rack_cid: , slot_position: , status: 
    box_name: box_capacity: , status: 4, slot_cid: -1035725
    l_b_a: status: 4, 1stpos: , lstpos: , tank_cid: -184833, rack_number: I7, slot_position: 7 (corrected)    
should slot be non-zero? check for upper slot number?
difference between slot_cid and slot_position? how can one change but not the other?
---    
OK  2731805	HPS2-THRIVE URINE_NLT 2731805 	
        Wrong last barcode		
        Create box_store record for 2731805; 
        update box_name.status and l_box_arrival_status	

    Before
    box_store:(na) record_id: , rack_cid: , slot_position: , status: 
    box_name: box_capacity: 0, status: 2, slot_cid: -1035722 
    l_b_a: status: , 1stpos: 1 [112109745], lstpos: 53 [112109745], tank_cid: -184833, rack_number: I7, slot_position: 8

    After
    box_store: record_id: , rack_cid: -942880, slot_position: 8, status: 6
    box_name: box_capacity: 0, status: 4, slot_cid: -1035722 
    l_b_a: status: 4, 1stpos: 1 [112109745], lstpos: 53 [112054031] (corrected), tank_cid: -184833, rack_number: , slot_position: 8     
---
    KS-A-120056
        ? a real live one

    Before
    box_store: record_id: , rack_cid: , slot_position: , status: 
    box_name: box_capacity: , status: , slot_cid: 
    l_b_a: status: , 1stpos: , lstpos: , tank_cid: , rack_number: , slot_position: 

    After
    box_store: record_id: , rack_cid: , slot_position: , status: 
    box_name: box_capacity: , status: , slot_cid: 
    l_b_a: status: , 1stpos: , lstpos: , tank_cid: , rack_number: , slot_position: 
    
## Template ##

    Before
    box_store: record_id: , rack_cid: , slot_position: , status: 
    box_name: box_capacity: , status: , slot_cid: 
    l_b_a: status: , 1stpos: , lstpos: , tank_cid: , rack_number: , slot_position: 

    After
    box_store: record_id: , rack_cid: , slot_position: , status: 
    box_name: box_capacity: , status: , slot_cid: 
    l_b_a: status: , 1stpos: , lstpos: , tank_cid: , rack_number: , slot_position: 

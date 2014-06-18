`c_box_name`

        project_cid                      integer                 4   no     yes           1980.1
        box_cid                          integer                 4   no      no     1     unique
        barcode                          varchar                24   no      no
        box_type_cid                     integer                 4   no      no            232.2
        box_capacity                     integer                 2   no   value
        external_name                    varchar                36   no      no
        process_cid                      integer                 4   no     yes
        status                           integer                 2   no     yes
        time_stamp                       ingresdate                  no   value

`c_slot_allocation`

        slot_cid                         integer                 4   no      no     1     unique
        rack_cid                         integer                 4   no      no             11.6
        slot_position                    integer                 2   no      no
        status                           integer                 2   no      no           6394.4
        project_cid                      integer                 4   no     yes
        box_cid                          integer                 4   no     yes
        time_stamp                       ingresdate                  no   value
        retrieval_cid                    integer                 4   no     yes
        */

    string bname, int pjid, string firstbar, int firstpos, string lastbar, int lastpos, // BoxRecord
         
    int recn, 
    int ltid, 
    int psid, 
    int baid, 
    int st,
      
    int tankid, 
    string tname, 
    int rackid, 
    string racknum, 
    int rackslot, 
    string slotname,
    int storestatus,
    
    TDateTime tstamp, TDateTime stime, TDateTime rtime) :
        
    record_no(recn), laptop_cid(ltid), process_cid(psid), box_arrival_id (baid), status(st),
    tank_cid(tankid), tank_name(tname), rack_cid(rackid), rack_name(racknum), slot_position(rackslot), slot_name(slotname),
    box_store_status(storestatus),
    time_stamp(tstamp), swipe_time(stime), removed(rtime), changed(false)
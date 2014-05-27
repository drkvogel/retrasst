Recording chunk/section number in SampleRow (sr)

## LCDbBoxRetrieval

    LCDbBoxRetrieval
        int rj_box_cid          // rj_box_cid
        int retrieval_cid       // retrieval_cid
        int box_id              // box_id
        int project_cid         // project_cid
        int section             // section
        int status              // status
        TDateTime time_stamp    // time_stamp

## extra data in LCDbBoxRetrieval

    LCDbBoxRetrieval
        --int rj_box_cid          // already in lcr
        int retrieval_cid       // not yet in SampleRow, but known in program
        --int box_id              // already in sr as old_box_cid?
        int project_cid         // in sr as standalone var, could be replaced
        int section             // needed for chunking
        int status              // needed? 
        TDateTime time_stamp    // prob not needed?

## LCDbBoxRetrieval constructor

    LCDbBoxRetrieval(int rj, int bid, int prj, int sec, int st) 
        rj_box_cid(0)
        retrieval_cid(rj)
        box_id(bid)
        project_cid(prj)
        section(sec)
        status(st)

## LCDbCryovialRetrieval

    LCDbCryovialRetrieval
        int rj_box_cid
        int position
        int aliquot_type_cid
        int old_box_cid
        int old_position
        int new_position
        int process_cid
        int status
        std::string cryovial_barcode
        TDateTime time_stamp



    LCDbCryovialRetrieval
        int rj_box_cid
        int position
        int aliquot_type_cid
        int old_box_cid
        int old_position
        int new_position
        int process_cid
        int status
        std::string cryovial_barcode
        TDateTime time_stamp
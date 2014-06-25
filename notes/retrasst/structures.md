    LPDbCryovial
    LPDbCryovialStore
    LCDbCryovialRetrieval

    SampleRow (
        int proj, 
        LPDbCryovial * cryo_rec, 
            LPDbID                      // "Cryovial_id"
            std::string     barcode     // "Cryovial_Barcode"
            int     boxID               // "box_cid"
            int     sampleID            // "sample_id"
            int     typeID              // "aliquot_type_cid"
            int     storeID             // "record_id"
            int     retrievalID         // "retrieval_cid"
            short   status              // "status"
            short   position            // "cryovial_position"
        LPDbCryovialStore * store_rec,
            LPDbID                      // "Cryovial_id" 
            int     cryovialID          // "Cryovial_id"
            int     boxID               // "box_cid"
            int     retrievalID         // "retrieval_cid"
            short   status              // "status"
            short   position            // "cryovial_position"
            float   volume              // -1
        LCDbCryovialRetrieval * retrieval_rec, 
            int     rj_box_cid          // "rj_box_cid"
            int     position            // "lcr_position"
            int     aliquot_type_cid    // "aliquot_type_cid"
            int     old_box_cid         // "old_box_cid"
            int     old_position        // "old_position"
            int     new_position        // "new_position"
            int     process_cid         // "lcr_procid"
            int     status              // "lcr_status"
            string  cryovial_barcode    // "cryovial_barcode"
            TDateTime   time_stamp      // "time_stamp"
        LPDbBoxName * dest_box,        
            string  name                // "external_name"
            string  barcode             // "barcode"
            short   status              // "status"
            int     box_type_cid        // "box_type_cid"
            int     filledBy            // 0 (process id?)
        string barc, 
        string srcnm, 
        int dstid, 
        string dstnm, 
        int dstps, 
        string site, 
        int vsps, 
        string vsnm, 
        int shlf, 
        int stps, 
        string stnm, 
        int bxps)


    SampleRow(  LPDbCryovial * cryo_rec, 
                LPDbCryovialStore * store_rec, 
                LCDbCryovialRetrieval * retrieval_rec,
                string barc, 
                string srcnm, 
                int dstid, 
                string dstnm, 
                int dstps,
                string site, 
                int vsps, 
                string vsnm, 
                int shlf, 
                int stps, 
                string stnm, 
                int bxps) :
                RetrievalRow(
                    srcnm, 
                    dstid, 
                    dstnm, 
                    site, 
                    vsps, 
                    vsnm, 
                    shlf, 
                    stps, 
                    stnm, 
                    bxps),
                cryo_record(cryo_rec), 
                store_record(store_rec), 
                retrieval_record(retrieval_rec), 
                cryovial_barcode(barc), 
                dest_cryo_pos(dstps), 
                secondary(NULL) {
    }

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
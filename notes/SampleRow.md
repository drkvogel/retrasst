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

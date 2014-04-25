    LPDbCryovial
    LPDbCryovialStore
    LCDbCryovialRetrieval

    SampleRow (
        int proj, 
        LPDbCryovial * cryo_rec, 
            std::string     barcode
            int     boxID
            int     sampleID
            int     typeID
            int     storeID
            int     retrievalID
            short   status
            short   position
        LPDbCryovialStore * store_rec, 
            int     cryovialID
            int     boxID
            int     retrievalID
            short   status
            short   position
            float   volume
        LCDbCryovialRetrieval * retrieval_rec, 
            int     rj_box_cid
            int     position
            int     aliquot_type_cid
            int     old_box_cid
            int     old_position
            int     new_position
            int     process_cid
            int     status
            std::string     cryovial_barcode
            TDateTime   time_stamp
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

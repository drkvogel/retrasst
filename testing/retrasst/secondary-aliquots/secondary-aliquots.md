class SampleRow : public RetrievalRow
    LPDbCryovial            * cryo_record; // auto_ptr for these?
    LPDbCryovialStore       * store_record;
    LCDbCryovialRetrieval   * retrieval_record;

class LCDbBoxRetrieval : public LCDbID
    int rj_box_cid, retrieval_cid, box_id, project_cid, section, status;
    enum Status { NEW, PART_FILLED, COLLECTED, NOT_FOUND, DELETED = 99, NUM_STATUSES };




showCurrentRow();
nextRow();


Android version: 4.0.3
Baseband: Acer_AV041_MT6575_E350_1.000.23
Kernel: 3.0.13
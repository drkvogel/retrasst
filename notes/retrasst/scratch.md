isJobComplete()

chunkCompleted()

SaveProgressThread::Execute()

discardBoxes() 


 * load in collected/not found/ignored samples as well to return to in progress chunk
    * what about secondaries?

accept(): aliquot->retrieval_record->setStatus(LCDbCryovialRetrieval::COLLECTED);
skip(): currentAliquot()->retrieval_record->setStatus(LCDbCryovialRetrieval::IGNORED);
notFound(): retrieval_record->setStatus(LCDbCryovialRetrieval::NOT_FOUND)
nextRow(): sample->retrieval_record->saveRecord(LIMSDatabase::getCentralDb())
    if (sample->secondary): sample->secondary->retrieval_record->saveRecord(LIMSDatabase::getCentralDb())

saveRecord doesn't seem to have done anything - there are 1674 = 2 * 837 aliquots ie the whole job in l_cryovial_retrieval, 
none with status != 0

first row is 112155048
	saveRecord()
		rj_box_cid = 38
		position = 10
		trying to update l_cryovial_retrieval WHERE rj_box_cid = 38 AND position = 10
		 	- ain't no such thing
		 	(rj_box_cid = 38??) no. 
		 	sample->retrieval_record

    RetrievalRow(
    	string srcnm, 
    	int dstid, 
    	string dstnm, 
    	string site, 
    	int vsps, 
    	string vsnm, 
    	int shlf, 
    	int stps, 
    	string stnm, 
    	int bxps) :
        src_box_name(srcnm), 
        dest_box_id(dstid), 
        dest_box_name(dstnm),
        site_name(site), 
        vessel_pos(vsps), 
        vessel_name(vsnm), 
        shelf_number(shlf), 
        structure_pos(stps), 
        structure_name(stnm), 
        box_pos(bxps) 

    LCDbCryovialRetrieval(
    	int rjbid, 
    	int pos, 
    	std::string barcode, 
    	int aliq, 
    	int oldbid, 
    	int oldpos, 
    	int newpos, 
    	int pid, 
    	int st, 
    	int slot) //TDateTime time_stamp;  int slot_number; // deprecated : 
    	rj_box_cid(rjbid), 
    	position(pos), 
    	cryovial_barcode(barcode), 
    	aliquot_type_cid(aliq), 
    	old_box_cid(oldbid), 
    	old_position(oldpos), 
    	new_position(newpos), 
    	process_cid(pid), 
    	status(st), 
    	slot_number(slot)

    	// st2 replace all: ctrl-alt-enter

            cbr.retrieval_cid, section AS chunk, cbr.rj_box_cid, //cbr.status, 
            cs.box_cid, sb.external_name AS src_box, cs.tube_position AS source_pos,  
            cbr.box_id AS dest_id, db.external_name AS dest_box, slot_number AS dest_pos, 
            lcr.process_cid AS lcr_procid, lcr.status AS lcr_status, lcr.slot_number AS lcr_slot, lcr.cryovial_barcode, lcr.aliquot_type_cid, 
            cs.note_exists, cs.cryovial_id, cs.cryovial_position, cs.status, 
            c.sample_id, cs.record_id, 
            db.external_name AS dest_name 

		int rjbid, 
    	int pos, 
    	std::string barcode, 
    	#int aliq, 
    	int oldbid, 
    	int oldpos, 
    	#int newpos, 
    	#int pid, 
    	#int st, 
    	int slot            
 
 LCDbCryovialRetrieval::LCDbCryovialRetrieval(const LQuery & query)
  	// doesn't fill in half the fields!!! it did say 'fixme'....
    position(query.readInt("dest_pos")), //??
    aliquot_type_cid(query.readInt("aliquot_type_cid")), //???slot_number(query.readInt("lcr_slot")),
    process_cid(query.readInt("lcr_procid")),
    status(query.readInt("lcr_status"))

    filled in missing fields, still not sure about 'position'/src pos/dest pos...

now:
    SELECT * FROM  l_cryovial_retrieval WHERE rj_box_cid = 4146617 AND position = 50

    returns a row, but 
     aliquot type is -31782
     select * from c_object_name where object_cid = -31782 # EDTA_2, EDTA_1 is -31781
    so if it updated anything (it didn't), it was the 2nd ali
    the rj_box_cid is that of the 2ndary aliquot, not the primary

    no mention/use of job->primaryaliquot? yes there is, they're defined already
    are they in the right order in the query, i.e. does my 
    	aliquot_type_cid (primary_aliquot < secondary_aliquot ? "ASC" : "DESC")
    thing work?
    # EDTA_1 is -31781, EDTA_2 is -31782, both -ve, so primary_aliquot < secondary_aliquot is NOT true, so it would be DESC, meaning...
    EDTA_1 -31781
    EDTA_2 -31782
    ...which looks right
    and it is DESC irl...
    





 
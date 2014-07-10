
//* Ask the relevant question(s) from the URS when they’re ready to finish
//    * only this: "). The option to exit the process saving progress should be offered, with an “are you sure?” message in case of accidental selection (REQ 8.3.12)."

//    } catch (Exception & e) {
//      TfrmRetrievalAssistant::msgbox(e.Message);
//    }

//    } catch (Exception & e) {
//      TfrmRetrievalAssistant::msgbox(e.Message);
//    } catch (...) {
//        TfrmRetrievalAssistant::msgbox("problem displaying plan");
//    }

//  } catch(Exception & e) {
//      AnsiString msg = e.Message;
//      frmRetrAsstCollectSamples->errors.push_back(msg.c_str());
//      frmRetrAsstCollectSamples->unactionedSamples = true;
//    } catch(char * e) {
//      frmRetrAsstCollectSamples->errors.push_back(e);
//      frmRetrAsstCollectSamples->unactionedSamples = true;
//  } catch (...) {
//      frmRetrAsstCollectSamples->errors.push_back("Unknown error");
//      frmRetrAsstCollectSamples->unactionedSamples = true;
//    }

//if (NULL == box) throw runtime_error(__FUNC__ +" null LPDbBoxName");
//if (NULL == size) throw runtime_error(__FUNC__ +" null LCDbBoxSize");
//if (NULL == tube) throw runtime_error(__FUNC__ +" null LCDbObject");
//operator() (const ProjBox lhs, const ProjBox rhs) { return lhs.project_cid == rhs.project_cid && lhs.box_cid == rhs.box_cid; }

/*
Chunk< SampleRow >::NOT_STARTED:RETRIEVAL_ASSISTANT_NEW_COLOUR;
Chunk< SampleRow >::INPROGRESS: RETRIEVAL_ASSISTANT_IN_PROGRESS_COLOUR;
Chunk< SampleRow >::DONE:       RETRIEVAL_ASSISTANT_COLLECTED_COLOUR;
*/

/*
    switch (status)  // could use currentAliquot() here?
        case LCDbCryovialRetrieval::EXPECTED:               background = RETRIEVAL_ASSISTANT_NEW_COLOUR;
        case LCDbCryovialRetrieval::IGNORED:                background = RETRIEVAL_ASSISTANT_IGNORED_COLOUR;
        case LCDbCryovialRetrieval::COLLECTED:              background = RETRIEVAL_ASSISTANT_COLLECTED_COLOUR;
        case LCDbCryovialRetrieval::NOT_FOUND:
            if (NULL != row->backup)
                switch (backupStatus)
                    case LCDbCryovialRetrieval::EXPECTED:   background = RETRIEVAL_ASSISTANT_SECONDARY_COLOUR
                    case LCDbCryovialRetrieval::IGNORED:    background = RETRIEVAL_ASSISTANT_IGNORED_COLOUR
                    case LCDbCryovialRetrieval::COLLECTED:  background = RETRIEVAL_ASSISTANT_COLLECTED_COLOUR
                    default:                                background = RETRIEVAL_ASSISTANT_NOT_FOUND_COLOUR
            else
                background = RETRIEVAL_ASSISTANT_NOT_FOUND_COLOUR */

// could use combineAliquots()?
        // void LoadVialsJobThread::combineAliquots(const vecpSampleRow & primaries, const vecpSampleRow & secondaries, vecpSampleRow & combined) {
        // primary_aliquot and secondary_aliquot are already defined
        //int currentAliquotType = row->cryo_record->getAliquotType();
        //int previousAliquotType = previous == NULL? 0 : previous->cryo_record->getAliquotType();
//        if (secondary_aliquot != 0 &&
//            secondary_aliquot == currentAliquotType &&
//            previous != NULL &&
//            previous->cryovial_barcode == row->cryovial_barcode) { // secondary aliquot, previous was primary of same sample
//            if (previousAliquotType == currentAliquotType) {
//                throw runtime_error("duplicate aliquot");
//            } else if (currentAliquotType != secondary_aliquot) {
//                throw runtime_error("spurious aliquot");
//            } else { // secondary
//                previous->backup = row;
//            }
//        } else {
//            frmRetrAsstCollectSamples->combined.push_back(row); // new primary
//            previous = row;
//            rowCount++; // only count primary aliquots
//        }

        // add box tube type name
        // I put this in a loop outside the main loop to avoid running another query...
        // doesn't seem to matter that it's not?
        //for (vector<SampleRow *>::iterator it = frmRetrAsstCollectSamples->combined.begin(); it != frmRetrAsstCollectSamples->combined.end(); ++it) {//, rowCount2++) {
//        for (vector<SampleRow *>::iterator it = frmRetrAsstCollectSamples->combined.begin(); it != frmRetrAsstCollectSamples->combined.end(); ++it) {//, rowCount2++) {
//            (*it)->dest_type_name = Util::boxTubeTypeName((*it)->project_cid, (*it)->dest_box_id).c_str();
//        }

//    int size1 = collect->primaries.size(), size2 = collect->secondaries.size(), size3 = collect->combined.size();
/*
    // find locations of source boxes
    // fixme should get storage for secondaries as well?
    // fixme this was put outside the main loop to avoid multiple queries as well - could actually be included in main loop?
    int rowCount2 = 0;
    for (vector<SampleRow *>::iterator it = collect->combined.begin(); it != collect->combined.end(); ++it, rowCount2++) {
        SampleRow * sample = *it;
        ostringstream oss; oss<<"Finding storage for "<<sample->cryovial_barcode<<" ["<<rowCount2<<"/"<<rowCount<<"]: ";
        main->getStorage(sample);
        if (NULL != sample->backup) {
            main->getStorage(sample->backup);
        }
        oss<<sample->storage_str(); loadingMessage = oss.str().c_str(); Synchronize((TThreadMethod)&updateStatus);
    } debugMessage = "finished load storage details"; Synchronize((TThreadMethod)&debugLog);
*/

//    // add box tube type name
//    for (auto &row: collect->primaries) {
//        try {
//            row->dest_type_name = Util::boxTubeTypeName(row->cbr_record->getProjId(), row->dest_box_id).c_str();
//        } catch (...) {
//            row->dest_type_name = "error";
//        }
//        try {
//            main->getStorage(row);
//        } catch (...) {
//            row->setLocation("error", 0, "error", 0, 0, "error", 0);
//        }
//        //row->dest_type_name = Util::boxTubeTypeName(row->cbr_record->getProjId(), row->dest_box_id).c_str();
//        //main->getStorage(row);
//    }
//
//    for (auto &row: collect->secondaries) {
//        try {
//            row->dest_type_name = Util::boxTubeTypeName(row->cbr_record->getProjId(), row->dest_box_id).c_str();
//        } catch (...) {
//            row->dest_type_name = "error";
//        }
//        try {
//            main->getStorage(row);
//        } catch (...) {
//            row->setLocation("error", 0, "error", 0, 0, "error", 0);
//        }
////        row->dest_type_name = Util::boxTubeTypeName(row->cbr_record->getProjId(), row->dest_box_id).c_str();
////        main->getStorage(row);
//    }

//const int pid = LCDbAuditTrail::getCurrent().getProcessID();
//wstringstream oss; oss<<__FUNC__<<oss<<"chunk:"<<chunk<<", rowCount: "<<rowCount; OutputDebugString(oss.str().c_str());

//    // add box tube type name
//    for (auto &row: collect->combined) {
//            row->dest_type_name = Util::boxTubeTypeName(row->cbr_record->getProjId(), row->dest_box_id).c_str();
//            main->getStorage(row);
//        if (NULL != row->backup) {
//            row->backup->dest_type_name = Util::boxTubeTypeName(row->backup->cbr_record->getProjId(), row->backup->dest_box_id).c_str();
//            main->getStorage(row->backup);
//        }
//    }
//, aliquot_type_cid " //<< (primary_aliquot < secondary_aliquot ? "ASC" : "DESC"

//      chunk = qd.readInt("chunk");
//        //wstringstream oss; oss<<__FUNC__<<oss<<"chunk:"<<chunk<<", rowCount: "<<rowCount; OutputDebugString(oss.str().c_str());
//        if (chunk > curchunk) { // new chunk, add the previous one
//            collect->addChunk(curchunk, rowCount-1);
//            curchunk = chunk;
//        }

// not needed in debug
//int row = sgVials->Row; sgVials->Row = row; // how to put these before and after to save row clicked on?

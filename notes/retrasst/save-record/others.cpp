bool LPDbX::saveRecord(LQuery query) {
	if (!saved) {
		claimNextID(query);
		query.setSQL("insert into ... (..., process_cid, time_stamp, note_exists)"
					 " values (..., :pid, 'now', :nc )" );
		query.setParam("pid", LCDbAuditTrail::getCurrent().getProcessID());
		query.setParam("nc", notes);
		query.setParam("cid", getID());
		if (!query.execSQL())
			return false;
	} else if (hasNotes()) {}
		query.setSQL("update ... set note_exists = note_exists + 1 where x_id = :cid");
		query.setParam("cid", getID());
		if (!query.execSQL())
			return false;
	}

	LPDbCryovialStore store(storeID, getID(), boxID, position);
	store.setStatus(LPDbCryovialStore::Status(status));
	return store.saveRecord(query);
}


bool LPDbCryovial::saveRecord( LQuery query )
{
	if( !saved )
	{
		claimNextID( query );
		query.setSQL( "insert into cryovial (cryovial_id, sample_id, cryovial_barcode,"
					 " aliquot_type_cid, process_cid, time_stamp, note_exists)"
					 " values ( :cid, :sid, :bar, :atid, :pid, 'now', :nc )" );
		query.setParam( "sid", sampleID );
		query.setParam( "bar", barcode );
		query.setParam( "atid", typeID );
		query.setParam( "pid", LCDbAuditTrail::getCurrent().getProcessID() );
		query.setParam( "nc", notes );
		query.setParam( "cid", getID() );
		if( !query.execSQL() )
			return false;
	}
	else if( hasNotes() )
	{
		query.setSQL( "update cryovial set note_exists = note_exists + 1"
					 " where cryovial_id = :cid" );
		query.setParam( "cid", getID() );
		if( !query.execSQL() )
			return false;
	}

	LPDbCryovialStore store( storeID, getID(), boxID, position );
	store.setStatus( LPDbCryovialStore::Status(status) );
	return store.saveRecord( query );
}

bool LPDbCryovialStore::saveRecord( LQuery query )
{
	if( !saved ) {
		claimNextID( query );
		query.setSQL( "insert into cryovial_store (record_id, cryovial_id, box_cid,"
					 " tube_position, time_stamp, status, note_exists, process_cid, retrieval_cid)"
					 " values ( :rid, :cid, :bid, :pos, 'now', :sts, 0, :pid, :jcid )" );
		query.setParam( "cid", getID() );
		query.setParam( "bid", boxID );
		query.setParam( "pos", position );
	} else {
		std::stringstream fields;
		fields << "update cryovial_store set status = :sts, retrieval_cid = jcid, process_cid = :pid";
		switch( status ) {
			case ALLOCATED:
			case CONFIRMED:
				fields << ", time_stamp = 'now'";
			break;
			case ANALYSED:
			case DESTROYED:
				fields << ", removed = 'now'";
		}
		if( volume >= 0 ) {
			fields << ", sample_volume = " << volume;
		}
		fields << " where record_id = :rid";
		query.setSQL( fields.str() );
	}
	query.setParam( "sts", status );
	query.setParam( "jcid", retrievalID );
	query.setParam( "pid", LCDbAuditTrail::getCurrent().getProcessID() );
	query.setParam( "rid", getID() );
	if( query.execSQL() ) {
        saved = true;
		return true;
	} else {
		return false;
    }
}
not found
  if primary != NOT_FOUND
  	mark not NOT_FOUND
  	if secondary
    	load secondary
   	else
   		nextrow
  else  # primary already marked NOT_FOUND
  	if secondary
  		mark secondary NOT_FOUND
  		nextrow
  	else
  		error, shouldn't be able to get here
  endif

doing "Not found" on a secondary causes access violation
  notFound()
     nextRow()
        showChunks()
            showChunk()
                fillRow()
                    sample = row->secondary
  because not enough strings in LCDbCryovialRetrieval::statusString() to cover status 6 (NOT_FOUND)
  and now sets it's status to 'Expected'
doing skip on 2nd gives not found colour

## done

secondary not in diffent colour-
reverts back to primary-

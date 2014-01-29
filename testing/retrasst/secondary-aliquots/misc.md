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

secondary not in diffent colour
reverts back to primary
doing "Not found" on a secondary now sets it's status to DELETED - bad pointer?
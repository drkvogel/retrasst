    -12896345|SHARP urine Nov 2013          |SHARP urine retrieval for Richard Haynes            No samples found
      -636363|Example box retrieval         |A THRIVE box retrieval for Chris                    None found but no message, sgvials is all blue
      -363636|Example cryovial retrieval    |A THRIVE cryovial retrieval for Chris               
       -14059|THRIVE Lp(a) Sep 2013         |THRIVE samples for a combined HPS/THRIVE retrieval  No samples found
       -14058|HPS Lp(a) Sep 2013            |HPS samples for a combined HPS/THRIVE retrieval     No samples found
       -14056|STICS troponin analysis       |14 boxes for a STICS serum_1 troponin retrieval     No samples found
        -4008|SHARP base&FU Oct 2013        |SHARP EDTA_2 retrieval - baseline and follow-up     No samples found
        -4004|SHARP baseline Oct 2013       |SHARP EDTA_2 retrieval - baseline only              No samples found
        -1015|THRIVE Lp(a) baseline         |Initial THRIVE Lp(a) analysis                       Loads ~4000 samples OK

saveRecord()

 not saving barcode, so collect query doesn't join

slot number?



 * Usual sort order before chunking: destination box and position
 * Usual sort order for each chunk: vessel position, shelf, structure, slot and current position

 * does LCDbCryovialRetrieval::saveRecord() insert work? - it does now
 * check cryo/store old/new params are correct for LCDbCryovialRetrieval
 * should use saveRecord for LCDbBoxRetrieval
 * signoff form
 * secondary aliquots should always saved if present

---------------------------
Debugger Exception Notification
---------------------------
Project Storeman.exe raised exception class Exception with message 'XDB error: IIAPI_ST_ERRORInfo: ERROR '23502' 6406: Cannot INSERT into table '"l_cryovial_retrieval"' because the values do not match those in table '"c_box_retrieval"' (violation of REFERENTIAL constraint '"fk_lcr_rj_box_cid"').
 svr_id_error     = 34361346
 svr_local error  = 6406
 svr_id_server    = 2846
 svr_server_type  = 0
 svr_severity     = IIAPI_SVR_DEFAULT ( 0x0 )
  : 'Wed Feb 12 15:50:25 2014 E_US1906_REFING_FK_INS_VIOLATION  Cannot INSERT into table '"l_cryovial_retrieval"' because the values do not match those in table '"c_box_retrieval"' (violation of REFERENTIAL constraint '"fk_lcr_rj_box_cid"').''.
---------------------------
Break   Continue   Help   
---------------------------



## todo

LCDbCryovialRetrieval::saveRecord()

## done

1st chunk red
could find out there's no samples really quite quickly without doing whole query... 

## misc

http://www.soulseekqt.net/SoulseekQT/Linux/SoulseekQt-2013-11-6.tgz 
quo: magnet:?xt=urn:btih:55E5E4FFC411BFCE9F7BB9221C9C17F1F24397DC

    int getPosition() const { return position; }
    int getBoxID() const { return boxID; }

LCDbBoxRetrieval
LCDbBoxRetrievals
LCDbCryovialRetrieval
LCDbCryovialRetrievals
    -12896345|SHARP urine Nov 2013          |SHARP urine retrieval for Richard Haynes            No samples found
      -636363|Example box retrieval         |A THRIVE box retrieval for Chris                    None found but no message, sgvials is all blue
      -363636|Example cryovial retrieval    |A THRIVE cryovial retrieval for Chris               
       -14059|THRIVE Lp(a) Sep 2013         |THRIVE samples for a combined HPS/THRIVE retrieval  No samples found
       -14058|HPS Lp(a) Sep 2013            |HPS samples for a combined HPS/THRIVE retrieval     No samples found
       -14056|STICS troponin analysis       |14 boxes for a STICS serum_1 troponin retrieval     No samples found
        -4008|SHARP base&FU Oct 2013        |SHARP EDTA_2 retrieval - baseline and follow-up     No samples found
        -4004|SHARP baseline Oct 2013       |SHARP EDTA_2 retrieval - baseline only              No samples found
        -1015|THRIVE Lp(a) baseline         |Initial THRIVE Lp(a) analysis                       Loads ~4000 samples OK

## todo

 * check cryo/store old/new params are correct for LCDbCryovialRetrieval
 * should use saveRecord for LCDbBoxRetrieval
 * signoff form
 * secondary aliquots should always be saved if present
 * slot number? (what about it?)

## misc

 * Usual sort order before chunking: destination box and position
 * Usual sort order for each chunk: vessel position, shelf, structure, slot and current position

## done

* does LCDbCryovialRetrieval::saveRecord() insert work? - it does now
not saving barcode, so collect query doesn't join
saveRecord()
LCDbCryovialRetrieval::saveRecord()
1st chunk red
could find out there's no samples really quite quickly without doing whole query... 

## misc

questions for meeting
 observation - still going on?
 slot number?
 bugzilla bugs (gencomms, 
 nova

http://www.soulseekqt.net/SoulseekQT/Linux/SoulseekQt-2013-11-6.tgz 

    int getPosition() const { return position; }
    int getBoxID() const { return boxID; }

LCDbBoxRetrieval
LCDbBoxRetrievals
LCDbCryovialRetrieval
LCDbCryovialRetrievals

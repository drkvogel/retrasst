### destination box types

It would now be useful to see destination box types in the lists, as we've ascertained that retrieval jobs can contain multiple aliquots from multiple projects, and some aliquots (vacutainer?) require different types of boxes to be put in. Display the box type as a column in the job/plan list and allow sorting on this column.

Keep relevant dest box info loose in SampleRow struct or in LPDbBoxName (has getTypeID() method)?

#### store box info in LPDbBoxName *

LPDbBoxName query constructor needs 'status' which is already used in the big query.
Other constructors are for creating new boxes. 

Want: box id, box type id, box name (not same as barcode?), status?

##### use query constructor

    LPDbBoxName( const LQuery & query );

    "external_name"     // doesn't clash yet
    "barcode"           // don't need?
    "status"            // clashes, but can be set after with setStatus()
    "box_type_cid"      // OK

##### id constructor

    LPDbBoxName( int id = 0 )
    : LPDbID( id ), status( EMPTY ), boxTypeID( 0 ), filledBy( 0 )

Can set status (don't need to?), can't set name, type

##### name and type constructor

    LPDbBoxName( const std::string & label, int typeID = 0 )
     : name( label ), status( IN_USE ), boxTypeID( typeID ), filledBy( 0 )

Set name and box type, then change status. But can't set ID.

//Suggest storing

#### store loose

Actually looking easiest.

#### get box type from box id

>Nick might know if there is some pre-rolled code to extract box type 
 information from a box id? (doesn't look like there is at the moment...) 
 
If you find the box details, e.g. using something like this: 
 
    const LPDbBoxName * box = LPDbBoxNames::records().readRecord(query, id); 
 
You can get the layout (I've made the method public): 
 
    const LCDbBoxSize * size = box->getLayout(); 
 
and then look up the tube type in `c_object_name`: 
 
    const LCDbObject * tube = LCDbObjects::records().findByID(size->getTubeType()); 


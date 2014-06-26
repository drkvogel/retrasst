# refboxes todo

## general

need way of recreating test cases, probably, and reset button

## dummy run

Why? Is this needed? For me for debugging, or required by spec or from meeting?

Improvements suggested:

1) Could be made more visually explicit - a popup for example

2) Automatically do live run after dummy run, on error:

  a) stop
  b) colour differently and skip

e.g. `vlab::test_ldbc STICS SERUM_2`

### in FindMatchesWorkerThread::Execute()

tank_name not filled in
could cache tank name

does query get tank and rack or rack and slot as comment says?

no, but `rack_cid` and `slot_position` could be got from `box_store`, which is LEFT JOINed - and sometimes returns no results, so `rack_cid` and `slot_position` would be null
because there is no `box_store` record because it is referred?

`/testing/refboxes/matching-boxes.sql`

    BoxArrivalRecord
        (int recn, int ltid, int psid, int baid, int pjid, TDateTime stime, string bname, int st,
        string firstbar, int firstpos, string lastbar, int lastpos,
        int tankid, string racknum, int rackslot, TDateTime tstamp) :

    tank_cid(tankid), rack_name(racknum)

    BoxArrivalRecord * box2 = new BoxArrivalRecord(... 
     0, "", ...) <-- tank and rack are null values
    frmReferred->matchingBoxes2.push_back(box2);

    findMatchesWorkerThreadTerminated
        it = matchingBoxes2.begin(); it != matchingBoxes2.end(); it++, i++) {
        BoxArrivalRecord * box = *it;
        sgMatches->Objects[0][i] = (TObject *)box;

    sgMatchesMouseUp() 
        selectedMatch = (BoxArrivalRecord *)sgMatches->Objects[0][row];

    editedBox = *selectedMatch;

    oss <<"Rack not found.\n\ntank: '"<<editedBox.tank_name<<"'\nrack: '"<<editedBox.rack_name<<"'";


    // box->tank_name  = getVesselName(box->tank_cid);

findStorage could be used here as well as in Retrasst?


## looking through all projects slow

Checking all projects for clashing storage takes a while, less on subsequent boxes (due to Ingres caching?), could be very time-consuming when there are a lot of referred boxes. 

Could now possibly use `c_slot_allocation` to search central - discuss with Nick.

## group actions

multiple rows could be actioned together to save time.
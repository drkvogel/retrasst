
autochunk: goes over last vial
chunks: put in db
vials: select 1st row/scroll to top when new chunk created
chunks: sizes are wrong - 1 too high? no, size calc is ok (0-indexed should be +1), something else wrong
chunks: can go over end of vials - check

Atkins/Ruskin 1h35, 2h07
sort out add first chunk mess:
    count the number of rows and put that as the end and size of the initial chunk
    double-clicking a row marks the start of a new chunk:
        set the previous row as the end of the previous chunk
        create a new chunk:
            set its start to that of the row clicked on
            set its end to the end of the whole list
    pressing incr increments the end of the current chunk (not in spec?)
    pressing decr decrements the end of the current chunk
    changing values in the chunk stringgrid changes chunk parameters? (not in spec?)

# done

### how to organise list and chunks
sgwVials: chunks[] // indexes to start/end of list
   or
Chunk * list? // pointer to *part* of vector...hmmm * wrapper->sort
    
### boilerplate    
    
/*
    ostringstream oss; oss<<__FUNC__; debugLog(oss.str().c_str());
    LQuery q(LIMSDatabase::getCentralDb());
    //LQuery q(Util::projectQuery(project), true); // get ddb with central and project dbs
    q.setSQL("SELECT * FROM  WHERE status != 99");
    Screen->Cursor = crSQLWait;
    q.open();
    delete_referenced<vecp>(s);
    while (!q.eof()) {
        RetrievalPlan * plan = new RetrievalPlan(q.readString("name"));
        //ob-> = q.readInt("");
        //ob-> = q.readString("");
        s.push_back();
        q.next();
    }
    Screen->Cursor = crDefault;
*/

### chunk split    
    
add chunk-
make it selected-
show chunks (ie. show contents of new chunk)-
chunk stringgridwrapper refactoring-
chunking
sgBoxesFixedCellClick
sortChunk    
template/breakpoint nonsense
it (Cbuilder) seems to be optimizing things even though it's told not to; you can't set breakpoints in certain places
    templates can't be debugged and unused templated code is not compiled
reject list-
get rid of samplechunk, boxchunk?-
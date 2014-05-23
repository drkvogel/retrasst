## long waits for queries slows down dev

can do 'other things' in meantime. 
make program send email to myself with details of how long each query took, for records.

## lcdbjob::str()

put debug string in here, inc. name, desc, id, etc
primary, secondary aliquot

## bad chunk

### 978238 several REVEAL boxes

>job: 978238, projectid: -582959, status: 1, type: 4: desc: Several REVEAL boxes for Chris

#### plan

699 total


1.  EDTA_BC     199
2.  EDTA_2      100
3.  EDTA_1      400

#### collect

1.  EDTA_BC     199
2.  ?           0
3.  EDTA_1      400

chunk 2 is size 0.

i.e. secondaries (EDTA_2) just ain't there.

    for (vecpSampleRow::const_iterator it = secondaries.begin(); it != secondaries.end(); it++) {
        combined.push_back(row);

because of recent changes?

box types are all the same?
    e.g. 978238 QClot_new 978237
    are they meant to be?
    take out of main loop

### job: 978253, projectid: -149662, status: 1

10 primaries with secondary backups
split into 3 chunks
looks OK

### job id: 978243 "Retrieval_978243", "Reveal boxes of three types"

3 different aliquot types but box types all same
sort and chunk by aliquot type
load plan
middle chunk (2) is 0 size and crashes
other chunks are 500 size each and seem to contain more than one aliquot


### ideas

try earlier commits? e.g.
84af27cefeeeaac499ce3e210db2be8cf9961756 [84af27c] before cvs pull



## fixme    

---

###  978243 "Retrieval_978243", "Reveal boxes of three types"

Contains secondaries, but not as backups (not same destination position) of primaries

### 978243 ("Reveal boxes of three types")

Primaries with secondaries (?) as backups




## confusing methods?

currentSample(), 
currentAliquot()
chunk->currentObject()
fillRow()


## chunkComplete()

## SaveProgressThread::Execute()

## SaveProgressThread::storeSample()

## SaveProgressThread::jobFinished()


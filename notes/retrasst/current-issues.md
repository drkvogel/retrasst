

* sometimes current storage info will not be available (box has not been through box reception and storage sync, hence status will be CONFIRMED [2] rather than IN_TANK [4]), but box name and tube pos will be
* finish chunk embarrasment - 'handle close'?? fencepost bug?

also there was an ingres error at one point:

    XDB error: IIAPI_ST_ERRORInfo: ERROR '22008' 4304: 21 is not a valid month for a date/time column.
     svr_id_error     = 34078728
     svr_local error  = 4304
     svr_id_server    = 2137
     svr_server_type  = 0
     svr_severity     = IIAPI_SVR_DEFAULT ( 0x0 )
        : 'Wed Jul  2 14:12:28 2014 E_US10D0_4304   21 is not a valid month for a date/time column.'

on L21:

    C:\Users\cbird>ingprenv
    ...
    II_DATE_FORMAT=US

on Q108:

    2014-07-02 16:31:57 cbird@Q108 /cygdrive/e
    $ ingprenv
    ...
    II_DATE_FORMAT=multinational4

on L21 (as administrator):

    C:\Users\cbird>ingsetenv II_DATE_FORMAT
    Value : multinational4


## confusing methods?

currentSample(), 
currentAliquot()
chunk->currentObject()
fillRow()

---

nextRow()
    if DONE == chunk->getStatus()
        chunkCompleted()
     if (isJobComplete())
        if (IDYES != Application->MessageBox(L"Save job? Are all chunks completed?", L"Info", MB_YESNO)) return;
        ModalResult = mrOk;    

chunkCompleted()

isJobComplete() 
    true if all chunks complete

exit()
calls:

SaveProgressThread::Execute()

    foreach sample
        find source box and cache
        if status not in EXPECTED, IGNORED
            storeSample
        else
            unactionedSamples = true
        if backup
            storeSample(sample->backup)



SaveProgressThread::storeSample()

SaveProgressThread::jobFinished()

saveProgressThreadTerminated
-> collectEmpties()

## debugging rosetta/database errors

put breakpoints on 

    LIMSDatabase::xerrorCallback()
    LIMSDatabase::rosettaCallback()
    LIMSDatabase::xdbErrorCallback()

---

LIMSDatabase::getProjectDb()
LCDbProject::isInCurrentSystem()
Tank::getVessel()? // Inventory.h
TfrmNewJob::createJob
SampleEntry
StoreDAO::findBox, addToRetrieval

    findBox( int box_id, int proj_id, ROSETTA & result )    
    loadBoxDetails()

## difference between addSampleDetails and getStorage

getStorage
    # just gets storage details
addSampleDetails 
    # calls getStorage and boxTubeTypeName on preferred and backup
    # ie. good to run once samples have been combined - to avoid doing in main loop    
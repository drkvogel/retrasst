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
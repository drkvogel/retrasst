
## fixme

`LoadPlanThread::Execute()`

    // find locations of source boxes
    // fixme should get storage for secondaries as well?
    // fixme this was put outside the main loop to avoid multiple queries as well - could actually be included in main loop?

## storeman log in

Hello both 
 
I have modified the log-in screen to select the appropriate database system for the new setup 
 
    Live = vnode_vlab_64::ldb* (the default for the release version) 
    Test = vnode_vlab_64::test_ldb* 
    Mirror = vnode_labdev_64::ldb* 
    Dev = vnode_labdev_64::t_ldb* (the default for the debug version) 
    Db Test = vnode_labdev_64::test_ldb* 
 
Please make sure you have your vnodes set up correctly. Don't use "mirror" if you're not sure. 
 
Jonathan: can you use the database connection from the log-in screen in your code? I'm happy to change my code to make that easier 
-- 
Nick 


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


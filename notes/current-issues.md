##

## combineAliquots()

does combineAliquots work properly?


## aliquot type colour key

// plan

    labelVialKeyPrimary->Color     = RETRIEVAL_ASSISTANT_PLANNED_COLOUR;
    labelVialKeySecondary->Color   = RETRIEVAL_ASSISTANT_SECONDARY_COLOUR;
    labelVialKeyCombined->Color    = RETRIEVAL_ASSISTANT_COMBINED_COLOUR;
    labelVialKeyExtra->Color       = RETRIEVAL_ASSISTANT_EXTRA_COLOUR;

// collect

    labelVialKeyExpected->Color             = RETRIEVAL_ASSISTANT_NEW_COLOUR;
    labelVialKeyExpectedSecondary->Color    = RETRIEVAL_ASSISTANT_PLANNED_COLOUR;
    labelVialKeyIgnored->Color              = RETRIEVAL_ASSISTANT_IGNORED_COLOUR;
    labelVialKeyCollected->Color            = RETRIEVAL_ASSISTANT_COLLECTED_COLOUR;
    labelVialKeyNotFound->Color             = RETRIEVAL_ASSISTANT_NOT_FOUND_COLOUR;

## chunk colour key

necessary?

    labelNew->Color         = RETRIEVAL_ASSISTANT_NEW_COLOUR;
    labelInProgress->Color  = RETRIEVAL_ASSISTANT_IN_PROGRESS_COLOUR;
    labelDone->Color        = RETRIEVAL_ASSISTANT_COLLECTED_COLOUR;

## plan "disappears"

If you save a plan, then come out of the program (e.g. to recompile) and back into it again, you may see that the plan that you just saved seems to have disappeared - it is not on the list. This is because it has been claimed by your previous session (with a different process ID) for 10 minutes. ie.:

    bool LCDbCryoJob::isAvailable() 

returns `false`.

###  978243 "Retrieval_978243", "Reveal boxes of three types"

Contains secondaries, but not as backups (not same destination position) of primaries

### 978243 ("Reveal boxes of three types")

Primaries with secondaries (?) as backups

why is seconday aliquot label greyed out in collect? ie. 

    labelSecondary->Enabled = false;



can we use it in collect?



## confusing methods?

currentSample(), 
currentAliquot()
chunk->currentObject()
fillRow()


## chunkComplete()

## SaveProgressThread::Execute()

## SaveProgressThread::storeSample()

## SaveProgressThread::jobFinished()


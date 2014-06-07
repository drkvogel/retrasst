## cache/hash column names

not important

## more loading messages

to show something's happening

## move Util::getAliquotDescription() to LCDbCryoJob?

So `job.str()` can print meaningful aliquot types

## debugLog() should take std::string?

    debugLog()

### weird bomb-outs

calling

    row->dest_type_name = Util::boxTubeTypeName(row->cbr_record->getProjId(), row->dest_box_id).c_str();

inside or out of the main loops seems to cause `LoadPlanThread` to terminate early (leaving 0 chunks and therefore an exception). rebooted as weird things happening... Turns out Ingres on `vlabdev` was throwing wobblies.

## box types are all the same?

    e.g. 978238 QClot_new 978237
    are they meant to be? yes, no jobs with multiple box types yet
    take out of main loop
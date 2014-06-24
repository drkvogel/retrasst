# deferred

## rack not found

May mean that, though a rack is possible in the layout, nothing had been added to it before and the right records 
[which?]
had not been created in Storage Sync because it referred the box instead (e.g. due to -ve id).

Possible solutions:

1) All rack layouts should have [whatever rack records are required] added by default - could be done with a script/small program - in which case, nothing to do for me

2) The record could be created on the fly by Referred Boxes

3) The findRack() shared method should create it on the fly

### findRack()

should create racks if in layout but not yet stored?

    StoreDAO::occupyRack()

    LCDbRack rackData("", editedBox.tank_cid, editedBox.rack_name);
            if (!rackData.findRack(qc)) 
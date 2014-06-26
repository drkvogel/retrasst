## 02 July 2014 13:30:00

## Storeman meeting 2014-06-17 - configure, browse and move

Laptop setup:
* DHCP was not enabled for 'Local Area Connection'
* Shortcut to 'Storeman on K' - 'Storeman has stopped working'
* 64-bit Windows
* Both 32 and 64-bit Ingres:
    * C:\Program Files\Ingres\IngresIJ
* C:\Program Files (x86)\Ingres\IngresII
* IJ instance started by default
* But has no vnodes set up
* Set up vnode_labdev_64 and vnode_vlabreal_64, tested connection, OK
* Ingres 10.0.0 

Login screen change

## Storeman meeting 2014-06-19 - analyse, transfer, refer

## general

need way of recreating test cases, probably

reset button

## records too dark grey-

`SLOT_CONFIRMED` records (`vlab::test_ldbc`, THRIVE and Kadoorie) too dark grey - because not a status I expected. Actually these are valid cases, which occur when storage sync encounters boxes with negative ids, and refers them [check]

clLtGray looks the same as clSilver

## kadoorie boxes have no `c_box_name` records-

Thus, can only be discarded. The data needs to be imported from the Kadoorie project database, which is a bit of a political wrangle, but should happen, so not something my program has to worry about.

## dummy run

Why? Is this needed? For me for debugging, or required by spec or from meeting?

Improvements suggested:

1) Could be made more visually explicit - a popup for example

2) Automatically do live run after dummy run, on error:

  a) stop
  b) colour differently and skip

e.g. `vlab::test_ldbc STICS SERUM_2`

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

### msgbox 

too small
terminology

## looking through all projects slow

Checking all projects for clashing storage takes a while, less on subsequent boxes (due to Ingres caching?), could be very time-consuming when there are a lot of referred boxes. 

Could now possibly use `c_slot_allocation` to search central - discuss with Nick.

## group actions

multiple rows could be actioned together to save time.

## fossil tickets should be ordered by date?

might be better to retain sense of chronology

## discard boxes

I reformatted code and fixed a bug, Nick made some minor changes, it now doesn't crash and works with current database - possibly nothing to do.

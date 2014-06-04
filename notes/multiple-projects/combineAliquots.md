## fix/replace combineAliquots in collect

replace with code similar to old method, based on assumption that backups will be immediately after preferred vials in the plan (which they will)

## are secondaries stored by plan and how? (yes)

    978253 "Retrieving THRIVE samples (with secondary)"

reset job
run plan query - no results.
10 combined vials. order by pos 
chunks
1. dest pos 1, 2, 4
2. dest pos 5, 7, 11
3. dest pos 12, 13, 14, 16
save

secondaries are included in plan with separate lcr_pos to primaries
ie. if plan is *saved* correctly, secondaries should appear in list after the primaries they are a backup of
is this the case?
looks like it in SavePlanThread::save() and from plan query
then in collect, don't need to sort by aliquot - they should be in the right order - and each secondary should be able to be matched to a previous primary.
but because of combineAliquots()(?) the order is messed up.

what would combineAliquots() do to this list?
looks like it does it correctly
should work on combined one
but if there are loose secondaries, they would be the end of the combined list, possibly out of chunk order

can combineAliquots combine backups with primaries correctly as it stands? no
can it be made to do so? er...
do we need a new function to combine in collect? maybe do it the old way

they should not be sorted into prim/sec in the main loop, just in one list which will be ordered correctly
combine function should add backup to previous if they match, loose if not
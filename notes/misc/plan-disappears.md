## plan "disappears"

If you save a plan, then come out of the program (e.g. to recompile) and back into it again, you may see that the plan that you just saved seems to have disappeared - it is not on the list. This is because it has been claimed by your previous session (with a different process ID) for 10 minutes. ie.:

    bool LCDbCryoJob::isAvailable() 

returns `false`.
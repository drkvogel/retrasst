
#include <vcl.h>
#include <dbtables.hpp>
#include <stdlib.h>
#include "MachineIncludeClass.h"


/*@*/

MachineIncludeClass::MachineIncludeClass()
: MachineName("")
{
    MachineID = 0;
    Include = true;
}

MachineIncludeClass::MachineIncludeClass(AnsiString aName)
: MachineName(aName)
{
    MachineID = 0;
    Include = true;
}

__fastcall MachineIncludeClass::~MachineIncludeClass()
{
}




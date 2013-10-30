/*
 *  Semaphore-based IPC, based on code originally written by Hugh
 *
 *	14 May 2004, NG:    Moved .cpp & .h to Shared/LabUtils
 *  18 June, NG/AM:     Checked in to CVS/GeneralClasses/CoreUtilities
 */

#include <vcl.h>
#include "TwoWayTalk.h"

#pragma hdrstop
#pragma package(smart_init)

//---------------------------------------------------------------------------

const char * TwoWayTalk::c_to_v = "COMS_VLD";
const char * TwoWayTalk::v_to_c = "BTA_COMS";
const char * TwoWayTalk::v_to_ba = "VLD_BTA";
const char * TwoWayTalk::ba_to_v = "BTA_VLD";

//---------------------------------------------------------------------------
//  Open or create a semaphore with the given name
//---------------------------------------------------------------------------

TwoWayTalk::TwoWayTalk( const char * name )
{
    semaphore = CreateSemaphore( NULL, 0, MAXLONG, name );
    if( semaphore == NULL )
        throw Exception( "Unable to create semaphore" );

    local_count = -1;       // first CheckIn() should return false
}

//---------------------------------------------------------------------------
//  Tell Windows we've finished using the semaphore
//---------------------------------------------------------------------------

TwoWayTalk::~TwoWayTalk()
{
    CloseHandle( semaphore );
}

//---------------------------------------------------------------------------
//  Returns true if no other talkers have checked in since this one
//---------------------------------------------------------------------------

bool TwoWayTalk::CheckIn()
{
    long shared_count;
    if( !ReleaseSemaphore( semaphore, 1, &shared_count ) )
        throw Exception( "Unable to release semaphore" );

    if( shared_count == local_count )
    {
        local_count ++;
        return true;
    }
    else
    {   local_count = shared_count + 1;
        return false;
    }
}

//---------------------------------------------------------------------------


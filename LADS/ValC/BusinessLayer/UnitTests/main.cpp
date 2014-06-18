#include <vcl.h>
#include <windows.h>

#include <iostream>
#include <tut.h>

#include <TutCallback.h>

#pragma hdrstop
#pragma argsused

#include <tchar.h>
#include <cstdio>

#include "BuddyDatabaseEntryIndexTest.h"
#include "ControlModelTest.h"
#include "ForceReloadTest.h"
#include "QueuedSamplesFactoryTest.h"
#include "RerunTest.h"
#include "RuleEngineTest.h"
#include "RuleLoaderTest.h"
#include "SampleRunGroupModelTest.h"
#include "WorklistRelativeTest.h"

#include "Thread.h"

void pause()
{
	std::puts( "[enter] to quit...\n" );
	std::getchar();
}

int old_tmain(int argc, _TCHAR* argv[])
{
	TutCallback callback;
    tut::runner.get().set_callback(&callback);
    // run all tests in all groups
    tut::runner.get().run_tests();

    pause();
    return EXIT_SUCCESS;
}

class TestRunner : public paulst::Runnable
{
public:
    void run( const paulst::Event* stopSignal )
    {
        old_tmain( 0, NULL );
    }
};


int _tmain(int argc, _TCHAR* argv[])
{
    /*
    The process of running the tests is pushed onto a new thread.

    This is because, without using a new thread, many unit tests fail at the following line in API.cpp:
 
	    paulstdb::throwUnlessOK( CoInitializeEx( NULL, COINIT_MULTITHREADED ) );

    This method fails with: HRESULT - 0x80010106 - Cannot change thread mode after it is set. 

    My interpretation: the 'main' thread has already had 'CoInitialize' (or 'CoInitializeEx') invoked for it.  Hence this error.

    For more info, try: http://support.microsoft.com/kb/824480
    */
    TestRunner testRunner;
    paulst::Thread t(&testRunner);
    return EXIT_SUCCESS;
}


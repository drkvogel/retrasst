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
#include "BuddySampleIDKeyedOnSampleRunIDTest.h"
#include "ForceReloadTest.h"
#include "QueuedSamplesFactoryTest.h"
#include "RerunTest.h"
#include "RuleEngineTest.h"
#include "RuleLoaderTest.h"
#include "SampleRunGroupModelTest.h"
#include "SampleRunIDResolutionServiceTest.h"
#include "WorklistRelativeTest.h"


void pause()
{
	std::puts( "[enter] to quit...\n" );
	std::getchar();
}

int _tmain(int argc, _TCHAR* argv[])
{
	TutCallback callback;
    tut::runner.get().set_callback(&callback);
    // run all tests in all groups
    tut::runner.get().run_tests();

    pause();
    return EXIT_SUCCESS;
}


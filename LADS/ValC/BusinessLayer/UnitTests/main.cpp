#include <vcl.h>
#include <windows.h>

#include <iostream>
#include <tut.h>

#include <TutCallback.h>

#pragma hdrstop
#pragma argsused

#include <tchar.h>
#include <stdio.h>

#include "ForceReloadTest.h"
#include "QueuedSamplesFactoryTest.h"

void pause()
{
        std::cout << "[enter] to quit..."  << std::endl;
        char c;
        std::cin >> std::noskipws >> c;
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

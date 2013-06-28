#include <cstdlib>
#include <iostream>
#include <tut.h>
#include <TutCallback.h>

#include "MyComponentTest.h"


namespace tut
{
    test_runner_singleton runner;
}

void pause()
{
    std::cout << "[enter] to quit..."  << std::endl;
    char c;
    std::cin >> std::noskipws >> c;
}

int main()
{
    TutCallback callback;
    tut::runner.get().set_callback(&callback);
    // run all tests in all groups
    tut::runner.get().run_tests();

    pause();
    return EXIT_SUCCESS;
}


#include <vcl.h>
#include <windows.h>

#include <System.hpp>
#include <cstdlib>
#include <iostream>
#include <tut.h>

#include <TutCallback.h>

#include "CSVIteratorTest.h"
#include "FixedCapacityBufferTest.h"
#include "ConfigTest.h"
#include "PropertiesTest.h"
#include "StringDBTest.h"
#include "StrUtilTest.h"
#include "UtilTest.h"

#include <tchar.h>
#include <stdio.h>

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

int _tmain(int argc, _TCHAR* argv[])
{
    TutCallback callback;
    tut::runner.get().set_callback(&callback);
    // run all tests in all groups
    tut::runner.get().run_tests();

    pause();
    return EXIT_SUCCESS;
}


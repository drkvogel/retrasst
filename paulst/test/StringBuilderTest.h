#ifndef STRINGBUILDERTEST_H
#define STRINGBUILDERTEST_H

#include <tut.h>

#include "StringBuilder.h"

namespace tut
{
	class StringBuilderTestFixture
    {
    };

    typedef test_group<StringBuilderTestFixture, 4> StringBuilderTestGroup;
	StringBuilderTestGroup testGroupStringBuilder("StringBuilder tests");
	typedef StringBuilderTestGroup::object testStringBuilder;

    template<>
    template<>
	void testStringBuilder::test<1>()
    {
        using namespace paulst;
        set_test_name("Some basic tests");

        ensure_equals( std::string() << 1, std::string("1") );
        ensure_equals( std::string("B") << "ang!", std::string("Bang!") );
        ensure_equals( std::string("12") << 34 << "56", std::string("123456") );

    }
     
};

#endif

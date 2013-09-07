#ifndef PROPERTIES_TEST_H
#define PROPERTIES_TEST_H

#include <tut.h>

#include "Properties.h"

namespace tut
{
	class PropertiesTestFixture
    {
    };

    typedef test_group<PropertiesTestFixture, 3> PropertiesTestGroup;
	PropertiesTestGroup testGroupProperties(
							"Properties tests");
	typedef PropertiesTestGroup::object testProperties;

    template<>
    template<>
	void testProperties::test<1>()
    {
        set_test_name("a=b,c=d,");

        using namespace paulst;

        Properties p("a=b,c=d,");

        ensure_equals( p.getValue("a"), std::string("b") );
        ensure_equals( p.getValue("c"), std::string("d") );
    }

    template<>
    template<>
	void testProperties::test<2>()
    {
        set_test_name("a=Fred Blogs,c=d,");

        using namespace paulst;

        Properties p("a=Fred Blogs,c=d,");

        ensure_equals( p.getValue("a"), std::string("Fred Blogs") );
        ensure_equals( p.getValue("c"), std::string("d") );
    }

    template<>
    template<>
	void testProperties::test<3>()
    {
        set_test_name("a=Fred Blogs,c=,x=y,");

        using namespace paulst;

        Properties p("a=Fred Blogs,c=,x=y,");

        ensure_equals( p.getValue("a"), std::string("Fred Blogs") );
        ensure_equals( p.getValue("c"), std::string("") );
        ensure_equals( p.getValue("x"), std::string("y") );
    }

};

#endif

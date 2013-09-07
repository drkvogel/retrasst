#ifndef CONFIGTESTH
#define CONFIGTESTH

#include <tut.h>

#include "Config.h"

namespace tut
{
	class ConfigTestFixture
    {
    };

    typedef test_group<ConfigTestFixture, 11> ConfigTestGroup;
	ConfigTestGroup testGroupConfig( "Config tests");
	typedef ConfigTestGroup::object testConfig;

    template<>
    template<>
	void testConfig::test<1>()
    {
        set_test_name("name on one line, value on the next, then empty line");

        paulst::Config cfg(
            "name:\n"
            "value\n"
            "\n");
        ensure( cfg.contains( "name" ) );
        ensure( cfg.get("name") == "value" );
    }

    template<>
    template<>
	void testConfig::test<2>()
    {
        set_test_name("missing value");

        paulst::Config cfg(
            "name:\n"
            "\n");
        ensure( cfg.contains( "name" ) );
        ensure( cfg.get("name") == "" );
    }

    template<>
    template<>
	void testConfig::test<3>()
    {
        set_test_name("encoded newline in value");

        paulst::Config cfg(
            "name:\n"
            "one\\ntwo\n"
            "\n");
        ensure( cfg.contains( "name" ) );
        ensure( cfg.get("name") == "one\ntwo" );
    }

    template<>
    template<>
	void testConfig::test<4>()
    {
        set_test_name("2 adjacent property names");

        paulst::Config cfg(
            "name:\n"
            "one:\n"
            "\n");
        ensure( cfg.contains( "name" ) );
        ensure( cfg.get("name") == "one:" );
    }

    template<>
    template<>
	void testConfig::test<5>()
    {
        set_test_name("2 multiline property values");

        paulst::Config cfg(
            "first one:\n"
            "my\n"
            " property\n"
            " val\n"
            "ue\n"
            "\n"
            "\n"
            "second one:\n"
            "another\n"
            " property\n"
            " val\n"
            "ue\n"
            "\n");
        ensure( cfg.contains( "first one" ) );
        ensure( cfg.get("first one") == "my property value" );
        ensure( cfg.contains( "second one" ) );
        ensure( cfg.get("second one") == "another property value" );
    }

    template<>
    template<>
	void testConfig::test<6>()
    {
        set_test_name("encoded escape sequences");

        paulst::Config cfg(
            "first one:\n"
            "my\\r\\nproperty\\\\nightmare\\\\\\n\n"
            "\n");
        ensure( cfg.contains( "first one" ) );
        ensure( cfg.get("first one") == "my\r\nproperty\\nightmare\\\n" );
        ensure_not( cfg.contains( "but I never specified this one" ) );
    }

    template<>
    template<>
	void testConfig::test<7>()
    {
        set_test_name("comments ignored");

        paulst::Config cfg(
            "first one:\n"
            "my\n"
            "# this comment should be ignored\n"
            " property\n"
            " val\n"
            "ue\n"
            "\n");
        ensure( cfg.contains( "first one" ) );
        ensure( cfg.get("first one") == "my property value" );
    }

    template<>
    template<>
	void testConfig::test<8>()
    {
        set_test_name("exception if no such property");

        paulst::Config cfg("");
        try
        {
            cfg.get("nonexistent");
            ensure( "shouldn't get this far", false );
        }
        catch( const Exception& e )
        {
            ensure( "Exception message should mention the missing property", e.Message.Pos( L"nonexistent" ) );
        }
    }

};
 
#endif


#ifndef UTIL_TEST_H
#define UTIL_TEST_H

#include <tut.h>

#include <map>
#include <utility>

#include "../../paulst/Util.h"

namespace tut
{
	class UtilTestFixture
    {
    };

    typedef test_group<UtilTestFixture, 21> UtilTestGroup;
	UtilTestGroup testGroupUtil( "Util tests");
	typedef UtilTestGroup::object testUtil;

    template<>
    template<>
	void testUtil::test<1>()
    {
        set_test_name("toDateTime: 2012-02-06T12:24:03");

        using namespace paulst;

        TDateTime t = toDateTime("2012-02-06T12:24:03" );

        unsigned short year, month, day, hour, min, sec, msec;
        year = month = day = hour = min = sec = msec = 0;

        t.DecodeDate( &year, &month, &day );
        t.DecodeTime( &hour, &min, &sec, &msec );

        ensure_equals( year, 2012 );
        ensure_equals( month, 2   );
        ensure_equals(   day, 6   );
        ensure_equals(  hour, 12  );
        ensure_equals(   min, 24  );
        ensure_equals(   sec, 3   );
        ensure_equals(  msec, 0   );
    }

    template<>
    template<>
	void testUtil::test<2>()
    {
        set_test_name("toDateTime: empty string");

        using namespace paulst;

        TDateTime t;
        TDateTime defaultVal(1900, 1, 1, 0, 0, 0, 0);

        try
        {
            t = toDateTime("", defaultVal );
        }
        catch( Exception& e )
        {
            std::wcerr << e.Message.c_str();
            ensure( false );
        }

        unsigned short year, month, day, hour, min, sec, msec;
        year = month = day = hour = min = sec = msec = 0;

        t.DecodeDate( &year, &month, &day );
        t.DecodeTime( &hour, &min, &sec, &msec );

        ensure_equals( 1900, year );
        ensure_equals(    1, month );
        ensure_equals(    1, day );
        ensure_equals(    0, hour );
        ensure_equals(    0, min );
        ensure_equals(    0, sec );
        ensure_equals(    0, msec );
    }

    template<>
    template<>
	void testUtil::test<3>()
    {
        set_test_name("trim");

        using namespace paulst;

        std::string s = "p";
        trim( s );
        ensure_equals( s, "p" );

        s = " p";
        trim( s );
        ensure_equals( s, "p" );


        s = "p ";
        trim( s );
        ensure_equals( s, "p" );


        s = "     and     ";
        trim( s );
        ensure_equals( s, "and" );

        s = "";
        trim( s );
        ensure_equals( s, "" );

        s = "     ";
        trim( s );
        ensure_equals( s, "" );

    }

    template<>
    template<>
	void testUtil::test<4>()
    {
        set_test_name("conditionBasedOutput( true, 1, \"2\", out)");

        using namespace paulst;

        std::ostringstream out;

        conditionBasedOutput( true, 1, "2", out );

        ensure_equals( out.str(), "1" );
    }

    template<>
    template<>
	void testUtil::test<5>()
    {
        set_test_name("conditionBasedOutput( false, 1, \"2\", out)");

        using namespace paulst;

        std::ostringstream out;

        conditionBasedOutput( false, 1, "2", out );

        ensure_equals( out.str(), "2" );
    }

    template<>
    template<>
	void testUtil::test<6>()
    {
        set_test_name("validateInt");

        using namespace paulst;

        std::map< std::string, bool > testData;
        testData.insert( std::make_pair( "1"                              , true  ) );
        testData.insert( std::make_pair( " 1"                             , true  ) );
        testData.insert( std::make_pair( "1 "                             , true  ) );
        testData.insert( std::make_pair( " 1 "                            , true  ) );
        testData.insert( std::make_pair( "1.2"                            , false ) );
        testData.insert( std::make_pair( "1234567890"                     , true  ) );
        testData.insert( std::make_pair( "a234567890"                     , false ) );
        testData.insert( std::make_pair( "123456789a"                     , false ) );
        testData.insert( std::make_pair( "1 a"                            , false ) );
        testData.insert( std::make_pair( "-1"                             , true  ) );
        testData.insert( std::make_pair( "  -1999928783 "                 , true  ) );
        testData.insert( std::make_pair( "  - 24 "                        , false ) );
        testData.insert( std::make_pair( "  -19999287834 "                , false  ) );// too big
        testData.insert( std::make_pair( "  -1.9 "                        , false  ) );


        for ( std::map< std::string, bool >::const_iterator i = testData.begin(); i != testData.end(); ++i )
        {
            bool result = validateInt( i->first );

            if ( i->second )
                ensure( i->first, result );
            else
                ensure_not( i->first, result );
        }
    }

    template<>
    template<>
	void testUtil::test<7>()
    {
        set_test_name("validateReal");

        using namespace paulst;

        std::map< std::string, bool > testData;
        testData.insert( std::make_pair( "1"                              , true  ) );
        testData.insert( std::make_pair( " 1"                             , true  ) );
        testData.insert( std::make_pair( "1 "                             , true  ) );
        testData.insert( std::make_pair( " 1 "                            , true  ) );
        testData.insert( std::make_pair( "1.2"                            , true  ) );
        testData.insert( std::make_pair( "1234567890"                     , true  ) );
        testData.insert( std::make_pair( "a234567890"                     , false ) );
        testData.insert( std::make_pair( "123456789a"                     , false ) );
        testData.insert( std::make_pair( "1 a"                            , false ) );
        testData.insert( std::make_pair( "-1"                             , true  ) );
        testData.insert( std::make_pair( "  -19999287 "                   , true  ) );
        testData.insert( std::make_pair( "  - 24 "                        , false ) );
        testData.insert( std::make_pair( "  -19999287834 "                , true  ) );
        testData.insert( std::make_pair( "  -1.9 "                        , true  ) );
        testData.insert( std::make_pair( "  -.9 "                         , true  ) );
        testData.insert( std::make_pair( "  -.9e "                        , false ) );
        testData.insert( std::make_pair( "  .9 "                          , true  ) );
        testData.insert( std::make_pair( "  .0 "                          , true  ) );
        testData.insert( std::make_pair( "  .  "                          , false ) );
        testData.insert( std::make_pair( "  -  "                          , false ) );


        for ( std::map< std::string, bool >::const_iterator i = testData.begin(); i != testData.end(); ++i )
        {
            bool result = validateReal( i->first );

            if ( i->second )
                ensure( i->first, result );
            else
                ensure_not( i->first, result );
        }
    }

    template<>
    template<>
	void testUtil::test<8>()
    {
        set_test_name("toString( DateTime, \"yyyy-mm-ddThh:mm:ss\" )");

        using namespace paulst;

        TDateTime dt( 2012, 2, 23, 1, 57, 30, 2 );

        ensure_equals( paulst::toString( dt, "yyyy-mm-dd'T'hh:nn:ss" ), std::string("2012-02-23T01:57:30") );
    }


};

#endif

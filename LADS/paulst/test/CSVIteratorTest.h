#ifndef CVSITERATORTEST_H
#define CVSITERATORTEST_H

#include <tut.h>

#include "CSVIterator.h"

namespace tut
{
    class CSVIteratorTestFixture
    {
    };

    typedef test_group<CSVIteratorTestFixture, 6> CSVIteratorTestGroup;
    CSVIteratorTestGroup testGroupCSVIterator(
                            "CSVIterator tests");
    typedef CSVIteratorTestGroup::object testCSVIterator;

    template<>
    template<>
    void testCSVIterator::test<1>()
    {
        set_test_name("empty sequence");
        using namespace paulst;
        CSVIterator<','> begin(""), end;
        ensure( begin == end );
        ensure( *(begin) == "" );
    }


    template<>
    template<>
    void testCSVIterator::test<2>()
    {
        set_test_name("single element, no trailing comma");
        using namespace paulst;
        CSVIterator<','> begin("piglet"), end;
        ensure( begin == end );
        ensure( *(begin) == "" );
    }

    template<>
    template<>
    void testCSVIterator::test<3>()
    {
        set_test_name("single element, trailing comma");
        using namespace paulst;
        CSVIterator<','> begin("piglet,"), end;
        ensure( begin != end );
        ensure( *(begin) == "piglet" );
        ++begin;
        ensure( begin == end );
    }

    template<>
    template<>
    void testCSVIterator::test<4>()
    {
        set_test_name("3 elements, with whitespace");
        using namespace paulst;
        CSVIterator<','> i(" a ,b  , c  ,"), end;
        ensure( i != end );
        ensure_equals( *i, "a" );
        ++i;
        ensure( *i++ == "b" );
        ensure( *i == "c" );
        ensure( ++i == end );
    }

    template<>
    template<>
    void testCSVIterator::test<5>()
    {
        set_test_name("3 empty elements");
        using namespace paulst;
        std::string s(",,,");
        CSVIterator<','> i(s), end;
        ensure( i != end );
        ensure( std::distance( i, end ) == 3 );
        for ( int x = 0; x < 3; ++x )
        {
            ensure( i != end );
            ensure( *i == "" );
            ++i;
        }
    }

    template<>
    template<>
    void testCSVIterator::test<6>()
    {
        set_test_name("newline as separator. Empty string equivalent to end-of-sequence? " );
        using namespace paulst;
        std::string s;
        CSVIterator<'\n'> i(s), end;
        ensure( i == end );
    }

};

#endif

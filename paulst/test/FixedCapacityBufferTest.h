#ifndef FIXEDCAPACITYBUFFERTESTH
#define FIXEDCAPACITYBUFFERTESTH

#include <tut.h>

#include <algorithm>
#include <cstring>
#include <iostream>
#include <iterator>
#include <string>

#include "FixedCapacityBuffer.h"
#include "StrUtil.h"

namespace tut
{
	class FixedCapacityBufferTestFixture
    {
    public:
        static std::string pushThroughBufferThenCopy( const std::string& s, int bufferCapacity )
        {
            paulst::FixedCapacityBuffer b( bufferCapacity );
            std::copy( s.begin(), s.end(), std::back_inserter(b) );
            return b.copy();
        }
    };

    typedef test_group<FixedCapacityBufferTestFixture, 11> FixedCapacityBufferTestGroup;
	FixedCapacityBufferTestGroup testGroupFixedCapacityBuffer( "FixedCapacityBuffer tests");
	typedef FixedCapacityBufferTestGroup::object testFixedCapacityBuffer;

    template<>
    template<>
	void testFixedCapacityBuffer::test<1>()
    {
        set_test_name("empty buffer");

        using namespace paulst;

        std::string copy = pushThroughBufferThenCopy( "", 5 );
        ensure ( last_line( copy.begin(), copy.end() ) == "" );
        ensure ( lastNonWhiteSpaceChar( copy.begin(), copy.end() ) == '\0' );
    }
    
    template<>
    template<>
	void testFixedCapacityBuffer::test<2>()
    {
        set_test_name("non-empty buffer, but no newline char");

        using namespace paulst;

        std::string copy = pushThroughBufferThenCopy( "sausages", 10 );
        ensure ( last_line( copy.begin(), copy.end() ) == "" );
        ensure ( lastNonWhiteSpaceChar( copy.begin(), copy.end() ) == 's' );
    }

    template<>
    template<>
	void testFixedCapacityBuffer::test<3>()
    {
        set_test_name("Single line in buffer");

        using namespace paulst;

        std::string b = pushThroughBufferThenCopy( "sausages\n", 10 );
        ensure ( last_line( b.begin(), b.end() ) == "sausages" );
        ensure ( lastNonWhiteSpaceChar( b.begin(), b.end() ) == 's' );
    }

    template<>
    template<>
	void testFixedCapacityBuffer::test<4>()
    {
        set_test_name("Single line in buffer, plus trailing chars");

        using namespace paulst;

        std::string b = pushThroughBufferThenCopy( "sausages\n and mash", 20 );
        ensure ( last_line( b.begin(), b.end() ) == "sausages" );
        ensure ( lastNonWhiteSpaceChar( b.begin(), b.end() ) == 'h' );
    }

    template<>
    template<>
	void testFixedCapacityBuffer::test<5>()
    {
        set_test_name("2 lines in buffer");

        using namespace paulst;

        std::string b = pushThroughBufferThenCopy( "sausages\n and mash\n", 20 );
        ensure ( last_line( b.begin(), b.end() ) == " and mash" );
        ensure ( lastNonWhiteSpaceChar( b.begin(), b.end() ) == 'h' );
    }

    template<>
    template<>
	void testFixedCapacityBuffer::test<6>()
    {
        set_test_name("2 lines in buffer, plus trailing chars");

        using namespace paulst;

        std::string b = pushThroughBufferThenCopy( "sausages\n and mash\n and\t", 30 );
        ensure ( last_line( b.begin(), b.end() ) == " and mash" );
        ensure ( lastNonWhiteSpaceChar( b.begin(), b.end() ) == 'd' );
    }

    template<>
    template<>
	void testFixedCapacityBuffer::test<7>()
    {
        set_test_name("appending more than will fit");

        using namespace paulst;

        std::string b = pushThroughBufferThenCopy( "sausages\n and mash\n", 3 );
        ensure ( last_line( b.begin(), b.end() ) == "sh" );
        ensure ( lastNonWhiteSpaceChar( b.begin(), b.end() ) == 'h' );
    }

    template<>
    template<>
    void testFixedCapacityBuffer::test<8>()
    {
        set_test_name("within buffer capacity");

        using namespace paulst;

        std::string b = pushThroughBufferThenCopy( "maximum", 10 );
        ensure_equals( std::strcmp( b.c_str(), "maximum" ), 0 );
    }
    
    template<>
    template<>
    void testFixedCapacityBuffer::test<9>()
    {
        set_test_name("at buffer capacity");

        using namespace paulst;

        std::string b = pushThroughBufferThenCopy( "maximum", 7 );
        ensure_equals( std::strcmp( b.c_str(), "maximum" ), 0 );
    }
    
    template<>
    template<>
    void testFixedCapacityBuffer::test<10>()
    {
        set_test_name("c_str() - buffer overflow");

        using namespace paulst;

        std::string b = pushThroughBufferThenCopy( "maximum", 6 );
        ensure_equals( std::strcmp( b.c_str(), "aximum" ), 0 );
    }
    
    template<>
    template<>
    void testFixedCapacityBuffer::test<11>()
    {
        set_test_name("The quick brown fox...");

        using namespace paulst;

        std::string b = pushThroughBufferThenCopy( "The quick brown fox jumped over the lazy dog.", 6 );
        ensure_equals( std::strcmp( b.c_str(), "y dog." ), 0 );
    }

};
 
#endif


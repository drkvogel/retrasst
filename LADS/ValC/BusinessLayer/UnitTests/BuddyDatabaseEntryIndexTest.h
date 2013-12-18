#ifndef BUDDYDATABASEENTRYINDEXTESTH
#define BUDDYDATABASEENTRYINDEXTESTH

#include "API.h"
#include "BuddyDatabaseEntryIndex.h"
#include <SysUtils.hpp>
#include <tut.h>

const int           BUDDY_SAMPLE_ID = 2;
const int           ALPHA_SAMPLE_ID = 3;
const std::string   BARCODE("BARCODE");
const std::string   DB_NAME("DB_NAME");
const int           RESULT_ID = 7;
const int           TEST_ID   = 9;

class CountingVisitor : public valc::BuddyDatabaseEntryIndex::Visitor
{
public:
    CountingVisitor() : m_counter(0) {}

    int count() const { return m_counter; }

    void visit( const valc::BuddyDatabaseEntry* bde, const valc::BuddyDatabaseEntryIndex::TestIDs& testsForWhichResults ) { ++m_counter; }

private:
    int m_counter;
};


class FindTest : public valc::BuddyDatabaseEntryIndex::Visitor
{
public:
    FindTest() : m_entry(0), m_test(0), m_found(false) {}
    bool found() const { return m_found; }
    void setTargetEntry( int buddySampleID ) { m_entry = buddySampleID; m_found = false; }
    void setTargetTest ( int testID        ) { m_test  = testID;        m_found = false; }
    void visit( const valc::BuddyDatabaseEntry* bde, const valc::BuddyDatabaseEntryIndex::TestIDs& testsForWhichResults ) 
    { 
        if ( bde->buddy_sample_id == m_entry )
        {
            m_found = testsForWhichResults.count( m_test );
        }
    }
private:
    bool m_found;
    int m_entry, m_test;
};

namespace tut
{
	class BuddyDatabaseEntryIndexTestFixture
    {
    };

    typedef test_group<BuddyDatabaseEntryIndexTestFixture, 5> BuddyDatabaseEntryIndexTestGroup;
	BuddyDatabaseEntryIndexTestGroup testGroupBuddyDatabaseEntryIndex( "BuddyDatabaseEntryIndex tests");
	typedef BuddyDatabaseEntryIndexTestGroup::object testBuddyDatabaseEntryIndex;

	template<>
	template<>
	void testBuddyDatabaseEntryIndex::test<1>()
	{
		set_test_name("Default constructed.");

		using namespace valc;

        BuddyDatabaseEntryIndex idx;
        
        ensure_equals( idx.lookupBuddySampleIDForResult( 1 ), 0 );

        CountingVisitor v;
        idx.accept( &v );

        ensure_equals( v.count(), 0 );

        bool exception = false;
        try
        {
            idx.get( 1 );
        }
        catch( ... )
        {
            exception = true;
        }

        ensure( exception );
	}

    template<>
	template<>
	void testBuddyDatabaseEntryIndex::test<2>()
	{
		set_test_name("Adding an entry.");

		using namespace valc;

        BuddyDatabaseEntryIndex idx;

        idx.add( BUDDY_SAMPLE_ID, ALPHA_SAMPLE_ID, BARCODE, DB_NAME, Now() );
        
        const BuddyDatabaseEntry* bde = idx.get(BUDDY_SAMPLE_ID);

        ensure( bde );
        ensure_equals( bde->barcode, BARCODE );

        CountingVisitor v;
        idx.accept( &v );

        ensure_equals( v.count(), 1 );

        bool exception = false;
        try
        {
            idx.get( BUDDY_SAMPLE_ID + 1 );
        }
        catch( ... )
        {
            exception = true;
        }

        ensure( exception );

	}

    template<>
	template<>
	void testBuddyDatabaseEntryIndex::test<3>()
	{
		set_test_name("test stuff.");

		using namespace valc;

        BuddyDatabaseEntryIndex idx;

        idx.add( BUDDY_SAMPLE_ID, ALPHA_SAMPLE_ID, BARCODE, DB_NAME, Now() );
        idx.supplementEntryWithResultInfo( BUDDY_SAMPLE_ID, RESULT_ID, TEST_ID );

        ensure_equals( idx.lookupBuddySampleIDForResult( RESULT_ID ), BUDDY_SAMPLE_ID );

        FindTest ft;
        ft.setTargetEntry( BUDDY_SAMPLE_ID );
        ft.setTargetTest( TEST_ID );
        idx.accept( &ft );
        ensure( ft.found() );

        ft.setTargetTest( TEST_ID + 1 );
        idx.accept( &ft );
        ensure_not( ft.found() );
    }

    template<>
	template<>
	void testBuddyDatabaseEntryIndex::test<4>()
	{
		set_test_name("Can't supplement if haven't added.");

		using namespace valc;

        BuddyDatabaseEntryIndex idx;

        idx.add( BUDDY_SAMPLE_ID, ALPHA_SAMPLE_ID, BARCODE, DB_NAME, Now() );

        bool exception = false;

        try
        {
            idx.supplementEntryWithResultInfo( BUDDY_SAMPLE_ID + 1, RESULT_ID, TEST_ID );
        }
        catch( ... )
        {
            exception = true;
        }

        ensure( exception ); 
    }

    template<>
	template<>
	void testBuddyDatabaseEntryIndex::test<5>()
	{
		set_test_name("More than one test for the same entry.");

		using namespace valc;

        BuddyDatabaseEntryIndex idx;

        idx.add( BUDDY_SAMPLE_ID, ALPHA_SAMPLE_ID, BARCODE, DB_NAME, Now() );

        idx.supplementEntryWithResultInfo( BUDDY_SAMPLE_ID, RESULT_ID    , TEST_ID );
        idx.supplementEntryWithResultInfo( BUDDY_SAMPLE_ID, RESULT_ID + 1, TEST_ID + 1 );

        FindTest ft;
        ft.setTargetEntry( BUDDY_SAMPLE_ID );
        ft.setTargetTest ( TEST_ID );
        ensure_not( ft.found() );
        idx.accept( &ft );
        ensure( ft.found() );

        ft.setTargetTest( TEST_ID + 1 );
        ensure_not( ft.found() );
        idx.accept( &ft );
        ensure( ft.found() );

        ft.setTargetTest( TEST_ID + 2 );
        ensure_not( ft.found() );
        idx.accept( &ft );
        ensure_not( ft.found() );
    }

};

#endif


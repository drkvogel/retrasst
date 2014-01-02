#ifndef WORKLISTRELATIVETESTH
#define WORKLISTRELATIVETESTH

#include "API.h"
#include "StrUtil.h"
#include "WorklistEntries.h"
#include "WorklistEntryImpl.h"
#include "WorklistLinks.h"
#include "WorklistRelativeImpl.h"
#include <tut.h>

namespace tut
{
	struct WorklistRelativeTestFixture
    {
        valc::WorklistEntries           worklistDirectory;
        valc::WorklistLinks             worklistLinks;
        valc::WorklistRelative::Impl    worklistRelativeImpl;
        
        WorklistRelativeTestFixture() 
            : 
            worklistLinks(&worklistDirectory) ,
            worklistRelativeImpl(&worklistLinks)
        {
        }

        void addWorklistEntry( int id, int sampleID, int projectID )
        {
            worklistDirectory.add( new valc::WorklistEntryImpl(
            id,// id
            paulst::format( "%d/%d", sampleID, projectID ),// sampleDescriptor
            -1234, //machineID,
            paulst::format( "barcode%d/%d", sampleID, projectID ),
            -101, //testID,
            0, //groupID
            0, //categoryID
            sampleID,
            projectID,
            1, //profileID
            "profileName",
            Now(),
            1,//tsSequence
            'C',//status,
            0.0,//diluent
            0, //buddyResultID
            0 //resultDirectory
                ) );
        }
    };

    typedef test_group<WorklistRelativeTestFixture, 5> WorklistRelativeTestGroup;
	WorklistRelativeTestGroup testGroupWorklistRelative( "WorklistRelative tests");
	typedef WorklistRelativeTestGroup::object testWorklistRelative;

	template<>
	template<>
	void testWorklistRelative::test<1>()
	{
		set_test_name("WorklistRelative behaviour demonstration");

		using namespace valc;

        addWorklistEntry( 1, 2, 3 );

        WorklistRelative wr = worklistRelativeImpl.wrap( worklistDirectory.get(1) );

        ensure          ( wr.isBoundToWorklistEntryInstance() );
        ensure_not      ( wr.hasChildren() );
        ensure_not      ( wr.hasParent() );
        ensure_equals   ( wr.getRelation(), '\0' );
        ensure_equals   ( wr.getID(), 1 );
        ensure_equals   ( wr->getID(), 1 );

        worklistLinks.addLink( 1, 9, 'r' );

        ensure( wr.hasChildren() );

        std::vector<WorklistRelative> children = wr.getChildren();

        ensure_equals( children.size(), 1U );

        WorklistRelative child = children.at(0);

        ensure_equals   ( child.getID(), 9 );
        ensure_not      ( child.isBoundToWorklistEntryInstance() );
        ensure_not      ( child.hasChildren() );
        ensure          ( child.hasParent() );
        ensure_equals   ( child.getParent().getID(), 1 );

        addWorklistEntry( 9, 2, 3 );

        ensure_not( child.isBoundToWorklistEntryInstance() );
        children = wr.getChildren();
        child = children.at(0);
        ensure    ( child.isBoundToWorklistEntryInstance() );

        addWorklistEntry( 11, 2, 3 );

        worklistLinks.addLink( 11, 1, 'r' );

        ensure_equals   ( wr.getID(), 1 );
        ensure          ( wr.hasParent() );
        ensure_equals   ( wr.getRelation(), '\0' ); // Stale!
        ensure_equals   ( wr.getParent().getID(), 11 );

        children = wr.getParent().getChildren();
        wr = children.at(0);

        ensure_equals   ( wr.getID(), 1 );
        ensure          ( wr.hasParent() );
        ensure_equals   ( wr.getRelation(), 'r' );
    }

    template<>
	template<>
	void testWorklistRelative::test<2>()
	{
		set_test_name("Trying to use -> when not bound to a worklist entry");

		using namespace valc;

        addWorklistEntry( 1, 2, 3 );

        WorklistRelative wr = worklistRelativeImpl.wrap( worklistDirectory.get(1) );
        worklistLinks.addLink( 1, 9, 'r' );

        std::vector<WorklistRelative> children = wr.getChildren();

        WorklistRelative child = children.at(0);

        bool exceptionThrown = false;

        try
        {
            child->getBarcode();
        }
        catch( const Exception& e )
        {
            exceptionThrown = true;
        }

        ensure( exceptionThrown );
    }

    template<>
	template<>
	void testWorklistRelative::test<3>()
	{
		set_test_name("Trying to use getChildren when no children");

		using namespace valc;

        addWorklistEntry( 1, 2, 3 );

        WorklistRelative wr = worklistRelativeImpl.wrap( worklistDirectory.get(1) );

        bool exceptionThrown = false;

        try
        {
            wr.getChildren();
        }
        catch( const Exception& e )
        {
            exceptionThrown = true;
        }

        ensure( exceptionThrown );
    }


    template<>
	template<>
	void testWorklistRelative::test<4>()
	{
		set_test_name("Trying to use getParent when no parent");

		using namespace valc;

        addWorklistEntry( 1, 2, 3 );

        WorklistRelative wr = worklistRelativeImpl.wrap( worklistDirectory.get(1) );

        bool exceptionThrown = false;

        try
        {
            wr.getParent();
        }
        catch( const Exception& e )
        {
            exceptionThrown = true;
        }

        ensure( exceptionThrown );
    }

    template<>
	template<>
	void testWorklistRelative::test<5>()
	{
		set_test_name("Trying to link to self");

		using namespace valc;

        bool exceptionThrown = false;

        try
        {
            worklistLinks.addLink( 29, 29, 'r' );
        }
        catch( const Exception& e )
        {
            exceptionThrown = true;
        }

        ensure( exceptionThrown );
    }


};

#endif


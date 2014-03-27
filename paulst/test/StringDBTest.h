#ifndef STRINGDBTEST_H
#define STRINGDBTEST_H

#include <tut.h>

#include "StringDB.h"

namespace tut
{
	class StringDBTestFixture
    {
    };

    typedef test_group<StringDBTestFixture, 5> StringDBTestGroup;
	StringDBTestGroup testGroupStringDB(
							"StringDB tests");
	typedef StringDBTestGroup::object testStringDB;

    template<>
    template<>
	void testStringDB::test<1>()
    {
        using namespace paulst;

        set_test_name("editing an existing attribute value");

        std::string before(
"run=29,basket=32,creator=Fred,priority=5,selector=1,runType=0,doneStatus=100,auth=paulst,\n"
"run=42,basket=79,creator=Paul,priority=2,selector=2,runType=1,doneStatus=10,blood=1,auth=paulst,basketNote=Check me out!,\n"
"run=88,basket=21,creator=Gail,priority=9,selector=2,runType=0,doneStatus=10,auth=paulst,basketNote=Check me out!,\n");

        StringDB db(before, "run");

        db.updateAttribute( "42", "selector", "5" );

        std::string after = db.getData();

        std::string expected(
"run=29,basket=32,creator=Fred,priority=5,selector=1,runType=0,doneStatus=100,auth=paulst,\n"
"run=88,basket=21,creator=Gail,priority=9,selector=2,runType=0,doneStatus=10,auth=paulst,basketNote=Check me out!,\n"
"run=42,basket=79,creator=Paul,priority=2,runType=1,doneStatus=10,blood=1,auth=paulst,basketNote=Check me out!,selector=5,\n"
);

        ensure_equals( after, expected );

        db.updateAttribute( "42", "doneStatus", "100" );
        expected = 
"run=29,basket=32,creator=Fred,priority=5,selector=1,runType=0,doneStatus=100,auth=paulst,\n"
"run=88,basket=21,creator=Gail,priority=9,selector=2,runType=0,doneStatus=10,auth=paulst,basketNote=Check me out!,\n"
"run=42,basket=79,creator=Paul,priority=2,runType=1,blood=1,auth=paulst,basketNote=Check me out!,selector=5,doneStatus=100,\n";

        ensure_equals( db.getData(), expected );
    }

    template<>
    template<>
	void testStringDB::test<2>()
    {
        set_test_name("Cursor: empty dataset");

        using namespace paulst;

        StringDB db("", "run");
        
        std::auto_ptr<LineByLineCursor> c( db.newCursor() );
        ensure( c->exhausted() );
    }

    template<>
    template<>
	void testStringDB::test<3>()
    {
        set_test_name("Cursor: single row");

        using namespace paulst;

        StringDB db("thing=1,name=Badger,\n", "thing");
        
        std::auto_ptr<LineByLineCursor> c( db.newCursor() );
        ensure_not( c->exhausted() );
        ensure_equals( c->get("thing"), std::string("1")      );
        ensure_equals( c->get("name") , std::string("Badger") );
        ensure_not( c->next() );
        ensure( c->exhausted() );
    }

    template<>
    template<>
	void testStringDB::test<4>()
    {
        set_test_name("Cursor: multi row");

        using namespace paulst;

        StringDB db(
            "thing=1,name=Badger,\n"
            "thing=3,name=Midge,\n"
            "thing=9,name=Heron,\n"
            "thing=2,name=Beagle,\n"
            , "thing");
        
        std::auto_ptr<LineByLineCursor> c( db.newCursor() );
        ensure_not( c->exhausted() );
        ensure_equals( c->get("thing"), std::string("1")      );
        ensure_equals( c->get("name") , std::string("Badger") );
        ensure( c->next() );
        ensure_equals( c->get("thing"), std::string("3")      );
        ensure_equals( c->get("name") , std::string("Midge") );
        ensure( c->next() );
        ensure_equals( c->get("thing"), std::string("9")      );
        ensure_equals( c->get("name") , std::string("Heron") );
        ensure( c->next() );
        ensure_equals( c->get("thing"), std::string("2")      );
        ensure_equals( c->get("name") , std::string("Beagle") );
        ensure_not( c->next() );
    }

    template<>
    template<>
	void testStringDB::test<5>()
    {
        set_test_name("Cursor: empty DB");

        using namespace paulst;

        StringDB db;
        
        std::auto_ptr<LineByLineCursor> c( db.newCursor() );
        ensure( c->exhausted() );
    }

};

#endif

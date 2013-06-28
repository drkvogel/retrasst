#ifndef FORCERELOAD_TEST_H 
#define FORCERELOAD_TEST_H

#include <algorithm>
#include "API.h"
#include <boost/scoped_ptr.hpp>
#include <boost/variant/get.hpp>
#include "MockConnectionFactory.h"
#include <tut.h>

/*
These tests use MockConnectionFactory.
*/
namespace tut
{
	class ForceReloadTestFixture
    {
    };

    typedef test_group<ForceReloadTestFixture, 10> ForceReloadTestGroup;
	ForceReloadTestGroup testGroupForceReload( "ForceReload tests");
	typedef ForceReloadTestGroup::object testForceReload;

    template<>
    template<>
	void testForceReload::test<1>()
    {
        set_test_name("ForceReload - no data!");

        using namespace valc;

        MockConnectionFactory connectionFactory;

        boost::scoped_ptr<valc::DBConnection> connection( connectionFactory.createConnection() );
        
        boost::scoped_ptr<valc::AnalysisActivitySnapshot> s( valc::SnapshotFactory::load( -1019349, 1234, connection.get() ) );

        ensure( s.get() );
        ensure( s->localBegin() == s->localEnd() );
        ensure( s->queueBegin() == s->queueEnd() );
    }

    template<>
    template<>
	void testForceReload::test<2>()
    {
        set_test_name("ForceReload - 3 tests queued, single sample.");

        using namespace valc;

        MockConnectionFactory connectionFactory;

        connectionFactory.setClusters( "-1019430,\n" );
        connectionFactory.setProjects( "-832455,reveal,ldb25,\n" );
        connectionFactory.setWorklist(
//rec  machine  barcode   test     group     c sample project p prof                  timestamp           seq s dil   result
"24110,-1019430,118502164,-1031390,-12744865,0,417128,-832455,0,EDTA_1 analysis other,21-06-2013 11:07:57,39,Q,0.000,0,\n"
"24111,-1019430,118502164,-1031389,-12744865,0,417128,-832455,0,EDTA_1 analysis other,21-06-2013 11:07:57,34,Q,0.000,0,\n"
"24112,-1019430,118502164,-1031388,-12744865,0,417128,-832455,0,EDTA_1 analysis other,21-06-2013 11:07:57,35,Q,0.000,0,\n"
            );

        boost::scoped_ptr<valc::DBConnection> connection( connectionFactory.createConnection() );
        
        boost::scoped_ptr<valc::AnalysisActivitySnapshot> s( valc::SnapshotFactory::load( -1019349, 1234, connection.get() ) );

        ensure( s.get() );
        ensure( s->localBegin() == s->localEnd() );
        ensure_equals( std::distance( s->queueBegin(), s->queueEnd() ), 1 );

        QueuedSample qs = *(s->queueBegin());

        ensure_equals( qs.getSampleDescriptor(), "417128/-832455" );

        SampleWorklistEntries wles = s->getWorklistEntries( qs.getSampleDescriptor() );

        ensure_equals( wles.size(), 3 );
    }

    template<>
    template<>
	void testForceReload::test<3>()
    {
        set_test_name("ForceReload - 2 samples, one with queued only, other with some results.");

        using namespace valc;

        MockConnectionFactory connectionFactory;

        connectionFactory.setClusters( "-1019430,\n" );
        connectionFactory.setProjects( "-832455,reveal,ldb25,\n" );
        connectionFactory.setWorklist(
//rec  machine  barcode   test     group     c sample project p prof                  timestamp           seq s dil   result
"-36832,-1019430,118507097,-1031384,-12750391,0,195199,-832455,0,EDTA_1 analysis other,27-06-2013 10:56:14,11,Q,0.000,0,\n"
"-36831,-1019430,118507097,-1031385,-12750391,0,195199,-832455,0,EDTA_1 analysis other,27-06-2013 10:56:14,19,Q,0.000,0,\n"
"-36830,-1019430,118507097,-1031386,-12750391,0,195199,-832455,0,EDTA_1 analysis other,27-06-2013 10:56:14,10,Q,0.000,0,\n"
"-36829,-1019430,118507097,-1031388,-12750391,0,195199,-832455,0,EDTA_1 analysis other,27-06-2013 10:56:14,12,Q,0.000,0,\n"
"-36828,-1019430,118507097,-1031389,-12750391,0,195199,-832455,0,EDTA_1 analysis other,27-06-2013 10:56:14,13,Q,0.000,0,\n"
"-36827,-1019430,118507097,-1031390,-12750391,0,195199,-832455,0,EDTA_1 analysis other,27-06-2013 10:56:14,14,Q,0.000,0,\n"
"-36845,-1019430,118507091,-1031390,-12750394,0,432560,-832455,0,EDTA_1 analysis other,27-06-2013 10:57:49,14,C,0.000,882431,\n"
"-36847,-1019430,118507091,-1031388,-12750394,0,432560,-832455,0,EDTA_1 analysis other,27-06-2013 10:57:49,12,C,0.000,882429,\n"
"-36846,-1019430,118507091,-1031389,-12750394,0,432560,-832455,0,EDTA_1 analysis other,27-06-2013 10:57:49,13,C,0.000,882430,\n"
"-36848,-1019430,118507091,-1031386,-12750394,0,432560,-832455,0,EDTA_1 analysis other,27-06-2013 10:57:49,10,C,0.000,882427,\n"
// Note that the last 2 have zero for buddy_result_id
// 
// the buddy_sample_id for buddy_result_id 882429 is: 882290
            );
        connectionFactory.setBuddyDB(
//bsid ,barcode  ,date analysed      ,dbname,sample,machine ,res id,test id ,result ,a,date analysed      ,restx,update when        ,
"882290,118507091,27-06-2013 11:42:36,REVEAL,432560,-1019349,882431,-1031390,1.850  ,0,27-06-2013 11:57:47,1.85 ,27-06-2013 10:57:49,0,,,,,,\n"
"882290,118507091,27-06-2013 11:42:36,REVEAL,432560,-1019349,882429,-1031388,0.960  ,0,27-06-2013 11:57:47,0.96 ,27-06-2013 10:57:49,0,,,,,,\n"
"882290,118507091,27-06-2013 11:42:36,REVEAL,432560,-1019349,882427,-1031386,57.100 ,0,27-06-2013 11:57:47,57.1 ,27-06-2013 10:57:49,0,,,,,,\n"
"882290,118507091,27-06-2013 11:42:36,REVEAL,432560,-1019349,882430,-1031389,2.360  ,0,27-06-2013 11:57:47,2.36 ,27-06-2013 10:57:49,0,,,,,,\n"
            );


        boost::scoped_ptr<valc::DBConnection> connection( connectionFactory.createConnection() );
        
        boost::scoped_ptr<valc::AnalysisActivitySnapshot> s( valc::SnapshotFactory::load( -1019349, 1234, connection.get() ) );

        ensure( s.get() );
        ensure_equals( std::distance( s->queueBegin(), s->queueEnd() ), 1 );
        ensure_equals( std::distance( s->localBegin(), s->localEnd() ), 1 );
        
        LocalEntry localEntry = *(s->localBegin());

        LocalRun lr = boost::get<LocalRun>(localEntry);
    }

};

#endif


#ifndef QUEUEDSAMPLESBUILDERFUNCTION_TEST_H 
#define QUEUEDSAMPLESBUILDERFUNCTION_TEST_H

#include "API.h"
#include <boost/shared_ptr.hpp>
#include "ClusterIDs.h"
#include "QueuedSamplesBuilderFunction.h"
#include "StringBuilder.h"
#include "WorklistEntries.h"
#include "WorklistEntryImpl.h"
#include <tut.h>
#include <vector>

const int DLDL = -246;
const int CREA = -23;
const int CHOL = -24;


class MockParams : public valc::QueuedSamplesBuilderFunction::Params
{
private:
    unsigned int                m_currentWorklistEntry;
    int                         m_localMachineID;
    valc::ClusterIDs            m_clusterIDs;
    valc::WorklistEntries       m_worklistEntries;
    std::vector<std::string>    m_sampleDescriptorsWithOpenRuns;

public:
    MockParams() : m_currentWorklistEntry(0) { }

    void addCluster( int clusterID )
    {
        m_clusterIDs.insert( clusterID );
    }

    void addOpenRunFor( const std::string& sampleDescriptor )
    {
        m_sampleDescriptorsWithOpenRuns.push_back( sampleDescriptor );
    }

    void addWorklistEntry( int sampleID, int projectID, int machineID, char status, int testID, TDateTime timestamp  )
    {
        m_worklistEntries.add( new valc::WorklistEntryImpl(
            m_worklistEntries.size() + 1,// id
            std::string() << sampleID << "/" << projectID,// sampleDescriptor
            machineID,
            std::string("barcode") << sampleID << "/" << projectID,
            testID,
            0, //groupID
            0, //categoryID
            sampleID,
            projectID,
            1, //profileID
            "profileName",
            timestamp,
            1,//tsSequence
            status,
            0.0,//diluent
            0, //buddyResultID
            &m_worklistEntries,
            0 //resultDirectory
            )
         );
    }

    bool existsOpenRunForSample( const std::string& sampleDescriptor )   
    { 
        return m_sampleDescriptorsWithOpenRuns.end() != std::find( 
            m_sampleDescriptorsWithOpenRuns.begin(), m_sampleDescriptorsWithOpenRuns.end(), sampleDescriptor );
    }

    void forEachWorklistEntry( valc::QueuedSamplesBuilderFunction* f )
    {
        valc::WorklistDirectoryFunctionAdapter func(f);
        m_worklistEntries.forEach( func );
    }

    bool isLocalOrCluster( int machineID )                               
    { 
        return ( machineID == m_localMachineID ) || m_clusterIDs.count(machineID) ; 
    }

    void setLocalMachineID( int id )
    {
        m_localMachineID = id;
    }

};

namespace tut
{
	class QueuedSamplesBuilderFunctionTestFixture
    {
    };

    typedef test_group<QueuedSamplesBuilderFunctionTestFixture, 8> QueuedSamplesBuilderFunctionTestGroup;
	QueuedSamplesBuilderFunctionTestGroup testGroupQueuedSamplesBuilderFunction( "QueuedSamplesBuilderFunction tests");
	typedef QueuedSamplesBuilderFunctionTestGroup::object testQueuedSamplesBuilderFunction;

    template<>
    template<>
	void testQueuedSamplesBuilderFunction::test<1>()
    {
        set_test_name("Queued for the local machine");

        using namespace valc;

        MockParams* params = new MockParams();
        params->setLocalMachineID( 54 );
        params->addCluster( 79 );
        params->addWorklistEntry( 901, 701 ,54, 'Q', DLDL, TDateTime(2013, 5, 21, 13, 59, 0, 0) );
        QueuedSamplesBuilderFunction buildQueue(params);
        QueuedSamples out;
        buildQueue( &out );

        ensure_equals( out.size(), 1U );
    }

    template<>
    template<>
	void testQueuedSamplesBuilderFunction::test<2>()
    {
        set_test_name("testing deduplication");

        using namespace valc;

        const int SAMPLE        = 901;
        const int PROJECT       = 701;
        const int LOCAL_MACHINE = 54;

        MockParams* params = new MockParams();
        params->setLocalMachineID( LOCAL_MACHINE );
        params->addCluster( 79 );
        params->addWorklistEntry( SAMPLE, PROJECT, LOCAL_MACHINE, 'Q', DLDL, TDateTime(2013, 5, 21, 13, 59, 0, 0) );
        params->addWorklistEntry( SAMPLE, PROJECT, LOCAL_MACHINE, 'Q', CREA, TDateTime(2013, 5, 21, 13, 59, 0, 0) );
        QueuedSamplesBuilderFunction buildQueue(params);
        QueuedSamples out;
        buildQueue( &out );

        ensure_equals( out.size(), 1U );
    }

    template<>
    template<>
	void testQueuedSamplesBuilderFunction::test<3>()
    {
        set_test_name("Business Rule 14.1: (P,C,X) before (Q,S,W)");

        using namespace valc;

        const int LOCAL_MACHINE = 54;
        const int CLUSTER_MACHINE = 79;
        const int PROJECT = 701;

        MockParams* params = new MockParams();
        params->setLocalMachineID( LOCAL_MACHINE );
        params->addCluster( CLUSTER_MACHINE );
        params->addWorklistEntry( 901, PROJECT, LOCAL_MACHINE,   'Q', DLDL, TDateTime(2013, 5, 21, 13, 59, 0, 0) );
        params->addWorklistEntry( 902, PROJECT, CLUSTER_MACHINE, 'Q', CREA, TDateTime(2013, 5, 21, 13, 59, 0, 0) );
        params->addWorklistEntry( 902, PROJECT, CLUSTER_MACHINE, 'Q', CHOL, TDateTime(2013, 5, 21, 13, 59, 0, 0) );
        params->addWorklistEntry( 904, PROJECT, CLUSTER_MACHINE, 'C', CREA, TDateTime(2013, 5, 21, 13, 59, 0, 0) );
        params->addWorklistEntry( 904, PROJECT, CLUSTER_MACHINE, 'Q', CHOL, TDateTime(2013, 5, 21, 13, 59, 0, 0) );
        QueuedSamplesBuilderFunction buildQueue(params);
        QueuedSamples out;
        buildQueue( &out );

        ensure_equals( out.size(), 3U );
        ensure_equals( out[0].getSampleDescriptor(), std::string() << 904 << "/" << PROJECT );
    }

    template<>
    template<>
	void testQueuedSamplesBuilderFunction::test<4>()
    {
        set_test_name("Business Rule 18.1: Got to have a Q or S to be listed");

        using namespace valc;

        const int LOCAL_MACHINE = 54;
        const int CLUSTER_MACHINE = 79;
        const int PROJECT = 701;

        MockParams* params = new MockParams();
        params->setLocalMachineID( LOCAL_MACHINE );
        params->addCluster( CLUSTER_MACHINE );
        params->addWorklistEntry( 901, PROJECT, CLUSTER_MACHINE, 'C', DLDL, TDateTime(2013, 5, 21, 13, 59, 0, 0) );
        params->addWorklistEntry( 901, PROJECT, CLUSTER_MACHINE, 'P', CHOL, TDateTime(2013, 5, 21, 13, 59, 0, 0) );
        params->addWorklistEntry( 902, PROJECT, CLUSTER_MACHINE, 'C', DLDL, TDateTime(2013, 5, 21, 13, 59, 0, 0) );
        params->addWorklistEntry( 902, PROJECT, CLUSTER_MACHINE, 'Q', CHOL, TDateTime(2013, 5, 21, 13, 59, 0, 0) );
        QueuedSamplesBuilderFunction buildQueue(params);
        QueuedSamples out;
        buildQueue( &out );

        ensure_equals( out.size(), 1U );
        ensure_equals( out[0].getSampleDescriptor(), std::string() << 902 << "/" << PROJECT );
    }

    template<>
    template<>
	void testQueuedSamplesBuilderFunction::test<5>()
    {
        set_test_name("Business Rule 14.2: (P,C,X) listed most recent timestamp first");

        using namespace valc;

        const int LOCAL_MACHINE = 54;
        const int CLUSTER_MACHINE = 79;
        const int PROJECT = 701;
        TDateTime MIDDAY(2013, 5, 21, 12,  0, 0, 0);
        TDateTime ONE_PM(2013, 5, 21, 13,  0, 0, 0);
        TDateTime TWO_PM(2013, 5, 21, 14,  0, 0, 0);
        TDateTime SIX_PM(2013, 5, 21, 18,  0, 0, 0);
        TDateTime TEN_PM(2013, 5, 21, 22,  0, 0, 0);

        MockParams* params = new MockParams();
        params->setLocalMachineID( LOCAL_MACHINE );
        params->addCluster( CLUSTER_MACHINE );
        params->addWorklistEntry( 901, PROJECT, LOCAL_MACHINE, 'C', DLDL,           SIX_PM );
        params->addWorklistEntry( 901, PROJECT, LOCAL_MACHINE, 'Q', CHOL, MIDDAY );
        params->addWorklistEntry( 902, PROJECT, LOCAL_MACHINE, 'C', DLDL,           ONE_PM );
        params->addWorklistEntry( 902, PROJECT, LOCAL_MACHINE, 'Q', CHOL, MIDDAY );
        params->addWorklistEntry( 903, PROJECT, LOCAL_MACHINE, 'C', DLDL,           TEN_PM );
        params->addWorklistEntry( 903, PROJECT, LOCAL_MACHINE, 'Q', CHOL, MIDDAY );
        params->addWorklistEntry( 904, PROJECT, LOCAL_MACHINE, 'C', DLDL,           TWO_PM );
        params->addWorklistEntry( 904, PROJECT, LOCAL_MACHINE, 'Q', CHOL, MIDDAY );
        QueuedSamplesBuilderFunction buildQueue(params);
        QueuedSamples out;
        buildQueue( &out );

        ensure_equals( out.size(), 4U );

        std::string actualSequence;

        for( QueuedSampleIterator i = out.begin(); i != out.end(); ++i )
        {
            actualSequence += i->getSampleDescriptor() + ",";
        }

        ensure_equals( actualSequence, "903/701,901/701,904/701,902/701," );
    }

    template<>
    template<>
	void testQueuedSamplesBuilderFunction::test<6>()
    {
        set_test_name("Business Rule 14.3: (Q,S,W) listed oldest timestamp first");

        using namespace valc;

        const int LOCAL_MACHINE = 54;
        const int CLUSTER_MACHINE = 79;
        const int PROJECT = 701;
        TDateTime MIDDAY(2013, 5, 21, 12,  0, 0, 0);
        TDateTime ONE_PM(2013, 5, 21, 13,  0, 0, 0);
        TDateTime TWO_PM(2013, 5, 21, 14,  0, 0, 0);
        TDateTime SIX_PM(2013, 5, 21, 18,  0, 0, 0);
        TDateTime TEN_PM(2013, 5, 21, 22,  0, 0, 0);

        MockParams* params = new MockParams();
        params->setLocalMachineID( LOCAL_MACHINE );
        params->addCluster( CLUSTER_MACHINE );
        params->addWorklistEntry( 901, PROJECT, LOCAL_MACHINE, 'Q', CHOL, TWO_PM );
        params->addWorklistEntry( 902, PROJECT, LOCAL_MACHINE, 'Q', CHOL, TEN_PM );
        params->addWorklistEntry( 903, PROJECT, LOCAL_MACHINE, 'Q', CHOL, ONE_PM );
        params->addWorklistEntry( 904, PROJECT, LOCAL_MACHINE, 'Q', CHOL, SIX_PM );
        QueuedSamplesBuilderFunction buildQueue(params);
        QueuedSamples out;
        buildQueue( &out );

        ensure_equals( out.size(), 4U );

        std::string actualSequence;

        for( QueuedSampleIterator i = out.begin(); i != out.end(); ++i )
        {
            actualSequence += i->getSampleDescriptor() + ",";
        }

        ensure_equals( actualSequence, "903/701,901/701,904/701,902/701," );
    }

    template<>
    template<>
	void testQueuedSamplesBuilderFunction::test<7>()
    {
        set_test_name("Business Rule 18.2: existence of an open sample run precludes listing");

        using namespace valc;

        const int LOCAL_MACHINE = 54;
        const int CLUSTER_MACHINE = 79;

        MockParams* params = new MockParams();
        params->setLocalMachineID( LOCAL_MACHINE );
        params->addCluster( CLUSTER_MACHINE );
        params->addOpenRunFor( "901/701" );
        params->addWorklistEntry( 901, 701, LOCAL_MACHINE, 'Q', CHOL, TDateTime(2013, 5, 21, 12,  0, 0, 0) );
        QueuedSamplesBuilderFunction buildQueue(params);
        QueuedSamples out;
        buildQueue( &out );

        ensure_equals( out.size(), 0U );
    }

    template<>
    template<>
	void testQueuedSamplesBuilderFunction::test<8>()
    {
        set_test_name("Putting it all together");

        using namespace valc;

        const int LOCAL_MACHINE = 54;
        const int CLUSTER_MACHINE = 79;
        const int PROJECT = 701;
        const TDateTime MIDDAY(2013, 5, 21, 12,  0, 0, 0);
        const TDateTime ONE_PM(2013, 5, 21, 13,  0, 0, 0);
        const TDateTime TWO_PM(2013, 5, 21, 14,  0, 0, 0);
        const TDateTime SIX_PM(2013, 5, 21, 18,  0, 0, 0);
        const TDateTime TEN_PM(2013, 5, 21, 22,  0, 0, 0);

        MockParams* params = new MockParams();
        params->setLocalMachineID( LOCAL_MACHINE );
        params->addCluster( CLUSTER_MACHINE );

        // Q-only entries 
        params->addWorklistEntry( 901, 701, LOCAL_MACHINE, 'Q', CHOL, MIDDAY );
        params->addWorklistEntry( 901, 701, LOCAL_MACHINE, 'Q', DLDL, MIDDAY );
        params->addWorklistEntry( 902, 701, LOCAL_MACHINE, 'Q', CHOL, TEN_PM );
        params->addWorklistEntry( 903, 701, LOCAL_MACHINE, 'Q', CHOL, SIX_PM );

        // Done-and-dusted
        params->addWorklistEntry( 904, 701, CLUSTER_MACHINE, 'C', CHOL, SIX_PM );
        params->addWorklistEntry( 905, 701, CLUSTER_MACHINE, 'C', CHOL, SIX_PM );
        params->addWorklistEntry( 905, 701, CLUSTER_MACHINE, 'C', DLDL, SIX_PM );

        // Mix of C, P and Q
        params->addWorklistEntry( 906, 701, CLUSTER_MACHINE, 'C', DLDL, TWO_PM );
        params->addWorklistEntry( 906, 701, CLUSTER_MACHINE, 'P', CHOL, TWO_PM );
        params->addWorklistEntry( 906, 701, CLUSTER_MACHINE, 'Q', CREA, TWO_PM );
        params->addWorklistEntry( 907, 701, CLUSTER_MACHINE, 'C', DLDL, ONE_PM );
        params->addWorklistEntry( 907, 701, CLUSTER_MACHINE, 'P', CHOL, ONE_PM );
        params->addWorklistEntry( 907, 701, CLUSTER_MACHINE, 'Q', CREA, ONE_PM );
        params->addWorklistEntry( 911, 701, CLUSTER_MACHINE, 'C', DLDL, TEN_PM );
        params->addWorklistEntry( 911, 701, CLUSTER_MACHINE, 'P', CHOL, TEN_PM );
        params->addWorklistEntry( 911, 701, CLUSTER_MACHINE, 'Q', CREA, TEN_PM );

        // Mix of C, P and Q, with on-going local analysis activity
        params->addWorklistEntry( 908, 701, CLUSTER_MACHINE, 'C', DLDL, TWO_PM );
        params->addWorklistEntry( 908, 701, CLUSTER_MACHINE, 'P', CHOL, ONE_PM );
        params->addWorklistEntry( 908, 701, CLUSTER_MACHINE, 'Q', CREA, MIDDAY );
        params->addWorklistEntry( 909, 701, CLUSTER_MACHINE, 'C', DLDL, TWO_PM );
        params->addWorklistEntry( 909, 701, CLUSTER_MACHINE, 'P', CHOL, ONE_PM );
        params->addWorklistEntry( 909, 701, CLUSTER_MACHINE, 'Q', CREA, MIDDAY );
        params->addOpenRunFor( "908/701" );
        params->addOpenRunFor( "909/701" );

        // In-process (P) but local machine not involved 
        params->addWorklistEntry( 910, 701, CLUSTER_MACHINE, 'P', CHOL, ONE_PM );

        // So, what do we expect? The following should be in the queue:
        // 901, 902, 903, 906, 907, 911
        // In what order?
        // (P,C,X) before (Q,S,W)-only, so
        // (906,907,911) before (901, 902, 903)
        // Latest 1st for (P,C,X), so: (911, 906, 907)
        // Earliest 1st for (Q,S,W)-only, so: (901,903,902)
        // So: 911,906,907,901,903,902

        QueuedSamplesBuilderFunction buildQueue(params);
        QueuedSamples out;
        buildQueue( &out );

        ensure_equals( out.size(), 6U );

        std::string actualSequence;

        for( QueuedSampleIterator i = out.begin(); i != out.end(); ++i )
        {
            actualSequence += i->getSampleDescriptor() + ",";
        }

        ensure_equals( actualSequence, "911/701,906/701,907/701,901/701,903/701,902/701," );
    }

};

#endif


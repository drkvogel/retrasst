#ifndef QUEUEDSAMPLESBUILDERFUNCTIONH
#define QUEUEDSAMPLESBUILDERFUNCTIONH

#include "API.h"
#include <boost/scoped_ptr.hpp>
#include <string>
#include <System.hpp>
#include "WorklistDirectory.h"

namespace valc
{

struct QueueTicket
{
    bool onlyQSW;
    TDateTime timestamp;

    QueueTicket();
    QueueTicket( const QueueTicket& );
    QueueTicket( bool hasOnlyQSWEntriesInWorklist, TDateTime timestamp );
    bool operator<( const QueueTicket& other ) const;
};

/*
From the collection of WorklistEntries, derives a list of samples that are NOT currently being analysed on the local machine BUT whose status suggests that they COULD be analysed on the local machine in the near future. Of course there is no certainty here. Another machine in the cluster might perform the required analyses.

The following Business Rules are relevant:

14. Rules for ordering samples in the bottom half of the list are as follows:

    1. Samples with 'P', 'C' or 'X' worklist entries get listed above samples that have only 'Q',  'S' or 'W' worklist entries.
    2. Samples with 'P', 'C' or 'X' are listed in such a way that the one with the most recent time_stamp is listed first.
    3. Samples with only 'Q', 'S' or 'W' worklist entries are listed so that the one with the oldest time_stamp is listed first.

18. Qualifications for being listed in the 'bottom half'

    The 'bottom half' is like a 'to do' list. For a sample to be listed in the bottom half, it must meet the following criteria

        1. It must have at least one worklist entry with a status of 'Q' or 'S', queued for the local machine or for the cluster.
        2. There must not be any ongoing analysis activity on the local machine for the sample. If there has been such activity, then it must have been closed off (Rule 9 above).

NB These Business Rules are copied from ZoHo (https://projects.zoho.com/portal/valc). Go to the latter for the most up-to-date versions.
*/
class QueuedSamplesBuilderFunction 
{
public:

    class Params
    {
    public:
        Params();
        virtual ~Params();
        virtual bool existsOpenRunForSample( const std::string& sampleDescriptor ) = 0;
        /*
            Implementations should call 'f->process' for all worklist entries. These calls must 
            be sequenced in such a way that if they relate to the same sample, they follow consecutively.
        */
        virtual void forEachWorklistEntry( QueuedSamplesBuilderFunction* f ) = 0;
        virtual bool isLocalOrCluster( int machineID ) = 0;
    };

    QueuedSamplesBuilderFunction( Params* p );
    void operator()( QueuedSamples* out );
    void process( const WorklistEntry* wle );

private:

    class ProcessState
    {
    public:
        ProcessState();
        virtual ~ProcessState();
        virtual QueueTicket     getQueueTicket()                                        = 0;
        virtual std::string     getSampleDescriptor()                                   = 0;
        virtual void            init( Params* p, const std::string& sampleDescriptor )  = 0;
        virtual bool            isInclusionJustifiedForSample()                         = 0;
        virtual void            process( const WorklistEntry* sampleWorklistEntry )     = 0;
    private:
        ProcessState( const ProcessState& );
        ProcessState& operator=( const ProcessState& );
    };

    typedef std::pair< std::string, QueueTicket > SampleDescriptorWithQueueTicket;

    ProcessState*                                   m_state;
    const boost::scoped_ptr< Params >               m_params;
    std::vector< SampleDescriptorWithQueueTicket >  m_queue;
    const boost::scoped_ptr<ProcessState>           m_stateBegin, 
                                                    m_stateIgnoring, 
                                                    m_stateConsidering;

    void flushState();

    friend bool compareOnQueueTicket( const SampleDescriptorWithQueueTicket&, const SampleDescriptorWithQueueTicket& );
};

/*
    Useful class for implementations of QueuedSamplesBuilderFunction::Params::forEachWorklistEntry.
*/
class WorklistDirectoryFunctionAdapter : public WorklistDirectory::Func
{
public:
    WorklistDirectoryFunctionAdapter( QueuedSamplesBuilderFunction* f );
    /*
        Invokes 'process' on m_callback, with 'wle' as a parameter.
    */
    void execute( const WorklistEntry* wle );

private:
    QueuedSamplesBuilderFunction* m_callback;
};

};

#endif


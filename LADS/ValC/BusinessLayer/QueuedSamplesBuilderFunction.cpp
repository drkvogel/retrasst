#include <algorithm>
#include <boost/bind.hpp>
#include <boost/foreach.hpp>
#include "QueuedSamplesBuilderFunction.h"
#include "Require.h"
#include <set>

namespace valc
{

QueueTicket::QueueTicket()
    :
    onlyQSW  (true),
    timestamp(2000, 1, 1, 0, 0, 0, 0)
{
}

QueueTicket::QueueTicket( const QueueTicket& other )
    :
    onlyQSW  ( other.onlyQSW ),
    timestamp( other.timestamp )
{
}

QueueTicket::QueueTicket( bool hasOnlyQSWEntriesInWorklist, TDateTime ts )
    :
    onlyQSW  ( hasOnlyQSWEntriesInWorklist ),
    timestamp( ts )
{
}

bool QueueTicket::operator<( const QueueTicket& other ) const
{
    if ( this->onlyQSW && other.onlyQSW )
    {
        return this->timestamp < other.timestamp;
    }
    else if ( other.onlyQSW && ! this->onlyQSW )
    {
        return true;
    }
    else if ( ! ( this->onlyQSW || other.onlyQSW ) )
    {
        return this->timestamp > other.timestamp;
    }
    else
    {
        // all possibilities of returning true have been exhausted.
        return false;
    }
}

QueuedSamplesBuilderFunction::ProcessState::ProcessState()
{
}

QueuedSamplesBuilderFunction::ProcessState::~ProcessState()
{
}

std::string QueuedSamplesBuilderFunction::ProcessState::getBarcode() const
{
    return m_barcode;
}

void QueuedSamplesBuilderFunction::ProcessState::init( Params* p, const std::string& sampleDescriptor, const std::string& barcode )
{
    m_sampleDescriptor = sampleDescriptor;
    m_barcode = barcode;
}

std::string QueuedSamplesBuilderFunction::ProcessState::getSampleDescriptor() const
{
    return m_sampleDescriptor;
}


class ProcessStateIgnoring : public QueuedSamplesBuilderFunction::ProcessState
{
public:
    ProcessStateIgnoring()                                                               {}
    QueueTicket     getQueueTicket()                                                     { return QueueTicket();      }
    bool            isInclusionJustifiedForSample()                                      { return false;              }
    void            process( const WorklistEntry* )                                      {}
};


class ProcessStateConsidering : public QueuedSamplesBuilderFunction::ProcessState
{
public:
    ProcessStateConsidering()
    {
    }

    QueueTicket getQueueTicket()
    {
        QueueTicket qp;
        qp.onlyQSW = true;

        BOOST_FOREACH( char status, m_statusValues )
        {
            switch( status )
            {
                case 'Q':
                case 'S':
                case 'W': 
                    continue; 
            }

            qp.onlyQSW = false;
            break;
        }

        qp.timestamp = qp.onlyQSW ? *(m_timestamps.begin()) : *(m_timestamps.rbegin());

        return qp;
    }

    void init( QueuedSamplesBuilderFunction::Params* p, const std::string& sampleDescriptor, const std::string& barcode ) 
    { 
        m_sampleDescriptor = sampleDescriptor;  
        m_barcode = barcode;
        m_params = p;
        m_somethingQueuedForLocalMachineOrCluster = false;
        m_statusValues.clear();
        m_timestamps  .clear();
    }

    bool isInclusionJustifiedForSample()
    {
        return m_somethingQueuedForLocalMachineOrCluster;
    }

    void process( const WorklistEntry* wle ) 
    {
        // BusinessRule 18.1 - something queued for the local machine or cluster
        m_somethingQueuedForLocalMachineOrCluster = m_somethingQueuedForLocalMachineOrCluster ||
            (   ( m_params->isLocalOrCluster( wle->getMachineID() ) ) 
                && 
                ( ( wle->getStatus() == 'S' ) || ( wle->getStatus() == 'Q' ) ) 
            );

        m_statusValues.push_back( wle->getStatus()    );
        m_timestamps  .insert   ( wle->getTimeStamp() );
    }

private:
    bool                                    m_somethingQueuedForLocalMachineOrCluster;
    QueuedSamplesBuilderFunction::Params*   m_params;
    std::vector< char      >                m_statusValues;
    std::set   < TDateTime >                m_timestamps;
};

QueuedSamplesBuilderFunction::Params::Params()
{
}

QueuedSamplesBuilderFunction::Params::~Params()
{
}

QueuedSamplesBuilderFunction::QueuedSamplesBuilderFunction( Params* p )
    : 
    m_params(p),
    m_stateIgnoring     ( new ProcessStateIgnoring()    ),
    m_stateConsidering  ( new ProcessStateConsidering() ),
    m_state             ( m_stateIgnoring.get()            )
{
}

bool compareOnQueueTicket( 
    const QueuedSamplesBuilderFunction::SampleDescriptorWithQueueTicket& q1, 
    const QueuedSamplesBuilderFunction::SampleDescriptorWithQueueTicket& q2 ) 
{
    const QueueTicket& qt1 = std::get<2>(q1);
    const QueueTicket& qt2 = std::get<2>(q2);
    return qt1 < qt2;
}

void QueuedSamplesBuilderFunction::operator()( QueuedSamples* out )
{
    m_state = m_stateIgnoring.get();
    m_queue.clear();

    m_params->forEachWorklistEntry( this );

    flushState();

    std::sort( m_queue.begin(), m_queue.end(), compareOnQueueTicket );

    BOOST_FOREACH( SampleDescriptorWithQueueTicket sd, m_queue )
    {
        out->push_back( QueuedSample( std::get<0>(sd) /* sample descriptor */, std::get<1>(sd) /* barcode */ ) );
    }
}

void QueuedSamplesBuilderFunction::process( const WorklistEntry* wle )
{
    const std::string sampleDescriptor = wle->getSampleDescriptor();
    // WorklistEntry instances get presented to this method grouped according to their sample.
    if ( m_state->getSampleDescriptor() != sampleDescriptor )
    {
        flushState();
        // Business Rule 18.2
        m_state = m_params->existsOpenRunForSample(sampleDescriptor) ? 
            (ProcessState*) (m_stateIgnoring   .get()):
            (ProcessState*) (m_stateConsidering.get());

        m_state->init( m_params.get(), sampleDescriptor, wle->getBarcode() );
    }

    m_state->process(wle);
}

void QueuedSamplesBuilderFunction::flushState()
{
    if ( m_state->isInclusionJustifiedForSample() )
    {
        m_queue.push_back( std::make_tuple( m_state->getSampleDescriptor(), m_state->getBarcode(), m_state->getQueueTicket() ) );
    }
}

WorklistDirectoryFunctionAdapter::WorklistDirectoryFunctionAdapter( QueuedSamplesBuilderFunction* f )
    :
    m_callback(f)
{
}

void WorklistDirectoryFunctionAdapter::execute( const WorklistEntry* wle )
{ 
    m_callback->process( wle );
}

}


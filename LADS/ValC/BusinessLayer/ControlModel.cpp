#include "AcquireCriticalSection.h"
#include "ControlModel.h"
#include "Require.h"
#include "RunIDC14n.h"
#include "SampleRunGroupModel.h"

namespace valc
{

ControlModel::ControlModel()
{
}

ControlModel::~ControlModel()
{
}

ControlModel::RuleResultPublisherAdapter::RuleResultPublisherAdapter( ControlModel* cm )
    :
    m_model(cm)
{
}

void ControlModel::RuleResultPublisherAdapter::publish( const RuleResults& results, int forResult )
{
    m_model->notifyQCEvaluationCompleted( results, forResult );
}

ControlModel::RuleEngineQueueListenerAdapter::RuleEngineQueueListenerAdapter( ControlModel* cm )
    :
    m_model(cm)
{
}

void ControlModel::RuleEngineQueueListenerAdapter::notifyQueued( const UncontrolledResult& r )
{
    m_model->notifyQCEvaluationStarted( r );
}

ControlModelImpl::ControlModelImpl()
    :
    m_sampleRunGroupModel(0),
    m_runIDC14n(0)
{
}

void ControlModelImpl::clear()
{
    m_sampleRunGroupModel = 0;
    m_pending.clear();
    m_cache.clear();
    m_runIDC14n = 0;
}

ControlStatus ControlModelImpl::getControlStatus( int testID, const std::string& runID ) const
{
    typedef std::vector< std::string > RunIDs;
    RunIDs runIDs;
    std::vector< QCControl > preceding, following;

    const std::string runIDCanonicalForm = m_runIDC14n->toCanonicalForm(runID);

    m_sampleRunGroupModel->listPrecedingQCRuns( runIDCanonicalForm, runIDs );

    m_cache.searchForMatchingQCControls( runIDs, testID, preceding );

    runIDs.clear();

    m_sampleRunGroupModel->listFollowingQCRuns( runIDCanonicalForm, runIDs );

    m_cache.searchForMatchingQCControls( runIDs, testID, following );

    return ControlStatus( preceding, following );
}

void ControlModelImpl::notifyQCEvaluationStarted( const UncontrolledResult& r )
{
    paulst::AcquireCriticalSection a(m_cs);

    m_pending.insert( std::make_pair(r.resultID, r) );
}

void ControlModelImpl::notifyQCEvaluationCompleted( const RuleResults& rr, int forResult )
{
    paulst::AcquireCriticalSection a(m_cs);

    require( m_pending.count( forResult ) );

    std::map< int, UncontrolledResult >::iterator i = m_pending.find( forResult );

    UncontrolledResult r = i->second;

    m_pending.erase( i );

    m_cache.add( r, rr );
}

void ControlModelImpl::setRunIDC14n( RunIDC14n* r )
{
    m_runIDC14n = r;
    m_cache.setRunIDC14n(r);
}

void ControlModelImpl::setSampleRunGroupModel( SampleRunGroupModel* m )
{
    m_sampleRunGroupModel = m;
}

}


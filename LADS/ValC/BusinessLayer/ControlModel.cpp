#include "AcquireCriticalSection.h"
#include "ControlModel.h"
#include "IDTokenSequence.h"
#include "Require.h"
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

void ControlModel::RuleEngineQueueListenerAdapter::notifyQueued( const UncontrolledResult& r, const IDToken& runID )
{
    m_model->notifyQCEvaluationStarted( r, runID );
}

ControlModelImpl::ControlModelImpl()
    :
    m_sampleRunGroupModel(0)
{
}

void ControlModelImpl::clear()
{
    m_sampleRunGroupModel = 0;
    m_pending.clear();
    m_cache.clear();
}

ControlStatus ControlModelImpl::getControlStatus( int testID, const IDToken& sampleRunID ) const
{
    IDTokenSequence runIDs;
    std::vector< QCControl > preceding, following;

    m_sampleRunGroupModel->listPrecedingQCRuns( sampleRunID, runIDs );

    m_cache.searchForMatchingQCControls( runIDs, testID, preceding );

    runIDs.clear();

    m_sampleRunGroupModel->listFollowingQCRuns( sampleRunID, runIDs );

    m_cache.searchForMatchingQCControls( runIDs, testID, following );

    return ControlStatus( QCControls(preceding), QCControls(following) );
}

void ControlModelImpl::notifyQCEvaluationStarted( const UncontrolledResult& r, const IDToken& runID )
{
    paulst::AcquireCriticalSection a(m_cs);

    m_pending.insert( std::make_pair(r.resultID, std::make_pair(r,runID) ) );
}

void ControlModelImpl::notifyQCEvaluationCompleted( const RuleResults& rr, int forResult )
{
    paulst::AcquireCriticalSection a(m_cs);

    require( m_pending.count( forResult ) );

    auto i = m_pending.find( forResult );

    UncontrolledResult r = i->second.first;
    IDToken runID = i->second.second;

    m_pending.erase( i );

    m_cache.add( r, runID, rr );
}

void ControlModelImpl::setSampleRunGroupModel( SampleRunGroupModel* m )
{
    m_sampleRunGroupModel = m;
}

}

